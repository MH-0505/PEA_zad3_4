
#ifndef PEA_ZAD1_CONFIGMANAGER_H
#define PEA_ZAD1_CONFIGMANAGER_H


#include <vector>
#include <map>

class ConfigManager {
public:
    static std::map<std::string, std::string> load_configuration(const std::string& filename);
    static std::vector<std::string> parse_filenames(std::map<std::string, std::string>& configuration);
};


#endif //PEA_ZAD1_CONFIGMANAGER_H
