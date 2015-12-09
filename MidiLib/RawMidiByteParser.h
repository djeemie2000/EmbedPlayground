#pragma once

#include <cstdint>


/*
       MIDI COMMANDS
       -------------------------------------------------------------------
       name                 status      param 1          param 2
       -------------------------------------------------------------------
       note off             0x80 & C       key #            velocity
       note on              0x90 & C       key #            velocity
       poly key pressure    0xA0 & C       key #            pressure value
       control change       0xB0 & C       control #        control value
       program change       0xC0 & C       program #        --
       mono key pressure    0xD0 & C       pressure value   --
       pitch bend           0xE0 & C       range (LSB)      range (MSB)
       system               0xF0 & C       manufacturer     model
       -------------------------------------------------------------------
       C is the channel number, from 0 to 15;
       -------------------------------------------------------------------
       source: http://ftp.ec.vanderbilt.edu/computermusic/musc216site/MIDI.Commands.html

       In this program the pitch bend range will be transmitter as
       one single 8-bit number. So the end result is that MIDI commands
       will be transmitted as 3 bytes, starting with the operation byte:

       buf[0] --> operation/channel
       buf[1] --> param1
       buf[2] --> param2        (param2 not transmitted on program change or key press)
*/

class CRawMidiParser
{
public:
    CRawMidiParser()
        : m_Command(0x00)
        , m_Channel(0)
        , m_Data()
        , m_DataSize(0)
    {}

    template<class IMidiHandler>
    void Parse(const std::uint8_t RawMidiByte, IMidiHandler& MidiHandler)
    {
        std::uint8_t IsCommand = RawMidiByte & 0x80;
        if(IsCommand)
        {
            m_Command = RawMidiByte & 0xF0;
            m_Channel = RawMidiByte & 0x0F;
            m_DataSize = 0;
        }
        else
        { // data
            if(m_DataSize<MaxDataSize)
            {
                m_Data[m_DataSize++] = RawMidiByte;

                // check if command + data is complete -> depends on command
                if(m_Command == NoteOffCommand)
                {
                    if(m_DataSize == 2)
                    {
                        int Note = m_Data[0];
                        int Velocity = m_Data[1];
                        MidiHandler.OnNoteOff(Note, Velocity, m_Channel);
                    }
                }
                else if(m_Command == NoteOnCommand)
                {
                    if(m_DataSize == 2)
                    {
                        int Note = m_Data[0];
                        int Velocity = m_Data[1];
                        MidiHandler.OnNoteOn(Note, Velocity, m_Channel);
                    }
                }
                else if(m_Command == ControlChangeCommand)
                {
                    if(m_DataSize == 2)
                    {
                        int Controller = m_Data[0];
                        int Value = m_Data[1];
                        MidiHandler.OnControlChange(Controller, Value, m_Channel);
                    }
                }
                //else unknown or unhandled
            }
            //else data buffer full
        }
    }


private:
    static const std::uint8_t NoteOffCommand = 0x80;
    static const std::uint8_t NoteOnCommand = 0x90;
    static const std::uint8_t PolyKeyPressureCommand = 0xA0;
    static const std::uint8_t ControlChangeCommand = 0xB0;
    static const std::uint8_t ProgramChangedCommand = 0xC0;
    static const std::uint8_t MonoKeyPressureCommand = 0xD0;
    static const std::uint8_t PitchBendCommand = 0xE0;
    static const std::uint8_t SystemCommand = 0xF0;

    static const int MaxDataSize = 3;
    std::uint8_t m_Command;
    int m_Channel;
    std::uint8_t m_Data[MaxDataSize];
    int m_DataSize;
};
