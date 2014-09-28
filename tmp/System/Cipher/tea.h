#ifndef _TEA_H_
#define _TEA_H_

#include "../KPlatform.h"
#include <cmath>


class TEA {
public:
     TEA(const BYTE *key, int round = 32);
     TEA(const TEA &rhs);
     TEA& operator=(const TEA &rhs);
     void encrypt(const BYTE *in, BYTE *out);
     void decrypt(const BYTE *in, BYTE *out);
inline double logbase(double base, double x) {
     return log(x)/log(base);
	}

private:
     void encrypt(const ULONG *in, ULONG *out);
     void decrypt(const ULONG *in, ULONG *out);
private:
     int _round; //iteration round to encrypt or decrypt
     bool _isNetByte; //whether input bytes come from network
     BYTE _key[16]; //encrypt or decrypt key
};


#endif