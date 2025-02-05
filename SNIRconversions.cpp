#include "SNIRconversions.h"
unsigned int SNIR::ChaineHexaVersInt( char ChaineHexa[20], int NbCar)
{   int nombre=0,j;
	char caractere;
	for(j=0;j<NbCar;j++)
	{   caractere=ChaineHexa[j];
		if(caractere>='0' && caractere<='9')
		{  nombre=nombre+((caractere-'0')<<(4*(NbCar-j-1)));

		} else nombre=nombre+((caractere-'A'+10)<<(4*(NbCar-j-1)));
	}
	return nombre;
}
string SNIR::FloatToString(float nb,int nbdecimales)
{   string format="%."+DecToString(nbdecimales)+"f";
	string s; char cnb[50]; sprintf(cnb,format.c_str(),nb); s=cnb; return s;
};
string SNIR::FloatToStringAvecExp(float nb,int nbdecimales)
{   string format="%."+DecToString(nbdecimales)+"E";
	string s; char cnb[50]; sprintf(cnb,format.c_str(),nb); s=cnb; return s;
};
float SNIR::StringToFloat(string s)
{   for(int i=0;i<s.length();i++) if(s[i]==',') s[i]='.';
	float n;
	istringstream(s) >> n;
	return n;
}
int SNIR::StringDecToInt(string s)
{	int n;
	istringstream(s) >> n;
	return n;
}
long long SNIR::StringDecToLongLong(string s)
{	long long n;
	istringstream(s) >> n;
	return n;
}
string SNIR::DecToString(int nb)
{	string s; int seuil=1000000000; bool tous=false;
	if(nb==0) return "0";
	if(nb<0) {s+='-';nb=-nb;}
	for(int i=0;i<10;i++)
	{	if(nb>=seuil || tous) {s+=(char)(nb/seuil+'0');nb=nb-(nb/seuil)*seuil;tous=true;}
		seuil/=10;}
	return s;
};
string SNIR::UnsignedDecToString(unsigned int nb)
{	string s; int seuil=1000000000; bool tous=false;
	if(nb==0) return "0";
	for(int i=0;i<10;i++)
	{	if(nb>=seuil || tous) {s+=(char)(nb/seuil+'0');nb=nb-(nb/seuil)*seuil;tous=true;}
		seuil/=10;}
	return s;
};
unsigned int SNIR::StringHexToUnsignedInt(string s)
{	unsigned int n;
	n=ChaineHexaVersInt((char*)s.c_str(),s.length());
	return n;
}
signed int SNIR::StringHexToSignedInt(string s,int NbOctets)
{   unsigned int n; unsigned int masque=0xffffffff;
	if(NbOctets<4) masque=0xffffff;
	if(NbOctets<3) masque=0xffff;
	if(NbOctets<2) masque=0xff;
	n=ChaineHexaVersInt((char*)s.c_str(),s.length());
	if((n & (0x80<<(NbOctets-1)*8))!=0)
	{	n=((~n)+1)&(masque);
		return -(int)n;
	}
	return n;
}
string SNIR::HexToString(char nb)
{	string s="";
	if(nb==0) return "00";
	for(int i=0;i<2;i++)
	{	char chiffre=(nb>>(4*(1-i)))&0xf;
		if(chiffre<10)s+=(char)(chiffre+'0');
		else s+=(char)(chiffre+'A'-10);
	}
	return s;
}

string SNIR::HexToString(short nb)
{	string s;
	if(nb==0) return "0000";
	for(int i=0;i<4;i++)
	{	char chiffre=(nb>>(4*(3-i)))&0xf;
		if(chiffre<10)s+=(char)(chiffre+'0');
		else s+=(char)(chiffre+'A'-10);
	}
	return s;
};
string SNIR::HexToString(int nb)
{	string s;
	if(nb==0) return "00000000";
	for(int i=0;i<8;i++)
	{	char chiffre=(nb>>(4*(7-i)))&0xf;
		if(chiffre<10)s+=(char)(chiffre+'0');
		else s+=(char)(chiffre+'A'-10);
	}
	return s;
};
string SNIR::BinToString(char nb)
{	string s;
	if(nb==0) return "00000000";
	for(int i=0;i<8;i++)
	{	char bit=(nb>>(7-i))&0x1;
		if(bit)s+='1';
		else s+='0';
	}
	return s;
};
string SNIR::BinToString(short nb)
{	string s;
	if(nb==0) return "0000000000000000";
	for(int i=0;i<16;i++)
	{	char bit=(nb>>(15-i))&0x1;
		if(bit)s+='1';
		else s+='0';
	}
	return s;
};
string SNIR::BinToString(int nb)
{	string s;
	if(nb==0) return "00000000000000000000000000000000";
	for(int i=0;i<32;i++)
	{	char bit=(nb>>(31-i))&0x1;
		if(bit)s+='1';
		else s+='0';
	}
	return s;
};

int SNIR::dec2bin(int num)
{
    int bin = 0, k = 1;

    while (num)
    {
        bin += (num % 2) * k;
        k *= 10;
        num /= 2;
    }

    return bin;
};

string SNIR::FileToString(string chemin)
{	ifstream f; f.open(chemin.c_str()); string s; getline(f,s,(char)EOF); return s;
};
#ifdef VCL_H
UnicodeString SNIR::ToUnicodeString(string s)
{	UnicodeString us=s.c_str(); return us;
};
string SNIR::ToString(UnicodeString us)
{	string s=AnsiString(us).c_str(); return s;
};
#endif  // VCL_H
