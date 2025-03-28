#ifndef JSONFILE_H
#define JSONFILE_H
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>


class JSONFile
{
private:
    std::string leJSON;
    //!JSON.
    /*!Chaine de caractère contenant le JSON*/
    std::ofstream busCAN;
    //!Bus CAN
    /*!ofstream permettant de mettre la chaine de caractère JSON dans un fichier JSON*/
public:
    JSONFile(std::string nomUtilisateur,std::string nomFichier);
    /*!
        @brief Constructeur qui permet de Créer un Fichier JSON et de débuter son contenu.
        @param nomUtilisateur Le nom d'utilisateur du fichier json.
        @param nomFichier Le nom du fichier JSON.
    */
    void creerJSON(std::string nomFichier);
    /*!
        @brief Méthode de Créer un Fichier JSON.
        @param nomFichier le nom du fichier JSON à joindre.
    */
    void DebuterJSON(std::string nomUtilisateur);
    /*!
        @brief Méthode permettant de débuter la chaine de caractère leJSON.
        @param nomUtilisateur le nom d'utilisateur du fichier json.
    */
    void AjouterDonneesJSON(std::string idCAN, std::string lenData, std::string Data);
    void AjouterDonneesJSON(unsigned int idCAN, int lenData, unsigned char * Data);
    /*!
        @brief Méthode permettant d'ajouter les données dans le fichier JSON.
        @param idCAN entier non-signé qui permet de savoir l'idantifiant CAN du véhicule.
        @param lenDAta entier permettant de savoir la taille des données.
        @param Data caractère non-signé permettant d'inclure les données.
    */
    void CloreJSON();
    /*!
        @brief Méthode permettant de finir la chaine de caractère le JSON et de fermer le fichier JSON(Bus CAN).
    */
    std::string getJSON();
    /*!
        @return retourne la chaine de caractère le JSON.
    */
};
#endif
