#include "mbed.h"
#include "USBMIDI.h"

//USBMIDI object
USBMIDI midi;

void show_message(MIDIMessage msg)
{
    switch (msg.type())
    {
        case MIDIMessage::NoteOnType:
            std::printf("Midi Ch %d note on %d vel %d \r\n", msg.channel(), msg.key(), msg.velocity());
            break;
        case MIDIMessage::NoteOffType:
            std::printf("Midi Ch %d note on %d vel %d \r\n", msg.channel(), msg.key(), msg.velocity());
            break;
        case MIDIMessage::ControlChangeType:
            std::printf("Midi Ch %d controlchange %d val %d \r\n", msg.channel(), msg.controller(), msg.value());
            break;
        case MIDIMessage::PitchWheelType:
            std::printf("Midi Ch %d pitch wheel %d \r\n", msg.channel(), msg.pitch());
            break;
        default:
            std::printf("Midi Ch %d other \r\n", msg.channel());
    }
}

int main()
{
    std::printf("USB midi hello world... \r\n");

    // call back for messages received
    midi.attach(show_message);

    while(1)
    {
        wait(2.0f);
        std::printf("Hello \r\n");
    }
}
