
#include "mbed.h"

// includes for libraries
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"

// includes for project specific stuff
#include "OscillatorSource.h"
#include "AnalogOutRenderer.h"
#include "RenderManager.h"

// global serial out
Serial g_Serial(USBTX, USBRX);

// F446RE AnalogOut: A2 or D13

struct SController
{
    static const int SamplingFrequency = 96000;
    static const int MinFreqHz = 32;
    static const int Capacity = SamplingFrequency/MinFreqHz;

    COscillatorSource<float, Capacity> m_Source;//TODO stereo
    CAnalogOutRenderer<float> m_Renderer;

    CRenderManager<COscillatorSource<float, Capacity>, CAnalogOutRenderer<float>> m_RenderManager;

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
    }
    MyTimer.stop();

    int Elapsed = MyTimer.read_ms();
    g_Serial.printf("%d samples %d mSec \r\n", SamplingFrequency, Elapsed);
}


int main()
{
    g_Serial.printf("\r\n --- SimpleSynth 3 --- \r\n");

    wait(1);

    // test timings on oscillator!
    //TestOscillatorTimings();

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
        if(Counter % (1*1000) == 0)
        {
//            g_Serial.printf("ReadControls \r\n");
            g_Serial.printf("ReadControls %d : %d Freq=%f Damp=%f Excite %f Attack %f Trigger %d %f \r\n",
                              Counter,
                              //Controller.m_Source.m_PitchCV,
                              Controller.m_Source.m_Amplitude,
                              Controller.m_Source.m_FrequencyHz,
                              Controller.m_Source.m_Damp,
                              Controller.m_Source.m_Excite,
                              Controller.m_Source.m_Attack,
                              Controller.m_Source.m_Cnt, Controller.m_Source.m_TriggerFreq);
        }
        else
        {
            wait_ms(1);
        }
    }

  return 0;
}
