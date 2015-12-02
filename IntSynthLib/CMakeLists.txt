cmake_minimum_required (VERSION 2.6)
project (IntSynthLib)
ADD_DEFINITIONS(
	-std=c++11 # or -std=c++0x
	# other flags
)

add_library(IntSynthLib
    IntCombinedOperator.h
    IntCombinorFactory.h
    IntCombinors.h
    IntConversions.h
    IntDelayLine.h
    IntDeltaSmooth.h
    IntEnvelope.h
    IntFeedbackDelay.h
    IntInterpolator.h
    IntKarplusStrong.h
    IntMultiSaw.h
    IntNoise.h
    IntOnePoleFilter.h
    IntOperatorFactory.h
    IntOperators.h
    IntPhaseGenerator.h
    IntScaledValue.h
    IntSimpleOscillator.h
    IntSkewer2D.h
    IntUtilities.h
    IntBipolarLUTShaper.h
    LUTSaturationTanH.h
    Dummy.cpp
)
