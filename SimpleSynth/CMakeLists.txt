# ------------------------------------------------------------------------------
# Copyright by Uwe Arzt mailto:mail@uwe-arzt.de, https://uwe-arzt.de
# under BSD License, see https://uwe-arzt.de/bsd-license/
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# set the arm toolchain
set(CMAKE_TOOLCHAIN_FILE ../mbed-cmake/arm_toolchain.cmake)

cmake_minimum_required(VERSION 2.8.1)

project (SimpleSynth C CXX ASM)

# ------------------------------------------------------------------------------
# set the target processor and include settings for cmake

#set(MBED_TARGET LPC1768)
#set(MBED_TARGET LPC11U24)
#set(MBED_TARGET RBLAB_NRF51822)
set(MBED_TARGET NUCLEO_F411RE)

# ------------------------------------------------------------------------------
# will add the include dirs and link the lib
#set(USE_RTOS true)
#set(USE_NET true)
set(USE_USB true)
set(USE_DSP true)

# ------------------------------------------------------------------------------
# for installing an serial console
set(MBEDMOUNT /media/pj/NODE_F411RE)
set(SERCON /dev/ttyACM0)


# ------------------------------------------------------------------------------
# build the executable
set(BLINKLED_SRC
  main.cpp
  MidiNoteFrequencies.cpp
  MidiNoteFrequencies.h
  SimpleOscillatorController.cpp
  SimpleOscillatorController.h
  CombNoiseController.cpp
  CombNoiseController.h
  IntSimpleOscillatorController.cpp
  IntSimpleOscillatorController.h
  KarplusStrongController.cpp
  KarplusStrongController.h

)

set(BIN
  SimpleSynth
)

include(../mbed-cmake/arm_mbed.cmake)

# ------------------------------------------------------------------------------
# build the executable

include_directories(./../MCP4822 ./../SynthLib ./../IntSynthLib ./../Utilities ./../RotaryEncoder)

add_subdirectory(../MCP4822 MCP4822)
add_subdirectory(../SynthLib SynthLib)
add_subdirectory(../IntSynthLib IntSynthLib)
add_subdirectory(../Utilities Utilities)
add_subdirectory(../RotaryEncoder RotaryEncoder)


add_executable(${BIN}
                ${BLINKLED_SRC}
                ${MBED_OBJECTS}
                ${MBED_HEADERS}
)

target_link_libraries(${BIN}
                        RotaryEncoder
                        MCP4822
                        ${MBED_LIBS})

