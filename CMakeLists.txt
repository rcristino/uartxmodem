# CMake minimum required version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Qmi6 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Google Test
find_package(GTest REQUIRED)
   
# Define the source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/XModem.cpp
    ${PROJECT_SOURCE_DIR}/src/XModemReceiver.cpp
    ${PROJECT_SOURCE_DIR}/src/XModemSender.cpp
    ${PROJECT_SOURCE_DIR}/src/TransmissionPrintOut.cpp
    ${PROJECT_SOURCE_DIR}/src/TransmissionUart.cpp
)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Add library (STATIC)
add_library(XModemLib STATIC ${SOURCES})

# Add the executables
add_executable(receiverUartXModem src/Receiver.cpp)
add_executable(senderUartXModem src/Sender.cpp)

# Link the library with the executable
target_link_libraries(receiverUartXModem
    PUBLIC 
        XModemLib
)
target_link_libraries(senderUartXModem
    PUBLIC 
        XModemLib
)

add_executable(uartxmodemTest ${SOURCES}
    tests/UartXModemTest.cpp
    tests/MockTransmission.h
)

target_link_libraries(uartxmodemTest
    PRIVATE
        GTest::gtest_main 
        GTest::gtest
        GTest::gmock
)

add_executable(transmissionTest ${SOURCES}
    tests/TransmissionTest.cpp
)

target_link_libraries(transmissionTest
    PRIVATE
        GTest::gtest_main 
        GTest::gtest
        GTest::gmock
)