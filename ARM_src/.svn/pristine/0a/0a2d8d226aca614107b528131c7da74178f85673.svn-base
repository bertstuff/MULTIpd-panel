#ifndef _SHA1_H
#define _SHA1_H

#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

//#include "defs.h"

#define SHA1_HASH_SIZE 20

/* Hash size in 32-bit words */
#define SHA1_HASH_WORDS 5

struct _SHA1Context {
  uint64_t totalLength;
  uint32_t hash[SHA1_HASH_WORDS];
  uint32_t bufferLength;
  union {
    uint32_t words[16];
    uint8_t bytes[64];
  } buffer;
#ifdef RUNTIME_ENDIAN
  int littleEndian;
#endif /* RUNTIME_ENDIAN */
};

typedef struct _SHA1Context SHA1Context;

#ifdef __cplusplus
extern "C" {
#endif

void SHA1Init (SHA1Context *sc);
void SHA1Update (SHA1Context *sc, const void *data, uint32_t len);
void SHA1Final (SHA1Context *sc, uint8_t hash[SHA1_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif /* _SHA1_H */
