#include "YamlConfigParser.h"
#include <iostream>

int main() {
    std::cout << "Initialisation...\n";
    YamlConfigParser parser("config_serveur1.yml");
    
    std::cout << "\nConfiguration complete:\n";
    parser.printConfig();

    // Appel avec 2 paramètres (utilise la valeur par défaut "")
    std::string addr = parser.getValue("config_serveur", "addrServeurRest");

    // Appel avec 3 paramètres
    std::string ip = parser.getValue("config_serveur", "ip", "127.0.0.1");

    // Conversion en int
    int port = parser.getIntValue("config_serveur", "PortServeurRest");
    
    std::cout << "\nValeurs specifiques:\n";
    std::cout << "Port TCP: " << parser.getIntValue("config_serveur", "serveurTCP_port") << std::endl;
    std::cout << "Adresse REST: " << addr << std::endl;
    std::cout << "Port REST: " <<port << std::endl;
    
    return 0;
}