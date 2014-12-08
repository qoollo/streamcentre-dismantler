#include "FileHelper.h"
#include <fstream>

namespace vseg {

std::vector<char> FileHelper::readAllByes(const std::string &fileName) {
    std::ifstream ifs(fileName, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return std::move(result);
}

void FileHelper::writeAllBytes(const std::string &fileName, const std::vector<char> &data) {
    std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
    ofs.write(data.data(), data.size());
}

}
