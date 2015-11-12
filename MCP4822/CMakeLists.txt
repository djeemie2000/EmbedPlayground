cmake_minimum_required (VERSION 2.6)

project (MCP4822)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(MCP4822
    MCP4822.cpp
    MCP4822.h
)
