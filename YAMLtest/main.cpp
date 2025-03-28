#include "Config_yaml.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        // 1. Créer et charger le fichier YAML
        Config_yaml config;
        if (!config.LireFichierYaml()) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier YAML." << std::endl;
            return 1;
        }

        // 2. Accéder aux valeurs (exemples)
        std::string addrRest = config.getString("config_server.addrServerRest");
        int portTCP = config.getInt("config_server.serverTCP_port");
        int portRest = config.getInt("config_server.PortServerRest");

        // 3. Afficher les résultats
        std::cout << "Configuration chargée :\n"
                  << "- Addresse REST : " << addrRest << "\n"
                  << "- Port TCP : " << portTCP << "\n"
                  << "- Port REST : " << portRest << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}