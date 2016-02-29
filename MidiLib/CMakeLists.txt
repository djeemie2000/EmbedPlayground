cmake_minimum_required (VERSION 2.6)

project (MidiLib)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(MidiLib
    RawMidiByteParser.h
    LogMidiHandler.h
    MidiNoteFrequencies.cpp
    MidiNoteFrequencies.h
    Dummy.cpp
)

