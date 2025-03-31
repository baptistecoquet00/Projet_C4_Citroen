#ifndef YAML_CONFIG_PARSER_H
#define YAML_CONFIG_PARSER_H

#include <string>
#include <map>

class YamlConfigParser {
private:
    std::map<std::string, std::map<std::string, std::string>> configData;

    static std::string trim(const std::string& str);
    static std::pair<std::string, std::string> splitKeyValue(const std::string& line);

public:
    explicit YamlConfigParser(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    const std::map<std::string, std::map<std::string, std::string>>& getData() const;
    
    std::string getValue(const std::string& section, const std::string& key, 
                        const std::string& defaultValue = "") const;
    int getIntValue(const std::string& section, const std::string& key, int defaultValue = 0) const;
    
    void printConfig() const;
};

#endif // YAML_CONFIG_PARSER_H