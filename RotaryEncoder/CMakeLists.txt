cmake_minimum_required (VERSION 2.6)

project (RotaryEncoder)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(RotaryEncoder
    RotaryEncoder.cpp
    RotaryEncoder.h
)
