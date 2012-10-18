#ifndef __TLV_MISC_H__
#define __TLV_MISC_H__

#define MIN(_a, _b) (_a < _b ? _a : _b)
#define MIN3(_a, _b, _c) (_a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c))
#define MIN2 MIN

#define MAX(_a, _b) (_a > _b ? _a : _b)
#define MAX3(_a, _b, _c) (_a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c))
#define MAX2 MAX

#endif /* __TLV_MISC_H__ */
