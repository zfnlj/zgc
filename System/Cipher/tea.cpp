#include "tea.h"

#include <cstring> //for memcpy,memset
 
using namespace std;
 
TEA::TEA(const BYTE *key, int round /*= 32*/)
:_round(round)
{
     if (key != 0)
         memcpy(_key, key, 16);
     else
         memset(_key, 0, 16);
 }
 
TEA::TEA(const TEA &rhs)
 :_round(rhs._round)
{
    memcpy(_key, rhs._key, 16);
}
 
TEA& TEA::operator=(const TEA &rhs) {
     if (&rhs != this) {
         _round = rhs._round;
         memcpy(_key, rhs._key, 16);
     }
     return *this;
}
 
void TEA::encrypt(const BYTE *in, BYTE *out) {
     encrypt((const ULONG*)in, (ULONG*)out);
}
 
void TEA::decrypt(const BYTE *in, BYTE *out) {
     decrypt((const ULONG*)in, (ULONG*)out);
}
 
void TEA::encrypt(const ULONG *in, ULONG *out) {
 
     ULONG *k = (ULONG*)_key;
     register ULONG y = in[0];
     register ULONG z = in[1];
     register ULONG a = k[0];
     register ULONG b = k[1];
     register ULONG c = k[2];
     register ULONG d = k[3];
     register ULONG delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
     register int round = _round;
     register ULONG sum = 0;
 
     while (round--) {    /* basic cycle start */
         sum += delta;
         y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
         z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
     }    /* end cycle */
     out[0] = y;
     out[1] = z;
}
 
void TEA::decrypt(const ULONG *in, ULONG *out) {
 
     ULONG *k = (ULONG*)_key;
     register ULONG y = in[0];
     register ULONG z = in[1];
     register ULONG a = k[0];
     register ULONG b = k[1];
     register ULONG c = k[2];
     register ULONG d = k[3];
     register ULONG delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
     register int round = _round;
     register ULONG sum = 0;
 
     if (round == 32)
         sum = 0xC6EF3720; /* delta << 5*/
     else if (round == 16)
         sum = 0xE3779B90; /* delta << 4*/
     else
         sum = delta << static_cast<int>(logbase(2, round));
 
     while (round--) {    /* basic cycle start */
         z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
         y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
         sum -= delta;
     }    /* end cycle */
     out[0] = y;
     out[1] = z;
}
