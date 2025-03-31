#include "YamlConfigParser.h"
#include <iostream>

int main() {
    YamlConfigParser parser("config_serveur1.yml");
    
    parser.printConfig();

    std::cout << "\nPort TCP: " << parser.getIntValue("config_serveur", "serveurTCP_port") << "\n";
    std::cout << "Adresse Serveur REST: " << parser.getValue("config_serveur", "ServeurRest.addrServeurRest") << "\n";
    std::cout << "Port Serveur REST: " << parser.getIntValue("config_serveur", "ServeurRest.PortServeurRest") << "\n";

    return 0;
}