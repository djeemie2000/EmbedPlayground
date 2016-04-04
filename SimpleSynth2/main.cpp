
#include "mbed.h"

// includes for libraries
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

// includes for project specific stuff
#include "Oscillator.h"
#include "OscillatorSource.h"
#include "AnalogOutRenderer.h"
#include "RenderManager.h"

// global serial out
Serial g_Serial(USBTX, USBRX);

// F446RE AnalogOut: A2 or D13

void TestOscillatorTimings()
{
    g_Serial.printf("Test oscillator timings...\r\n");

    const int SamplingFrequency = 96000;
    COscillator<float> Oscillator(SamplingFrequency);
    float FrequencyHz = 110.0f;
    float Amplitude = 1;

    Timer MyTimer;
    MyTimer.start();
    float UseValue =0;
    for(int Repeat = 0; Repeat<SamplingFrequency; ++Repeat)
    {
        UseValue += Oscillator(FrequencyHz, Amplitude);
    }
    MyTimer.stop();
    int Elapsed = MyTimer.read_ms();
    g_Serial.printf("%d samples %d mSec (%f)\r\n", SamplingFrequency, Elapsed, UseValue);
}

struct SController
{
    static const int SamplingFrequency = 96000;//44100;//?

    CInterruptInManager m_Trigger;

    AnalogIn m_PitchCVIn;
    float m_PitchCV;

    COscillatorSource<float> m_OscillatorSource;
    CAnalogOutRenderer<int> m_Renderer;

    CRenderManager<COscillatorSource<float>, CAnalogOutRenderer<int>> m_RenderManager;

    SController()
     : m_Trigger(D7)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_OscillatorSource(SamplingFrequency)
     , m_Renderer(D13)
     , m_RenderManager()
    {
    }

    void Start()
    {
        m_RenderManager.Start(SamplingFrequency, &m_OscillatorSource, &m_Renderer);
        m_OscillatorSource.SetFrequencyHz(110.0f);
        m_OscillatorSource.SetAmplitude(0);
        m_Trigger.Start();
    }

    void ReadControls()
    {
        // triggers CV pitch
        m_OscillatorSource.SetAmplitude(m_Trigger.Read());
        //  sample pitch CV
        SamplePitch();
     }

     void SamplePitch()
     {
         // TODO try a one pole LPF on Value???
         float Value = m_PitchCVIn.read();
         m_PitchCV = (15*m_PitchCV+Value)/16;

         const float ReferenceVoltage = 3.3f;
         const float VoltageDivider = 2.07f;// 2.03f;//????
         float Voltage = VoltageDivider*m_PitchCV*ReferenceVoltage;
         int MidiNote = Voltage*12 + 0.5f;

         float Freq = GetMidiNoteFrequencyMilliHz(MidiNote+36)/1000.0f;//starts with C1?
         m_OscillatorSource.SetFrequencyHz(Freq);
         //g_Serial.printf("%f = %f V  -> Note %d %f Hz \r\n", Value, Voltage, MidiNote, Freq);
     }
};


int main()
{
    g_Serial.printf("\r\n --- SimpleSynth 2 --- ");

    wait(1);

    // test timings on oscillator!
    TestOscillatorTimings();

    SController Controller;
    // TODO some timings and tests on controller!

    g_Serial.printf("Start...\r\n");
    Controller.Start();

    int Counter = 0;
    while(true)
    {
        Controller.ReadControls();
        ++Counter;
        if(Counter % (2*1000) == 0)
        {
            g_Serial.printf("ReadControls %d : %f %d Freq=%f \r\n",
                              Counter,
                              Controller.m_PitchCV,
                              Controller.m_OscillatorSource.m_Amplitude,
                              Controller.m_OscillatorSource.m_FrequencyHz);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
