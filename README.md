# UARTxMODEM

## Overview

This project intends to send a file from process Sender to process Receiver

As a transportation layer the method used to send data is via UART.
In order to control data integrity during communication it used XModem protocol.

## Getting Started

### Prerequisites

- C++ Compiler (e.g., g++, clang++)
- CMake (for build automation)
- Google Test (for unit testing)
- Google Mocks (for unit testing)
- socat (it is optional for testing UART by creating virtual ports)

### Project step by step

1. **Build Project**

   ```console
      mkdir build
      cd build
      cmake ..
      make
    ```

2. **Create Virtual Ports**

   ```console
      ./build/createVirtualSerialPorts.sh
   ```
   
   Follow the instructions and confirm which ports have been created by checking logs/socat.log

3. **Run Sender**

   ```console
      ./build/senderUartXModem inputFile.txt
   ```

4. **Run Receiver**

   ```console
   ./build/receiverUartXModem outputFile.txt
   ```

5. **Run Test Cases**

   Running test cases for XModem protocol
   ```console
   ./build/uartxmodemTest
   ```

   Running test cases UART transmission
   ```console
   ./build/transmissionTest
   ```

### TODO's

* Doing a proper Error handling. The retries by sending ACK again are not implemented
* Check the edge cases like when data to be sent is bigger then the package size
* Improve test cases to cover edge scenarios and error handling
* Code optimization, mostly handling buffers
* etc