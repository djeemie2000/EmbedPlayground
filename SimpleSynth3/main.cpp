
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

    COscillatorSource<float, SamplingFrequency> m_Source;
    CAnalogOutRenderer<float> m_Renderer;

    CRenderManager<COscillatorSource<float, SamplingFrequency>, CAnalogOutRenderer<float>> m_RenderManager;

    SController()
     : m_Source(SamplingFrequency)
     , m_Renderer(D13)
     , m_RenderManager()
    {
    }

    void Start()
    {
        m_RenderManager.Start(SamplingFrequency, &m_Source, &m_Renderer);
    }
};


void TestRenderTimings(SController& Controller)
{
    g_Serial.printf("Test render timings...\r\n");

    const int SamplingFrequency = 96000;

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
        ++Counter;
        if(Counter % (5*1000) == 0)
        {
            g_Serial.printf("ReadControls %d : %d Freq=%f Damp=%f Excite %f Attack %f \r\n",
                              Counter,
                              //Controller.m_Source.m_PitchCV,
                              Controller.m_Source.m_Amplitude,
                              Controller.m_Source.m_FrequencyHz,
                              Controller.m_Source.m_Damp,
                              Controller.m_Source.m_Excite,
                              Controller.m_Source.m_Attack);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
