#include "MidiNoteFrequencies.h"

int GetMidiNoteFrequencyMilliHz(int MidiNote)
{
    int Nr = MidiNote;
    if(Nr<MinMidiNoteNumber)
    {
        Nr = MinMidiNoteNumber;
    }
    else if(MaxMidiNoteNumber<Nr)
    {
        Nr = MaxMidiNoteNumber;
    }
    return MidiNoteFrequencyMilliHz[Nr];
}
