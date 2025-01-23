#include "SNPortSerie.h"

#ifdef __unix__
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#elif _WIN32 || _WIN64

#endif

#include <iostream>
using namespace std;

/********************************************************************************
*	SOUS-PROGRAMME SNPortSerie DE LA CLASSE SNPortSerie	*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE :						*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Le constructeur fourni des valeurs par défaut au attributs de la 	*
* 	classe. Par défaut, le port est fermé, le descripteur de fichier vaut 	*
* 	-1 et le nom du port est une chaine de caractère vide.			*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
********************************************************************************/
SNPortSerie::SNPortSerie()
{
    // Valeur par défaut des attributs
    m_estOuvert = false;
    m_nomDuPortSerie =  "";
    m_descripteurFichier = -1;
}


/********************************************************************************
*	SOUS-PROGRAMME ~SNPortSerie DE LA CLASSE SNPortSerie	*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE :						*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Le destructeur doit vérifier que le port série est bien fermé avant que	*
* 	l'objet soit détruit. S'il n'est pas fermé, il le ferme.		*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
********************************************************************************/
SNPortSerie::~SNPortSerie()
{
    if(m_estOuvert)
        this->Fermer();
}

/********************************************************************************
*	SOUS-PROGRAMME ouvrir DE LA CLASSE SNPortSerie			*
*		DONNEES EN ENTREE : nomDuPortSerie[] : CARACTERE		*
* 		DONNEES EN SORTIE : erreur : ENTIER				*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Cette méthode doit ouvrir le port série dont le nom est passé en 	*
* 	argument : 'nomDuPortSerie'. Si cet objet a déjà ouvert un port série	*
* 	ou que l'ouverture du port série spécifié est impossible (port ouvert	*
* 	par quelqu'un d'autre, droits insuffisants...) cette méthode retourne	*
* 	une valeur négative. Sinon, elle renvoie 0.				*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
*	Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.	*
********************************************************************************/
int SNPortSerie::Ouvrir(const char * nomDuPortSerie)
{  
    // Copie du nom dans l'attribut m_nomDuPortSerie si le nom n'est pas trop long
    if(strlen(nomDuPortSerie) > 50) return -1;
    m_nomDuPortSerie = string(nomDuPortSerie);

    // On vérifie qu'un port de comunication série n'est pas déjà ouvert...
    if(m_estOuvert) return -2;

	// On ouvre le port de communication série demandé
	#ifdef __unix__
	//m_descripteurFichier = open(nomDuPortSerie.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	//m_descripteurFichier = open(nomDuPortSerie.c_str(), O_RDWR | O_NDELAY);
	
	//m_descripteurFichier = ::open(nomDuPortSerie.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY );
	m_descripteurFichier = open(nomDuPortSerie, O_RDWR );
	#elif _WIN32 || _WIN64

	#endif

	if (m_descripteurFichier == -1) return -3;
	m_estOuvert = true;
	
	
	// La lecture sera bloquante
	//fcntl(m_descripteurFichier, F_SETFL, 0);	// Lecture bloquante
	//::fcntl(m_descripteurFichier, F_SETFL, FNDELAY);
	

	#ifdef __unix__
	// Déclaration d'une structure contenant les paramètres du port série
    //struct termios optionsPrecedentes;
    struct termios options;
    
    // On récupère les paramètres actuels du port série
    /*if( ::tcgetattr(m_descripteurFichier, &options) == -1)
    {
    	cout << "Impossible de récupérer les paramètres termios." << endl;
    	return -4;
    }*/
    
    #ifdef TIOCEXCL
    // A faire : chercher l'explication...
    //::ioctl(m_descripteurFichier, TIOCEXCL);
	#endif
 

    // Raw input
	/* cfmakeraw() sets the terminal to something like the "raw" mode of the old Version 7 terminal driver: input is available character by character, echoing is disabled, and all special processing of terminal input and output characters is disabled. The terminal attributes are set as follows:
		termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
				| INLCR | IGNCR | ICRNL | IXON);
		termios_p->c_oflag &= ~OPOST;
		termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		termios_p->c_cflag &= ~(CSIZE | PARENB);
		termios_p->c_cflag |= CS8; 
	*/
    ::cfmakeraw(&options);
	
	options.c_cflag |= (CREAD | CLOCAL);
	// Paramètre dans le cas d'une lecture non bloquante du port série.
    options.c_cc[VTIME] = 0;	//	Timeout de 0 dizième de second
    options.c_cc[VMIN] = 0;		//	Nombre de caractères minimal à attendre : 0
 
    
    
    // On impose une vitesse à 2400 baud en lecture et en écriture
    cfsetispeed(&options, B2400);
    cfsetospeed(&options, B2400);
    // On impose 8N1  : 8 bits de données, pas de bit de parité, 1 bit stop.
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    // On modifie les paramètres du port série avec nos nouvelles valeurs
	if( ::tcsetattr(m_descripteurFichier, TCSANOW, &options) == -1)
	{
		cout << "Impossible de modifier les paramètres termios." << endl;
		return -5;
	}
	
	#elif _WIN32 || _WIN64

	#endif
	
	//ModifierVitesse(V9600_BAUD);
	//ModifierLongueurDonnees(D8_BITS);
	//ModifierNombreBitStop(S1_BIT);
	//ModifierParite(PAS_DE_PARITE);
	
    return 0;
}

bool SNPortSerie::ModifierVitesse(SNPortSerieVitesse vitesse)
{
	if(!m_estOuvert) return false;

	speed_t speed;
	switch(vitesse)
	{
		case V0_BAUD : speed=B0;break;
		case V50_BAUD : speed=B50;break;
		case V75_BAUD : speed=B75;break;
		case V110_BAUD : speed=B110;break;
		case V134_BAUD : speed=B134;break;
		case V150_BAUD : speed=B150;break;
		case V200_BAUD : speed=B200;break;
		case V300_BAUD : speed=B300;break;
		case V600_BAUD : speed=B600;break;
		case V1200_BAUD : speed=B1200;break;
		case V1800_BAUD : speed=B1800;break;
		case V2400_BAUD : speed=B2400;break;
		case V4800_BAUD : speed=B4800;break;
		case V9600_BAUD : speed=B9600;break;
		case V19200_BAUD : speed=B19200;break;
		case V38400_BAUD : speed=B38400;break;
		case V57600_BAUD : speed=B57600;break;
		case V115200_BAUD : speed=B115200;break;
		case V230400_BAUD : speed=B230400;break;
		case V460800_BAUD : speed=B460800;break;
		case V500000_BAUD : speed=B500000;break;
		case V576000_BAUD : speed=B576000;break;
		case V921600_BAUD : speed=B921600;break;
		case V1000000_BAUD : speed=B1000000;break;
		case V1152000_BAUD : speed=B1152000;break;
		case V1500000_BAUD : speed=B1500000;break;
		case V2000000_BAUD : speed=B2000000;break;
		case V2500000_BAUD : speed=B2500000;break;
		case V3000000_BAUD : speed=B3000000;break;
		case V3500000_BAUD : speed=B3500000;break;
#ifdef B3710000
		case V3710000_BAUD : speed=B3710000;break;
#endif
		case V4000000_BAUD : speed=B4000000;break;
		default : speed = B9600; break;
	}

	struct termios options;
	if( ::tcgetattr(m_descripteurFichier, &options) == -1)
    {
    	cout << "Impossible de récupérer les paramètres du port série." << endl;
    	return false;
    }

	// On modifie la valeur de la vitesse en lecture et en écriture
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);

    // On modifie les paramètres du port série avec la structure termios modifiée
	if( ::tcsetattr(m_descripteurFichier, TCSANOW, &options) == -1)
	{
		cout << "Impossible de modifier les paramètres termios." << endl;
		return false;
	}

	return true;
}

bool SNPortSerie::ModifierLongueurDonnees(SNPortSerieLongueurDonnees longueurDonnees)
{
	if(!m_estOuvert) return false;

	tcflag_t monCflag;
	switch(longueurDonnees)
	{
		case D5_BITS : monCflag = CS5; break;
		case D6_BITS : monCflag = CS6; break;
		case D7_BITS : monCflag = CS7; break;
		case D8_BITS : monCflag = CS8; break;
		default : monCflag = CS8;
	}

	struct termios options;
	if( ::tcgetattr(m_descripteurFichier, &options) == -1)
    {
    	cout << "Impossible de récupérer les paramètres du port série." << endl;
    	return false;
    }

	// On modifie la valeur de la vitesse en lecture et en écriture
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= monCflag;

    // On modifie les paramètres du port série avec la structure termios modifiée
	if( ::tcsetattr(m_descripteurFichier, TCSANOW, &options) == -1)
	{
		cout << "Impossible de modifier les paramètres termios." << endl;
		return false;
	}

	return true;
}

bool SNPortSerie::ModifierNombreBitStop(SNPortSerieBitStop bitStop)
{
	if(!m_estOuvert) return false;

	struct termios options;
	if( ::tcgetattr(m_descripteurFichier, &options) == -1)
    {
    	cout << "Impossible de récupérer les paramètres du port série." << endl;
    	return false;
    }

	switch(bitStop)
	{
		case S1_BIT : options.c_cflag &= ~CSTOPB; break;
		case S2_BITS : options.c_cflag |= CSTOPB; break;
		default : options.c_cflag &= ~CSTOPB;
	}


    // On modifie les paramètres du port série avec la structure termios modifiée
	if( ::tcsetattr(m_descripteurFichier, TCSANOW, &options) == -1)
	{
		cout << "Impossible de modifier les paramètres termios." << endl;
		return false;
	}

	return true;	
}

bool SNPortSerie::ModifierParite(SNPortSerieParite parite)
{
	if(!m_estOuvert) return false;

	struct termios options;
	if( ::tcgetattr(m_descripteurFichier, &options) == -1)
    {
    	cout << "Impossible de récupérer les paramètres du port série." << endl;
    	return false;
    }

	if(parite == PAS_DE_PARITE)
	{
		options.c_cflag &= ~PARENB;
	}
	else if(parite == PARITE_PAIRE)
	{
		options.c_cflag |= PARENB;
		options.c_cflag |= PARODD;
	}
	else
	{
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
	}

    // On modifie les paramètres du port série avec la structure termios modifiée
	if( ::tcsetattr(m_descripteurFichier, TCSANOW, &options) == -1)
	{
		cout << "Impossible de modifier les paramètres termios." << endl;
		return false;
	}

	return true;

}



/********************************************************************************
*	SOUS-PROGRAMME fermer DE LA CLASSE SNPortSerie			*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE : erreur : ENTIER				*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Cette méthode doit fermer le port série précédemment ouvert. Si le port *
* 	série n'était pas ouvert ou que la fermeture du port série provoque une *
* 	erreur, cette méthode renvoie une valeur négative. Sinon, elle renvoie 	*
* 	0.									*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.	*
********************************************************************************/
int SNPortSerie::Fermer()
{
    // Si le port série n'a pas été ouvert, on renvoie -1
    if(m_descripteurFichier == -1)
        return -1;
    // Si la fermeture du port série échoue, on renvoie -2
    #ifdef __unix__
    if(close(m_descripteurFichier) == -1)
        return -2;
    #elif _WIN32 || _WIN64

	#endif
    // Sinon, on indique que la fermeture s'est bien passé en mettant le booléen m_estouvert à false et en renvoyant la valeur 0
    m_estOuvert = false;
    return 0;
}

int SNPortSerie::fermer()
{
	return this->Fermer();
}


/********************************************************************************
*	SOUS-PROGRAMME envoyerCaractere DE LA CLASSE SNPortSerie		*
*		DONNEES EN ENTREE : c : CARACTERE       			*
* 		DONNEES EN SORTIE : resultat : BOOLEEN				*
* 		DONNEES LOCALES : 						*
*********************************************************************************
* 	DESCRIPTION :								*
* 	Cette méthode doit envoyer sur le port série le caractère 'c' passé en  *
*       argument. Le port série doit être préalalement ouvert, sinon 'resultat' *
*       vaut 'FAUX'.                                                            *
* 	Si le nombre d'octet réellement envoyé est différent de 1, 'resultat'   *
*       vaut 'FAUX'.                						*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie FAUX en cas d'erreur, VRAI sinon.                 *
********************************************************************************/
/*bool SNPortSerie::envoyerCaractere(const char c) const
{
    // Si le port série n'est pas ouvert, renvoie de 'FAUX'
    if (!m_estOuvert)
        return false;
    // Écriture du caractère 'c' sur le port série. On récupère le nombre d'octet vraiment écrit dans nbOctet.
    int nbOctet;
    #ifdef __unix__
    nbOctet = write(m_descripteurFichier, &c, 1);
    #elif _WIN32 || _WIN64

	#endif
    // Si nombre d'octet vraiment écrit est différent de 1, renvoie de la valeur 'FAUX', 'VRAI sinon.
    if(nbOctet != 1)
        return false;
    return true;
}*/


/********************************************************************************
*	SOUS-PROGRAMME envoyerTrame DE LA CLASSE SNPortSerie		*
*		DONNEES EN ENTREE : trame[] : CARACTERE       			*
* 		DONNEES EN SORTIE : resultat : BOOLEEN				*
* 		DONNEES LOCALES : 						*
*********************************************************************************
* 	DESCRIPTION :								*
* 	Cette méthode doit envoyer sur le port série la chaîne de caractères    *
*       'trame' passé en argument. Le port série doit être préalalement ouvert  *
*       sinon 'resultat' vaut 'FAUX'.                                           *
* 	Si le nombre d'octet réellement envoyé est différent de la longueur de  *
*       la trame à envoyer, 'resultat' vaut 'FAUX'.                             *                						*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie FAUX en cas d'erreur, VRAI sinon.                 *
********************************************************************************/
bool SNPortSerie::EnvoyerDesOctets(const char trame[], int nbOctets) const
{
	//int tailleDeLaTrame = strlen(trame);
	int nbOctetsLus;
	
	if (!m_estOuvert)
		return false;
	#ifdef __unix__
	nbOctetsLus = write(m_descripteurFichier, trame, nbOctets);
	//usleep(100000);
	#elif _WIN32 || _WIN64
	
	#endif
	
	if(nbOctetsLus != nbOctets)
		return false;

	return true;
}

bool SNPortSerie::Envoyer(unsigned char unOctet)
{
	return this->EnvoyerDesOctets((const char *)&unOctet, 1);
}

bool SNPortSerie::envoyer(unsigned char unOctet)
{
	return this->Envoyer(unOctet);
}


// Surcharge de la méthode précédente
/*bool SNPortSerie::envoyerTrame(string trame) const
{
	int tailleDeLaTrame = trame.length();
	int nbOctet;
	
	if (!m_estOuvert)
		return false;
	#ifdef __unix__
	nbOctet = write(m_descripteurFichier, trame.c_str(), tailleDeLaTrame);
	usleep(100000);
	#elif _WIN32 || _WIN64
	
	#endif

	if(nbOctet != tailleDeLaTrame)
		return false;

	return true;
}*/

int SNPortSerie::LireDesOctets(char message[], int tailleMaximaleDuMessage) const
{
	int nbOctet=0;
	//cout << "Attente d'un message" << endl;
	#ifdef __unix__
	nbOctet = read(m_descripteurFichier, message, tailleMaximaleDuMessage);
	#elif _WIN32 || _WIN64
	
	#endif

	//cout << "Message lu" << endl;
	// Rajout du caractère \0 si nécessaire
	//if(message[nbOctet-1] != '\0')
	//	message[nbOctet] = '\0';
	return nbOctet;
}

int SNPortSerie::Recevoir(unsigned char * unOctet)
{
	int nbOctet;
	#ifdef __unix__
	nbOctet = read(m_descripteurFichier, unOctet, 1);
	#elif _WIN32 || _WIN64
	
	#endif
	
	if(nbOctet == 1) return 0;
	return -1;
}

int SNPortSerie::recevoir(unsigned char * unOctet)
{
	return this->Recevoir(unOctet);
}

/*string SNPortSerie::lireMessage() const
{
	char msg[1024];
	int nbOctet=0;
	//cout << "Attente d'un message" << endl;
	#ifdef __unix__
	nbOctet = read(m_descripteurFichier, msg, 1023);
	#elif _WIN32 || _WIN64
	
	#endif

	cout << "Message lu" << endl;
	// Rajout du caractère \0 si nécessaire
	if(msg[nbOctet-1] != '\0')
		msg[nbOctet] = '\0';
	//return nbOctet;
	return string(msg);
}*/

void SNPortSerie::ModifierDataTerminalReady(bool value)
{
	// repris de la classe QSerialPort
    int status = TIOCM_DTR;
    if (::ioctl(m_descripteurFichier, value ? TIOCMBIS : TIOCMBIC, &status) == -1)
    	cout << "Erreur lors le l'écriture du status du port série." << endl;

/*	int status;
	if(ioctl(m_descripteurFichier, TIOCMGET, &status) == -1)
	{
		cout << "Erreur lors de la lecture du status du port série" << endl;
		return;
	}
	if(value)
	{
		status |= TIOCM_DTR;
	}
	else
	{
		status &= TIOCM_DTR;
	}
	if(ioctl(m_descripteurFichier, TIOCMSET, &status) == -1)
	{
		cout << "Erreur lors le l'écriture du status du port série." << endl;
		return;
	}
*/		

/*    int iFlags = TIOCM_DTR;
    if(value)
    {
        // turn on DTR
        ioctl(m_descripteurFichier, TIOCMBIS, &iFlags);
    }
    else
    {
        // turn off DTR
        ioctl(m_descripteurFichier, TIOCMBIC, &iFlags);
    }

    int serial;
    ioctl(m_descripteurFichier, TIOCMGET, &serial);
    if (serial & TIOCM_DTR)
           cout << "TIOCM_DTR is not set" << endl;
        else
            cout << "TIOCM_DTR is set" << endl;
*/
}

void SNPortSerie::ModifierRequestToSend(bool value)
{

    int status = TIOCM_RTS;
    if( ::ioctl(m_descripteurFichier, value ? TIOCMBIS : TIOCMBIC, &status) == -1)
    	cout << "Erreur lors le l'écriture du status du port série." << endl;

/*	int status;
	if(ioctl(m_descripteurFichier, TIOCMGET, &status) == -1)
	{
		cout << "Erreur lors de la lecture du status du port série" << endl;
		return;
	}
	if(value)
	{
		status |= TIOCM_DTR;
	}
	else
	{
		status &= TIOCM_DTR;
	}
	if(ioctl(m_descripteurFichier, TIOCMSET, &status) == -1)
	{
		cout << "Erreur lors le l'écriture du status du port série." << endl;
		return;
	}
*/
/*    int iFlags = TIOCM_RTS;
    if(value)
    {
        // turn on RTS
        ioctl(m_descripteurFichier, TIOCMBIS, &iFlags);
    }
    else
    {
        // turn off RTS
        ioctl(m_descripteurFichier, TIOCMBIC, &iFlags);
    }
    
    int serial;
    ioctl(m_descripteurFichier, TIOCMGET, &serial);
    if (serial & TIOCM_RTS)
           cout << "TIOCM_RTS is not set" << endl;
        else
            cout << "TIOCM_RTS is set" << endl;
*/
}

/*int SNPortSerie::lireMessageMieux(char message[], int tailleMaximaleDuMessage) const
{
    int nbOctetsLus=0;
    int res;
    int tmp = 0;
    do{
      res = read(m_descripteurFichier, message+nbOctetsLus, 1);
      //cout << "Lire - res : " << res << " - nbOctetsLus : " << nbOctetsLus << endl;
      if(res > 0 ) nbOctetsLus += res;
      tmp ++;
      usleep(10);
    } while(tmp<8000);
    // Rajout du caractère \0 si nécessaire
    if(message[nbOctetsLus-1] != '\0')
        message[nbOctetsLus] = '\0';
    return nbOctetsLus;
}


int SNPortSerie::lireMessageACK(char message[], int tailleMaximaleDuMessage) const
{
    int nbOctetsLus=0;
    int nbOctetsALire = 3;
    do
    {
      nbOctetsLus += read(m_descripteurFichier, message + nbOctetsLus, 1);
      if((nbOctetsLus == 1) && (message[0] == 'N'))
      {
	nbOctetsALire = 4;
      }
    }while (nbOctetsLus < nbOctetsALire );
    // Rajout du caractère \0 si nécessaire
    if(message[nbOctetsLus-1] != '\0')
        message[nbOctetsLus] = '\0';
    return nbOctetsLus;
}*/

