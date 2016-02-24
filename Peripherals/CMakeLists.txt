cmake_minimum_required (VERSION 2.6)

project (Peripherals)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(Peripherals
    MCP4822.cpp
    MCP4822.h
    PCF8591.cpp
    PCF8591.h
)
