/*
 * This is the original mbed example
 */

#include "mbed.h"
#include "AnalogOutBuffer.h"
#include "InterruptInManager.h"
#include "MidiNoteFrequencies.h"
#include "PhaseStep.h"
#include "PhaseAccumulator.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

//AnalogOut myAnOut(A2);// A2 or D13

struct SController
{

    static const int SamplingFrequency = 96000;//44100;//?
    static const int AnalogOutBufferSize = SamplingFrequency/10;

    CAnalogOutBuffer<AnalogOutBufferSize> m_AnalogOutBuffer;

    CInterruptInManager m_Trigger;
    int m_Amplitude;

    AnalogIn m_PitchCVIn;
    float m_PitchCV;

    float m_FrequencyHz;
    CPhaseStep<float> m_PhaseStep;
    CPhaseAccumulator<float> m_Phasor;


    SController()
     : m_AnalogOutBuffer(D13)
     , m_Trigger(D7)
     , m_Amplitude(0)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_PhaseStep(SamplingFrequency)
     , m_Phasor()
    {

    }

    void Start()
    {
        m_AnalogOutBuffer.Start(SamplingFrequency);
        m_FrequencyHz = 110.0f;
        m_PhaseStep.SetFrequency(m_FrequencyHz);//
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
                float Tmp = m_Amplitude * m_Phasor(m_PhaseStep(m_FrequencyHz));
                int OutValue = (1<<15)*(1 + Tmp);
                Buffer.Write(OutValue);
                //Buffer.Write(m_Amplitude ? idx<<8 : 1<<15);// scale 512 (9 bits) to 16 bits ?
            }
            //pc.printf("Write block, %d available \r\n", NumAvailable);
        }
        else
        {
            //pc.printf("Cannot write block, %d available \r\n", NumAvailable);
            //wait_ms(1);//too large??
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
         const float VoltageDivider = 2.03f;
         float Voltage = VoltageDivider*m_PitchCV*ReferenceVoltage;
         int MidiNote = Voltage*12 + 0.5f;

         float Freq = GetMidiNoteFrequencyMilliHz(MidiNote+24)/1000.0f;//starts with C1?

         m_FrequencyHz = Freq;

         m_PhaseStep.SetFrequency(m_FrequencyHz);

         //pc.printf("%f = %f V  -> Note %d %f Hz \r\n", Value, Voltage, MidiNote, Freq);
     }
};


int main()
{
    pc.printf("SimpleSynth 2 !");

    SController Controller;

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
        if(Counter % 5000 == 0)
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
