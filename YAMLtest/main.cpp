#include <iostream>
#include "Config_yaml.h"

int main() {
    Config_yaml config;//("Config_Serveur.yml");
    
    if (!config.LireFichierYaml()) {
        std::cerr << "Failed to load config file" << "\n";
        return 1;
    }

    try {
        int tcpPort = config.getInt("config_serveur", "serveurTCP_port");
        std::string restAddr = config.getString("config_serveur.ServeurRest", "addrServeurRest");
        int restPort = config.getInt("config_serveur.ServeurRest", "PortServeurRest");

        std::cout << "Configuration loaded:" << std::endl;
        std::cout << "TCP Port: " << tcpPort << std::endl;
        std::cout << "REST Address: " << restAddr << std::endl;
        std::cout << "REST Port: " << restPort << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}