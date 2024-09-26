#ifndef AES_H
#define AES_H

#define NB_LEAKAGE 4096

int aes_withleakage(const char *received, char *trace);
#endif // AES_H
