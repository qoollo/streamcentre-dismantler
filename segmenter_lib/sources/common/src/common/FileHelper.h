#ifndef COMMON_FILEHELPER_H
#define COMMON_FILEHELPER_H

#include "VSCommon.h"
#include <vector>

namespace vseg {

class FileHelper {
public:
    static std::vector<char> readAllByes(const std::string &fileName);
    static void writeAllBytes(const std::string &fileName, const std::vector<char> &data);
};

}

#endif // COMMON_FILEHELPER_H
