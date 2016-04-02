/*
 * This is the original mbed example
 */

#include "mbed.h"
#include "AnalogOutBuffer.h"
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

#include "Oscillator.h"
#include "AnalogOutRenderer.h"
#include "RenderManager.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

// F446RE AnalogOut: A2 or D13

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

    CInterruptInManager m_Trigger;
    int m_Amplitude;
    float m_FrequencyHz;

    AnalogIn m_PitchCVIn;
    float m_PitchCV;

    COscillator<float> m_Oscillator;
    CAnalogOutRenderer<int> m_Renderer;

    CRenderManager<SController, CAnalogOutRenderer<int>> m_RenderManager;

    SController()
     : m_Trigger(D7)
     , m_Amplitude(0)
     , m_FrequencyHz(110.0f)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_Oscillator(SamplingFrequency)
     , m_Renderer(D13)
     , m_RenderManager()
    {
    }

    void Start()
    {
        m_RenderManager.Start(SamplingFrequency, this, &m_Renderer);
        m_FrequencyHz = 110.0f;
        m_Trigger.Start();
    }

    int Render()
    {
        int OutValue = m_Oscillator(m_FrequencyHz);
        if(!m_Amplitude)
        {
            OutValue = 1<<15;
        }
        return OutValue;
    }

    void ReadControls()
    {
        // triggers CV pitch
        m_Amplitude = m_Trigger.Read();
        //  sample pitch CV
        SamplePitch();
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
    pc.printf("\r\n --- SimpleSynth 2 --- ");

    wait(1);

    // test timings on oscillator!
    TestOscillatorTimings();

    SController Controller;
    // TODO some timings and tests on controller!

    pc.printf("Start");
    Controller.Start();

    int Counter = 0;
    while(true)
    {
        Controller.ReadControls();
        ++Counter;
        if(Counter % (2*1000) == 0)
        {
            pc.printf("WriteToBuffer %d Freq=%f \r\n", Counter, Controller.m_FrequencyHz);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
