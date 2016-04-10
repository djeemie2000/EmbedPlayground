
#include "mbed.h"

// includes for libraries
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

// includes for project specific stuff
#include "Oscillator.h"
#include "OscillatorSource.h"
#include "AnalogOutRenderer.h"
#include "RenderManager.h"
#include "AnalogInSource.h"

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
    float DetuneA = 1.1f;
    float DetuneB = 0.9f;

    Timer MyTimer;
    MyTimer.start();
    float UseValue =0;
    for(int Repeat = 0; Repeat<SamplingFrequency; ++Repeat)
    {
        UseValue += Oscillator(FrequencyHz, Amplitude, DetuneA, DetuneB);
    }
    MyTimer.stop();
    int Elapsed = MyTimer.read_ms();
    g_Serial.printf("%d samples %d mSec (%f)\r\n", SamplingFrequency, Elapsed, UseValue);
}

struct SController
{
    static const int SamplingFrequency = 96000;

    CInterruptInManager m_Trigger;

    AnalogIn m_PitchCVIn;
    float m_PitchCV;

    AnalogIn m_DetuneCV;
    AnalogIn m_CutoffCV;

    COscillatorSource<float> m_Source;
    CAnalogInSource<float> m_InSource;
    CAnalogOutRenderer<float> m_Renderer;

    CRenderManager<COscillatorSource<float>, CAnalogOutRenderer<float>> m_RenderManager;
    //CRenderManager<CAnalogInSource<float>, CAnalogOutRenderer<float>> m_RenderManager;

    SController()
     : m_Trigger(D7)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_DetuneCV(A0)
     , m_CutoffCV(A1)
     , m_Source(SamplingFrequency)
     , m_InSource(A2)
     , m_Renderer(D13)
     , m_RenderManager()
    {
    }

    void Start()
    {
        m_RenderManager.Start(SamplingFrequency, &m_Source, &m_Renderer);
        //m_RenderManager.Start(SamplingFrequency, &m_InSource, &m_Renderer);
        m_Source.SetFrequencyHz(110.0f);
        m_Source.SetAmplitude(1);
        m_Trigger.Start();
    }

    void ReadControls()
    {
        // triggers CV pitch
        m_Source.SetAmplitude(m_Trigger.Read());

        //  sample pitch CV
        SamplePitch();
        // CV detune: CV [0,1]
        m_Source.SetDetune(m_DetuneCV.read());
        //
        m_Source.SetCutoff(m_CutoffCV.read());
        //
        m_InSource.SetAmplitude(1);
     }

     void SamplePitch()
     {
         // TODO try a one pole LPF on Value???
         float Val = m_PitchCVIn.read();
         //m_PitchCV = Value;
         m_PitchCV = (Val+m_PitchCV)/2;//
         //m_PitchCV = (15*m_PitchCV+Value)/16;

         float ReferenceVltage = 3.3f;
         float VltageMult = 2.03f;//1.99;//2.07f;// 2.03f;//????
         float Vltage = VltageMult*m_PitchCV*ReferenceVltage;
         int MidiNote = Vltage*12 + 0.5f;

         float Freq = GetMidiNoteFrequencyMilliHz(MidiNote+24)/1000.0f;//starts with C1?
         m_Source.SetFrequencyHz(Freq);

         //g_Serial.printf("%f = %f V  -> Note %d %f Hz \r\n", m_PitchCV, Vltage, MidiNote, Freq);
     }
};


void TestRenderTimings(SController& Controller)
{
    g_Serial.printf("Test render timings...\r\n");

    const int SamplingFrequency = 96000;
    Controller.m_InSource.SetAmplitude(1);

    Timer MyTimer;
    MyTimer.start();
    for(int Repeat = 0; Repeat<SamplingFrequency; ++Repeat)
    {
        Controller.m_Renderer.Render(Controller.m_Source.Render());
        //Controller.m_Renderer.Render(Controller.m_InSource.Render());
    }
    MyTimer.stop();

    int Elapsed = MyTimer.read_ms();
    g_Serial.printf("%d samples %d mSec \r\n", SamplingFrequency, Elapsed);
}


int main()
{
    g_Serial.printf("\r\n --- SimpleSynth 2 --- ");

    wait(1);

    // test timings on oscillator!
    TestOscillatorTimings();

    SController Controller;
    // TODO some timings and tests on controller!
    // timing analog out write ( m_Renderer.Render(m_Source.Render()) )
    TestRenderTimings(Controller);
    wait(2);

    g_Serial.printf("Start...\r\n");
    Controller.Start();

    int Counter = 0;
    while(true)
    {
        Controller.ReadControls();
        ++Counter;
        if(Counter % (5*1000) == 0)
        {           
            g_Serial.printf("ReadControls %d : %f %f Freq=%f Detune=%f %f \r\n",
                              Counter,
                              Controller.m_PitchCV,
                              Controller.m_Source.m_Amplitude,
                              Controller.m_Source.m_FrequencyHz,
                              Controller.m_Source.m_DetuneA,
                              Controller.m_Source.m_DetuneB);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
