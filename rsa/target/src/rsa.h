#ifndef RSA_H
#define RSA_H

#define NB_LEAKAGE 1000000
#define MAX_SIZE 32

int rsa_withleakage(const char *received, char *trace);
#endif // AES_H
