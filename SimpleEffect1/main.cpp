
#include "mbed.h"

// includes for libraries
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

// includes for project specific stuff
#include "StereoAnalogOutRenderer.h"
#include "RenderManager.h"
#include "StereoAnalogInSource.h"

// global serial out
Serial g_Serial(USBTX, USBRX);

// F446RE AnalogOut: A2 or D13

struct SController
{
    static const int SamplingFrequency = 96000;

    CStereoAnalogInSource<float> m_InSource;
    CStereoAnalogOutRenderer<float> m_Renderer;

    CRenderManager<CStereoAnalogInSource<float>, CStereoAnalogOutRenderer<float>> m_RenderManager;

    SController()
     : m_InSource(A3)
     , m_Renderer(D13, A2)
     , m_RenderManager()
    {
    }

    void Start()
    {
        m_RenderManager.Start(SamplingFrequency, &m_InSource, &m_Renderer);
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
        Controller.m_Renderer.Render(Controller.m_InSource.Render());
    }
    MyTimer.stop();

    int Elapsed = MyTimer.read_ms();
    g_Serial.printf("%d samples %d mSec \r\n", SamplingFrequency, Elapsed);
}


int main()
{
    g_Serial.printf("\r\n --- SimpleSynth 2 --- ");
    wait(1);

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
            g_Serial.printf("ReadControls %d : ? CutOff=%f %f \r\n",
                              Counter,
                              //Controller.m_InSource.m_Amplitude,
                              Controller.m_InSource.m_CutOff);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
