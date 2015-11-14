#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillator.h"
#include "OnePoleFilter.h"
#include "MidiNoteFrequencies.h"

MCP4822 g_MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

Serial g_Serial(USBTX, USBRX); // tx, rx
Timer g_Timer;

const int g_SamplingFrequency = 30000;
CSimpleOscillator<float> g_Oscillator(g_SamplingFrequency);
COnePoleLowPassFilter<float> g_LPF;

void TestDacSpeed()
{
    g_Timer.reset();
    g_Timer.start();
    int NumRepeats = 40000;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        g_MCP.writeAB((8*Repeat)%4096, (8*Repeat)%4096);
    }
    g_Timer.stop();
    g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());

    g_Serial.printf("---------------\r\n");
}

void TestOscillatorSpeed()
{
    const int SamplingFrequency = 40000;
    CSimpleOscillator<float> Oscillator(SamplingFrequency);

    for(std::size_t Operator = 0; Operator<CSelectableOperatorFactory::SelectionList().size(); ++Operator)
    {
        Oscillator.SelectWaveform(Operator);
        Oscillator.SetFrequency(110.0f);//faster if not always recalculating phase step from frequency
        g_Timer.reset();
        g_Timer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += Oscillator();
        }
        g_Timer.stop();
        g_Serial.printf("Operator %s -> %f \r\n", CSelectableOperatorFactory::SelectionList()[Operator].c_str(), Sum);
        g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());
    }

    g_Serial.printf("---------------\r\n");
}

void TestLowPassFilterSpeed()
{
    const int SamplingFrequency = 40000;
    CSimpleOscillator<float> Oscillator(SamplingFrequency);
    Oscillator.SelectWaveform(0);
    Oscillator.SetFrequency(110.0f);
    COnePoleLowPassFilter<float> LPF;
    COnePoleHighPassFilter<float> HPF;
    CMultiStageFilter<float, COnePoleLowPassFilter<float>, 4> MLPF;

    {
        LPF.SetParameter(0.5);
        g_Timer.reset();
        g_Timer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += LPF(Oscillator());
        }
        g_Timer.stop();
        g_Serial.printf("Operator %s -> LPF -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());
    }

    {
        g_Timer.reset();
        g_Timer.start();
        float Sum = 0;
        float Parameter = 0.5f;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Parameter += 0.0001f;
            if(1.0f<=Parameter)
            {
                Parameter = 0.5f;
            }
            Sum += LPF(Oscillator(), Parameter);
        }
        g_Timer.stop();
        g_Serial.printf("Operator %s -> LPF(Parameter) -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());
    }

    {
        MLPF.SetStages(4);
        g_Timer.reset();
        g_Timer.start();
        float Sum = 0;
        float Parameter = 0.5f;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Parameter += 0.0001f;
            if(1.0f<=Parameter)
            {
                Parameter = 0.5f;
            }
            Sum += MLPF(Oscillator(), Parameter);
        }
        g_Timer.stop();
        g_Serial.printf("Operator %s -> MLPF(Parameter) -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());
    }

    {
        LPF.SetParameter(0.5);
        g_Timer.reset();
        g_Timer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += LPF(LPF(LPF(LPF(Oscillator()))));
        }
        g_Timer.stop();
        g_Serial.printf("Operator %s -> LPF x4 -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());
    }

    g_Serial.printf("---------------\r\n");
}

void OnTick()
{
    float Out = g_LPF(g_Oscillator());
    // convert and clamp to [0, 4096[
    int Value = Out*2048;
    Value += 2048;
    if(Value<0)
    {
        Value = 0;
    }
    else if(4095<Value)
    {
        Value = 4095;
    }

    g_MCP.writeAB(Value, Value);
}

int main()
{
    g_Serial.printf("SimpleSynth... \r\n \r\n");

    // self test
    g_Serial.printf("Testing... \r\n \r\n");
    TestDacSpeed();
    TestOscillatorSpeed();
    TestLowPassFilterSpeed();

    // setup
    g_Serial.printf("Starting... \r\n \r\n");

    AnalogIn In0(A0);
    AnalogIn In1(A1);
    AnalogIn In2(A2);

    g_Oscillator.SelectWaveform(3);
    g_Oscillator.SetFrequency(110.0f);
    g_LPF.SetParameter(0.1);

    float PeriodMicroSeconds = 1000000.0f/g_SamplingFrequency;
    g_Serial.printf("Fs = %d Ps = %f  \r\n", g_SamplingFrequency, PeriodMicroSeconds);

    Ticker myTicker;
    myTicker.attach_us(OnTick, PeriodMicroSeconds);

    g_Serial.printf("Started! \r\n \r\n");

    int PrevVal0 = 0;
    int PrevVal1 = 0;
    int PrevVal2 = 0;

    while(1)
    {
//        g_Serial.printf("Running \r\n");

        int Val0 = In0.read_u16();
        int Val1 = In1.read_u16();
        int Val2 = In2.read_u16();
        //g_Serial.printf("%d %d %d \r\n", Val0, Val1, Val2);

        if(PrevVal0>>8!=Val0>>8)
        {
            int Operator = Val0*(CSelectableOperatorFactory::SelectionList().size()-1)>>16;
            g_Oscillator.SelectWaveform(Operator);
            g_Serial.printf("Operator %d %s \r\n", Operator, CSelectableOperatorFactory::SelectionList()[Operator].c_str());
        }
        PrevVal0 = Val0;

        if(PrevVal1>>8!=Val1>>8)
        {
            int MidiNote = Val1>>9;//16 to 7 bits
            float Freq = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;
            g_Oscillator.SetFrequency(Freq);
            g_Serial.printf("MidiNote %d Freq %f \r\n", MidiNote, Freq);
        }
        PrevVal1 = Val1;

        if(PrevVal2>>8!=Val2>>8)
        {
            float CutOff = Val2/65536.0f;
            g_LPF.SetParameter(CutOff);
            g_Serial.printf("CutOff %f \r\n", Val2);
        }
    }
}
