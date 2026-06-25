#include "WriteOut.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>

void WriteBinaryFile(const std::string &path, const std::vector<uint8_t> &data)
{
    std::ofstream outFile(path, std::ios::binary);
    if (!outFile) throw std::runtime_error("Cannot write to " + path);
    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void WriteCArrayFile(const std::string &path, const std::vector<uint8_t> &data)
{
    size_t pos = path.find_last_of("/\\");
    std::string arrayName;
    if (pos != std::string::npos) {
        arrayName = path.substr(pos + 1);
    } else {
        arrayName = path;
    }

    std::replace(arrayName.begin(), arrayName.end(), '.', '_');
    
    std::ofstream outFile(path);
    if (!outFile.is_open()) {
        throw std::runtime_error("Cannot open file for writing.");
    }

    outFile << "constexpr unsigned long long " << arrayName << "_size = " << data.size() << ";\n";

    outFile << "static const unsigned char " << arrayName << "[] = {\n  ";

    const size_t bytesPerLine = 16;
    for (size_t i = 0; i < data.size(); ++i) {
        outFile << "0x" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(data[i]);

        if (i != data.size() - 1) {
            outFile << ", ";
            if ((i + 1) % bytesPerLine == 0) {
                outFile << "\n  ";
            }
        }
    }

    outFile << "\n};\n";


    outFile.close();
}
