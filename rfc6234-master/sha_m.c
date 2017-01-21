//
//  sha.c
//  Workbench
//
//  Created by Wai Man Chan on 10/31/14.
//
//

#include "sha_m.h"

int SHA512Reset(SHA512_CTX *c) {
    SHA512_Init(c);
    return 0;
}
int SHA512Input(SHA512_CTX *c, const void *data, unsigned int len) {
    SHA512_Update(c, data, len);
    return 0;
}
int SHA512FinalBits(SHA512_CTX *c, const void *data, unsigned int len) {
    SHA512_Update(c, data, len);
    return 0;
}
int SHA512Result(SHA512_CTX *c, unsigned char *md) {
    SHA512_Final(md, c);
    return 0;
}