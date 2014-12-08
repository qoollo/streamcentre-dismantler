#ifndef VSM_CONFIGREADER_H
#define VSM_CONFIGREADER_H

#include <common/VSCommon.h>
#include "Configs.h"

namespace vseg {

class ConfigReader {
public:
    up<Configs> readConfigs(std::string const& fileName) const;
};

}

#endif // VSM_CONFIGREADER_H
