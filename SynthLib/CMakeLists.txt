cmake_minimum_required (VERSION 2.6)

project (SynthLib)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(Json11
    PhaseAccumulator.h
    PhaseStep.h
    SelectableOperator.h
    SelectableOperatorFactory.cpp
    SelectableOperatorFactory.h
    Triangle.h
)

