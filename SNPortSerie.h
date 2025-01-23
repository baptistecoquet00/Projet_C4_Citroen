#ifndef SNPortSerie_H
#define SNPortSerie_H

// Avec l'aide de : https://qt.gitorious.org/qt/qtserialport/source/27966cb75d8a95c4ada1fbd2f5ee19cf66199842:src/serialport/qserialport_unix.cpp#LNaN-NaN



#include <string.h>
#include <string>
using namespace std;


//!  Classe de communication avec l'afficheur 
/*!
  Elle permet la communication par le port série avec l'afficheur.
  Le nom du port série utilisé pour la communication doit être indiqué par la méthode ouvrir().
  Ensuite, les méthodes envoyerCaractere() et envoyerTrame() permettent l'envoi de données vers l'afficheur.
  La méthode lireMessage() permet de lire les messages provenant de l'afficheur. Il n'y a que 2 messages possibles : ACK ou NACK.
  Pour terminer correctement la communication, la méthode fermer() doit être utilisée.
*/

typedef enum {
	V0_BAUD,
	V50_BAUD,
	V75_BAUD,
	V110_BAUD,
	V134_BAUD,
	V150_BAUD,
	V200_BAUD,
	V300_BAUD,
	V600_BAUD,
	V1200_BAUD,
	V1800_BAUD,
	V2400_BAUD,
	V4800_BAUD,
	V9600_BAUD,
	V19200_BAUD,
	V38400_BAUD,
	V57600_BAUD,
	V115200_BAUD,
	V230400_BAUD,
	V460800_BAUD,
	V500000_BAUD,
	V576000_BAUD,
	V921600_BAUD,
	V1000000_BAUD,
	V1152000_BAUD,
	V1500000_BAUD,
	V2000000_BAUD,
	V2500000_BAUD,
	V3000000_BAUD,
	V3500000_BAUD,
	V3710000_BAUD,
	V4000000_BAUD	
}SNPortSerieVitesse;

typedef enum {
	D5_BITS,
	D6_BITS,
	D7_BITS,
	D8_BITS
}SNPortSerieLongueurDonnees;

typedef enum {
	S1_BIT,
	S2_BITS
}SNPortSerieBitStop;

typedef enum {
	PARITE_PAIRE,
	PARITE_IMPAIRE,
	PAS_DE_PARITE
}SNPortSerieParite;

class SNPortSerie
{		
public:

    //! Le constructeur.
    /*!
      Le constructeur fourni des valeurs par défaut aux attributs de la classe. Par défaut, le port est fermé, le descripteur de fichier vaut -1 et le nom du port est une chaine de caractère vide.
      \sa ouvrir(), ~SNPortSerie()
    */
    SNPortSerie();

    //! Le destructeur.
    /*!
      Le destructeur ferme le port série s'il est encore ouvert.
      \sa fermer(), SNPortSerie()
    */
    ~SNPortSerie();
    
    //! Ouverture du port série.
    /*!
      Cette méthode ouvre la communication sur le port série dont le nom est passé en argument. Les paramètres de la communication avec l'afficheur sont : 9600 baud - 8N1 (8 bits de données, pas de parité et 1 bit stop).
      Si l'ouverture du port série spécifié est impossible (la méthode ouvrir() a déjà été appelée avec succès et la méthode fermer() n'a pas été appelée, le port série a été ouvert par quelqu'un d'autre, les droits de l'utilisateur sont insuffisants...) cette méthode renvoie	une valeur négative. Sinon, elle renvoie 0.
      \param nomDuPortSerie une chaîne de caractères contenant le nom du port série à ouvrir.
      \return Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.
      \sa fermer()
    */
    int Ouvrir(const char * nomDuPortSerie);

	bool ModifierVitesse(SNPortSerieVitesse vitesse);

	bool ModifierLongueurDonnees(SNPortSerieLongueurDonnees longueurDonnees);

	bool ModifierNombreBitStop(SNPortSerieBitStop bitStop);

	bool ModifierParite(SNPortSerieParite parite);

    //! Fermeture du port série.
    /*!
      Cette méthode ferme le port série précédemment ouvert. Si le port série n'était pas ouvert ou que la fermeture du port série provoque une erreur, cette méthode renvoie une valeur négative. Sinon, elle renvoie 0.
      \return Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.
      \sa ouvrir()
    */
    int Fermer();
	int fermer();


	//! Emission d'une chaîne de caractères sur le port série.
	/*!
	  Cette méthode envoie la chaîne de caractères 'trame' passé en argument sur le port série. Le port série doit être préalablement ouvert, sinon la méthode renvoie false. De plus, si le nombre d'octets réellement envoyé est différent de la taille de la chaîne de caractères 'trame', cette méthode renvoie false.
	  \param trame la chaîne de caractère à émettre.
	  \return Cette méthode renvoie true en cas de succès, false sinon.
	  \sa envoyerCaractere(), lireMessage()
	*/
	bool EnvoyerDesOctets(const char trame[], int nbOctets) const;
	bool Envoyer(unsigned char unOctet);
	bool envoyer(unsigned char unOctet);


	//! Lecture d'un message sur le port série.
	/*!
	  Cette méthode lit un message sur le port série et le stocke dans la chaîne de caractère 'message'. Le port série doit être préalablement ouvert, sinon la méthode renvoie une valeur négative.
	  \param message la chaîne de caractère qui contiendra le message à lire.
	  \param tailleMaximaleDuMessage la taille maximale du message à recevoir (correspond généralement au nombre de caractères de la chaîne de caractères de recevant ce message)
	  \return Cette méthode renvoie le nombre de caractères lus en cas de succès, une valeur négative sinon.
	  \sa envoyerTrame(), lireMessage()
	*/
	int LireDesOctets(char message[], int tailleMaximaleDuMessage) const;
	int Recevoir(unsigned char * unOctet);
	int recevoir(unsigned char * unOctet);


    void ModifierRequestToSend(bool value);
    void ModifierDataTerminalReady(bool value);

    
private:
    //! Le nom du port série.
    /*! Cette chaîne de caractères contient le nom du port série. Le constructeur initialise cette chaîne à "". Le port série virtuel utilisé se nommera souvent "/dev/ttyUSB0". C'est l'utilisateur qui fixe le nom de ce port par la méthode ouvrir().*/
    string m_nomDuPortSerie;
    //! Le descripteur de fichier.
    /*! Le port série étant vu comme un fichier, ouvrir la communication sur le port série revient à ouvrir un fichier. Si l'ouverture est réussie, l'OS nous fournit le descripteur de ce fichier (une valeur entière) qui doit être utilisée par la suite pour toutes les opérations d'écriture, de lecture ou même de fermeture de ce fichier.*/
    int m_descripteurFichier;
    //! Booléen indiquant si le port série est ouvert.
    /*! Lors de la construction, ce booléen est placé à false. Si l'ouverture du fichier est réussie, il passe alors à true. Il vaudra à nouveau false après la fermeture.*/
    bool m_estOuvert;

};
#endif
