cmake_minimum_required (VERSION 2.6)

project (SynthLib)

ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(SynthLib
    PhaseAccumulator.h
    PhaseStep.h
    SelectableOperator.h
    SelectableOperatorFactory.cpp
    SelectableOperatorFactory.h
    Triangle.h
    OnePoleFilter.h
    CombFilter.h
    DelayLine2.h
    DeltaSmooth.h
    PolyKarplusStrong.h
    AREnvelope.h
    ConstNumSamplesGenerator.h
    Dummy.cpp
)

