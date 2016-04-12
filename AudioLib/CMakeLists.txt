cmake_minimum_required (VERSION 2.6)

project (AudioLib)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(AudioLib
    AnalogOutRenderer.h
    RenderManager.h
    Dummy.cpp
)

