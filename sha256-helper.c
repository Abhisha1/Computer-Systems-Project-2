#include <stdlib.h>
#include <assert.h>
#include "proj-2_sha256.h"
#include "sha256-helper.h"

// adapted from https://github.com/RemyNoulin/sha256
char *sha256_byteToHexString(BYTE data[]) {
	char *hexC = "0123456789abcdef";
	char *hexS = malloc(65);
    assert(hexS);
	if (!hexS) return NULL;
	for(BYTE i=0; i<32; i++) {
		hexS[i*2]   = hexC[data[i]>>4];
		hexS[i*2+1] = hexC[data[i]&0xF];
	}
	hexS[64] = 0;
	return hexS;
}