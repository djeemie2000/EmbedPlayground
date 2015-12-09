#include "RawMidiParser.h"
#include "MidiHandlerI.h"

CRawMidiParser::CRawMidiParser(IMidiHandler &MidiHandler)
    : m_MidiHandler(MidiHandler)
{
}

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

namespace
{

const std::uint8_t NoteOffCommand = 0x80;
const std::uint8_t NoteOnCommand = 0x90;
const std::uint8_t PolyKeyPressureCommand = 0xA0;
const std::uint8_t ControlChangeCommand = 0xB0;
const std::uint8_t ProgramChangedCommand = 0xC0;
const std::uint8_t MonoKeyPressureCommand = 0xD0;
const std::uint8_t PitchBendCommand = 0xE0;
const std::uint8_t SystemCommand = 0xF0;

}



void CRawMidiParser::Parse(const std::vector<std::uint8_t> &RawMidiData)
{
    Parse(RawMidiData, m_MidiHandler);
}


void CRawMidiParser::Parse(const std::vector<std::uint8_t>& RawMidiData, IMidiHandler& MidiHandler)
{
    // append to buffer
    m_RawMidiDataBuffer.insert(m_RawMidiDataBuffer.end(), RawMidiData.begin(), RawMidiData.end());

    if(!m_RawMidiDataBuffer.empty())
    {
        bool KeepOnParsing = true;
        while(KeepOnParsing && !m_RawMidiDataBuffer.empty())
        {
            // parse buffer:
            //  find midi operation in buffer
            //  then parse param1, param2 and handle
            //  then remove operation, param1, param2 from buffer

            std::uint8_t Command = m_RawMidiDataBuffer[0] & 0xF0;
            //int Channel = m_RawMidiDataBuffer[0] & 0x0F;
            int BufferSize = static_cast<int>(m_RawMidiDataBuffer.size());
            int ParsedSize = 0;

            if(Command == NoteOffCommand)
            {
                if(3<=BufferSize)
                {
                    int Note = m_RawMidiDataBuffer[1];
                    int Velocity = m_RawMidiDataBuffer[2];
                    ParsedSize = 3;
                    MidiHandler.OnNoteOff(Note, Velocity, 0);
                }
            }
            else if(Command == NoteOnCommand)
            {
                if(3<=BufferSize)
                {
                    int Note = m_RawMidiDataBuffer[1];
                    int Velocity = m_RawMidiDataBuffer[2];
                    ParsedSize = 3;
                    MidiHandler.OnNoteOn(Note, Velocity, 0);
                    // note: note on with velocity zero to be handled as note off?
                }
            }
            else if(Command == PolyKeyPressureCommand)
            {
                if(3<=BufferSize)
                {
                    ParsedSize = 3;
                    MidiHandler.OnUnknown(0);
                }
            }
            else if(Command == ControlChangeCommand)
            {
                if(3<=BufferSize)
                {
                    int Parameter = m_RawMidiDataBuffer[1];
                    int Value = m_RawMidiDataBuffer[2];
                    ParsedSize = 3;
                    MidiHandler.OnController(Parameter, Value, 0);
                }
            }
            else if(Command == ProgramChangedCommand)
            {
                if(2<=BufferSize)
                {
                    ParsedSize = 2;
                    MidiHandler.OnUnknown(0);
                }
            }
            else if(Command == MonoKeyPressureCommand)
            {
                if(2<=BufferSize)
                {
                    ParsedSize = 2;
                    MidiHandler.OnUnknown(0);
                }
            }
            else if(Command == PitchBendCommand)
            {
                if(3<=BufferSize)
                {
                    int PitchBend = m_RawMidiDataBuffer[1];//????
                    ParsedSize = 3;
                    MidiHandler.OnPitchbend(PitchBend, 0);
                }
            }
            else if(Command == SystemCommand)
            {
                if(3<=BufferSize)
                {
                    ParsedSize = 3;
                    MidiHandler.OnUnknown(0);
                }
            }
            else
            {
                // not a command => ignore this byte
                ParsedSize = 1;
            }

            if(ParsedSize)
            {
                // something is parsed => erase parsed bytes and try parsing the remaining bytes (if any)
                m_RawMidiDataBuffer.erase(m_RawMidiDataBuffer.begin(), m_RawMidiDataBuffer.begin()+ParsedSize);
            }
            else
            {
                // nothing is parsed => incomplete => stop parsing
                KeepOnParsing = false;
            }
        }
    }
}
