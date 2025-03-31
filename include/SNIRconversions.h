#ifndef SNIR_H
#define SNIR_H
#include <string>
#include <fstream>
#include <sstream>
#ifdef VCL_H
#include <vcl.h>
#endif  // VCL_H
//refaire à la main ToFloat (mantisse...)
using namespace std;
class SNIR
{	public:
	static unsigned int ChaineHexaVersInt(char ChaineHexa[20],int NbCar);	//ChaineHexaVersInt("FF",2)=255
	static string FloatToString(float nb,int nbdecimales);                  //FloatToString(1.5,2)="1.50"
	static string FloatToStringAvecExp(float nb,int nbdecimales);           //FloatToString(1.5,5)="1.50000E+00"
	static float StringToFloat(string s);                                   //StringToFloat("1.5")=1.5
	static int StringDecToInt(string s);                                    //StringDecToInt("100")=100
	static unsigned int StringHexToUnsignedInt(string s);                   //StringHexToUnsignedInt("100")=256
	static signed int StringHexToSignedInt(string s,int NbOctets);          //StringHexToUnsignedInt("8000",2)=-32768
	static long long StringDecToLongLong(string s);                         //8 octets
	static string DecToString(int nb);                                      //DecToString(-100)="-100";
	static string UnsignedDecToString(unsigned int nb);                     //UnsignedDecToString(100)="100"
	static string HexToString(char nb);                                     //HexToString(10)="0A"
	static string HexToString(short nb);                                    //HexToString(10)="000A"
	static string HexToString(int nb);                                      //HexToString(10)="0000000A"
	static string BinToString(char nb);                                     //BinToString(10)="1010";
	static string BinToString(short nb);
	static string BinToString(int nb);
	static string FileToString(string chemin);
	static int dec2bin(int num);
#ifdef VCL_H
	static UnicodeString ToUnicodeString(string s);
	static string ToString(UnicodeString us);
#endif  // VCL_H
};
#endif  //SNIR_H