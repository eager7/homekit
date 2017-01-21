//#include "../Configuration.h"


#include "utils.h"

#include <openssl/sha.h>
#include <stdint.h>
#include <unistd.h>
typedef SHA512_CTX SHACTX;
#define SHAInit SHA512_Init
#define SHAUpdate SHA512_Update
#define SHAFinal SHA512_Final
#define SHA_DIGESTSIZE 64
#define SHA_BlockSize 128
