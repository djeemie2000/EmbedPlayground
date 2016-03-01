/*
 * This is the original mbed example
 */

#include "mbed.h"
#include "AnalogOutBuffer.h"
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

#include "PhaseStep.h"
#include "PhaseAccumulator.h"
#include "Pulse.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

//AnalogOut myAnOut(A2);// A2 or D13

template<class T>
class COscillator
{
public:
    COscillator(T SamplingFrequency)
     : m_PhaseStepA(SamplingFrequency)
     , m_PhaseAccumulatorA()
     , m_DetuneA(1.01f)
     , m_PhaseStepB(SamplingFrequency)
     , m_PhaseAccumulatorB()
     , m_DetuneB(1/m_DetuneA)
     , m_PhaseStepSub(SamplingFrequency)
     , m_PhaseAccumulatorSub()
     , m_DetuneSub(0.5f)
     , m_OscillatorSub()
    {}

    int operator()(T FrequencyHz)//TODO amplitude/gate
    {
        T Value = m_PhaseAccumulatorA(m_PhaseStepA(FrequencyHz*m_DetuneA))
                + m_PhaseAccumulatorB(m_PhaseStepB(FrequencyHz*m_DetuneB))
                + m_OscillatorSub(m_PhaseAccumulatorSub(m_PhaseStepSub(FrequencyHz*m_DetuneSub)));

        return (1<<15)*(1+0.33f*Value);
    }

private:
    CPhaseStep<T> m_PhaseStepA;
    CPhaseAccumulator<T> m_PhaseAccumulatorA;
    T m_DetuneA;

    CPhaseStep<T> m_PhaseStepB;
    CPhaseAccumulator<T> m_PhaseAccumulatorB;
    T m_DetuneB;

    CPhaseStep<T> m_PhaseStepSub;
    CPhaseAccumulator<T> m_PhaseAccumulatorSub;
    const T m_DetuneSub;
    CPulsePos<T> m_OscillatorSub;
};

void TestOscillatorTimings()
{
    pc.printf("Test oscillator timings...\r\n");

    const int SamplingFrequency = 96000;
    COscillator<float> Oscillator(SamplingFrequency);
    float FrequencyHz = 110.0f;

    Timer MyTimer;
    MyTimer.start();
    float UseValue =0;
    for(int Repeat = 0; Repeat<SamplingFrequency; ++Repeat)
    {
        UseValue += Oscillator(FrequencyHz);
    }
    MyTimer.stop();
    int Elapsed = MyTimer.read_ms();
    pc.printf("%d samples %d mSec (%f)\r\n", SamplingFrequency, Elapsed, UseValue);
}

struct SController
{

    static const int SamplingFrequency = 96000;//44100;//?
    static const int AnalogOutBufferSize = SamplingFrequency/10;

    CAnalogOutBuffer<AnalogOutBufferSize> m_AnalogOutBuffer;

    CInterruptInManager m_Trigger;
    int m_Amplitude;
    float m_FrequencyHz;

    AnalogIn m_PitchCVIn;
    float m_PitchCV;

    COscillator<float> m_Oscillator;

    SController()
     : m_AnalogOutBuffer(D13)
     , m_Trigger(D7)
     , m_Amplitude(0)
     , m_FrequencyHz(110.0f)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_Oscillator(SamplingFrequency)
    {

    }

    void Start()
    {
        m_AnalogOutBuffer.Start(SamplingFrequency);
        m_FrequencyHz = 110.0f;
        m_Trigger.Start();
    }

    void WriteToBuffer()
    {
        static const int WriteBlockSize = 256;
        auto& Buffer = m_AnalogOutBuffer.GetBuffer();
        int NumAvailable = Buffer.AvailableForWrite();
        if(WriteBlockSize<=NumAvailable)
        {
            // write 512 samples
            for(int idx = 0; idx<WriteBlockSize; ++idx)
            {
                //float Tmp = m_Amplitude * m_Phasor(m_PhaseStep(m_FrequencyHz));
                //int OutValue = (1<<15)*(1 + Tmp);
                int OutValue = m_Oscillator(m_FrequencyHz);
                if(!m_Amplitude)
                {
                    OutValue = 1<<15;
                }
                Buffer.Write(OutValue);
            }
        }
    }

    void ReadControls()
    {
        //TODO triggers CV pitch
        m_Amplitude = m_Trigger.Read();
        //if(0<m_Trigger.RiseCount())
        //{
        //    m_Trigger.Clear();
            //  sample pitch CV
            SamplePitch();
        //}
     }

     void SamplePitch()
     {
         float Value = m_PitchCVIn.read();
         m_PitchCV = (15*m_PitchCV+Value)/16;
         // TODO try a one pole LPF on Value???

         const float ReferenceVoltage = 3.3f;
         const float VoltageDivider = 2.07f;// 2.03f;//????
         float Voltage = VoltageDivider*m_PitchCV*ReferenceVoltage;
         int MidiNote = Voltage*12 + 0.5f;

         float Freq = GetMidiNoteFrequencyMilliHz(MidiNote+24)/1000.0f;//starts with C1?

         m_FrequencyHz = Freq;

         //pc.printf("%f = %f V  -> Note %d %f Hz \r\n", Value, Voltage, MidiNote, Freq);
     }
};


int main()
{
    pc.printf("SimpleSynth 2 !");

    // TODO timings on oscillator!
    TestOscillatorTimings();

    SController Controller;
    // TODO some timings and tests on controller!

    //
    Controller.WriteToBuffer();
    Controller.WriteToBuffer();
    Controller.WriteToBuffer();
    Controller.WriteToBuffer();
    Controller.Start();

    int Counter = 0;
    while(true)
    {
        Controller.ReadControls();
        Controller.WriteToBuffer();
        ++Counter;
        if(Counter % 1000 == 0)
        {
            pc.printf("WriteToBuffer %d Freq=%f \r\n", Counter, Controller.m_FrequencyHz);
        }
        else
        {
            wait_us(1000);
        }
    }

  return 0;
}
