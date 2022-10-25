#include "cipher.h"
#include <ctype.h>
#define ASCIIBIG 65
#define ASCIISMALL 97
#define ALPHABET_SIZE 26

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

int modulo (int x, int n){
    return (x%n +n) %n;
}

// See full documentation in header file
void encode (char s[], int k)
{
    char *c;
    int i = 0;
    for (c = &s[i]; *c != '\0'; c = &s[++i]) {
        if (islower(*c)) {
            *c = modulo((*c + k)-ASCIISMALL, ALPHABET_SIZE) + ASCIISMALL;
        }
        if (isupper(*c)){
            *c = modulo(((*c + k)-ASCIIBIG), ALPHABET_SIZE) + ASCIIBIG;
        }
    }
}

// See full documentation in header file
void decode (char s[], int k)
{
    encode(s, (-1*k));
}
