#include <iostream>
#include <memory.h>
#include <fstream>
#include <algorithm>

#include "XModemReceiver.h"
#include "TransmissionUart.h"

#define RECEIVER_PORT "/dev/ttyV0" // UART device file

class FileWriter {
public:
    // Method to write the content of a vector of unsigned char to a file
    void writeToFile(const std::string& filename, std::vector<unsigned char>& buffer) {
        std::ofstream outfile(filename, std::ios::binary);

        if (!outfile) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        // Remove null values from the vector
        buffer.erase(std::remove(buffer.begin(), buffer.end(), '\0'), buffer.end());

        outfile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        outfile.close();
    }
};

int main(int argc, char* argv[]) {
    // Check if the filename is provided as an argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <outputFilename>" << std::endl;
        return 1;
    }

    std::cout << "Starting Receiver..." << std::endl;
    std::string outputFilename = argv[1];

    std::shared_ptr<TransmissionUartReceiver> commsPtr = std::make_shared<TransmissionUartReceiver>();
    commsPtr->StartReceiver(RECEIVER_PORT);
    XModemReceiver receiver(commsPtr);
    XModemByteArray data = receiver.ReceiveBuffer();

    commsPtr->StopReceiver();

    FileWriter fileWriter;
    fileWriter.writeToFile(outputFilename, data);
    std::cout << "Data successfully written to " << outputFilename << std::endl;
    
    /*
    std::cout << "data: ";
    for (unsigned char value : data) {
        std::cout << value << ":";
    }
    std::cout << std::endl;
    */
    std::cout << "Receiver EXIT" << std::endl;
    return 0;
}
