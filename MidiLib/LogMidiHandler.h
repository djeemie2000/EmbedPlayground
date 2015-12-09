#pragma once

class CLogMidiHandler
{
public:
    CLogMidiHandler()
    {}

    void OnNoteOff(int Note, int Velocity, int Channel)
    {
        printf("NoteOff %d (%d) - Ch %d \r\n", Note, Velocity, Channel);
    }

    void OnNoteOn(int Note, int Velocity, int Channel)
    {
        printf("NoteOn %d (%d) - Ch %d \r\n", Note, Velocity, Channel);
    }

    void OnControlChange(int Controller, int Value, int Channel)
    {
        printf("ControlChange %d (%d) - Ch %d \r\n", Controller, Value, Channel);
    }
};
