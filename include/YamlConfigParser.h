#ifndef YAML_CONFIG_PARSER_H
#define YAML_CONFIG_PARSER_H

#include <string>
#include <map>
#include <vector>

class YamlConfigParser {
private:
    std::map<std::string, std::map<std::string, std::string>> configData;
    
    void parseLine(std::string line, std::string& currentSection, int& currentIndent);
    std::string trim(const std::string& str);
    int countIndentation(const std::string& line);

public:
    YamlConfigParser(const std::string& filename);
    
    std::string getValue(const std::string& section, 
        const std::string& key,
        const std::string& defaultValue = "") const;

    int getIntValue(const std::string& section, 
        const std::string& key,
        int defaultValue = 0) const;
    
    void printConfig() const;
};
#endif