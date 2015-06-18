#ifndef _TLV_H_
#define _TLV_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN32
    #include "pshpack1.h"
#endif
typedef struct tlv {
    uint32_t typ;
    uint32_t len;
    char *val;
#if defined(_WIN32)
}
    #include "poppack.h"
#else
} __attribute__((packed)) tlv;
#endif

tlv *tlv_getNext(char **data, size_t *buflen);
tlv *tlv_get(char *data, size_t buflen);
void tlv_free(tlv *p);
tlv *tlv_copy(tlv *src);
uint32_t tlv_getType(char *data, size_t buflen);
uint32_t tlv_getLen(char *data, size_t buflen);
ssize_t tlv_send(size_t sock, uint32_t typ, uint32_t len, char *val);

#endif /* _TLV_H_ */
