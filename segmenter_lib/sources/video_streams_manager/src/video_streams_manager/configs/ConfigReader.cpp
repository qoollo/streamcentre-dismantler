#include "ConfigReader.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace vseg {

up<Configs> ConfigReader::readConfigs(std::string const& fileName) const {
    using namespace boost::property_tree;

    ptree ptree;
    json_parser::read_json(fileName, ptree);

    ContainerFormat cfmt = StringHelper::fromString<ContainerFormat>(
            ptree.get<std::string>("segmenter_params.container_format"));

    double durationSec = ptree.get<double>("segmenter_params.fragment_duration_sec");

    SegmenterParameters segmenterParameters = SegmenterParameters(durationSec, cfmt);

    std::string storageUrl = ptree.get<std::string>("storage_url");

    return up<Configs>(new Configs(segmenterParameters, storageUrl));
}

}
