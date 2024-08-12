#include <iostream>
#include <memory.h>
#include <fstream>

#include "XModemSender.h"
#include "TransmissionUart.h"

#define SENDER_PORT "/dev/ttyV1" // UART device file

class FileReader {
public:
    // Method to read from a file and store the content in a vector of unsigned char
    std::vector<unsigned char> readFromFile(const std::string& filename) {
        std::ifstream infile(filename, std::ios::binary | std::ios::ate); // Open file in binary mode and set the file pointer at the end
        std::vector<unsigned char> buffer;

        if (!infile) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return buffer;
        }

        std::streamsize fileSize = infile.tellg();  // Get file size
        infile.seekg(0, std::ios::beg);  // Set file pointer back to the beginning

        buffer.resize(fileSize);  // Resize vector to fit the file content
        if (!infile.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
            std::cerr << "Error: Could not read file " << filename << std::endl;
        }

        infile.close();
        return buffer;
    }
};

int main(int argc, char* argv[]) {
    // Check if the filename is provided as an argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <inputFilename>" << std::endl;
        return 1;
    }

    std::cout << "Starting Sender..." << std::endl;
    std::string inputFilename = argv[1];

    FileReader fileReader;
    XModemByteArray data = fileReader.readFromFile(inputFilename);

    if(data.empty()) {
        std::cerr << "The provided file contains no data. " << inputFilename << std::endl;    
        return 1;
    }

    std::cout << "Data successfully read from " << inputFilename << std::endl;

    std::shared_ptr<TransmissionUartSender> commsPtr = std::make_shared<TransmissionUartSender>();
    commsPtr->StartSender(SENDER_PORT);
    XModemSender sender(commsPtr);
    sender.SendBuffer(data);

    commsPtr->StopSender();
    
    std::cout << "Sender EXIT" << std::endl;
    return 0;
}
