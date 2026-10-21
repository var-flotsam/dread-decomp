// Sets two global mode values (0-9). When either changes, marks two flags dirty, records the
// new values, maps each into a second numbering (0-3 -> 1-4, 4-5 -> 9-10, 6-9 -> 5-8), and
// passes the mapped pair, twice, to a global callback.
#include "types.h"

// All reached through the GOT.
extern s32 unk_7101cc5e68;   // first value
extern s32 unk_7101cc5e70;   // second value
extern bool unk_7101cc5da8;  // dirty flags
extern bool unk_7101cc5db0;
extern s32 unk_7101cc5e78;   // first value, mapped
extern s32 unk_7101cc5e80;   // second value, mapped
struct UnkTarget_710015a500;
extern UnkTarget_710015a500 unk_7101cc5cd8;
extern void (*unk_7101cc54b0)(UnkTarget_710015a500* target, s32 a, s32 b, s32 c, s32 d);

static inline void Map_710015a500(s32 v, s32* out) {
    switch (v) {
    case 0:
        *out = 1;
        break;
    case 1:
        *out = 2;
        break;
    case 2:
        *out = 3;
        break;
    case 3:
        *out = 4;
        break;
    case 4:
        *out = 9;
        break;
    case 5:
        *out = 10;
        break;
    case 6:
        *out = 5;
        break;
    case 7:
        *out = 6;
        break;
    case 8:
        *out = 7;
        break;
    case 9:
        *out = 8;
        break;
    }
}

void fn_710015a500(s32 a, s32 b) {
    if (unk_7101cc5e68 == a && unk_7101cc5e70 == b)
        return;
    unk_7101cc5da8 = true;
    unk_7101cc5db0 = true;
    if (unk_7101cc5e68 != a) {
        unk_7101cc5e68 = a;
        Map_710015a500(a, &unk_7101cc5e78);
    }
    if (unk_7101cc5e70 != b) {
        unk_7101cc5e70 = b;
        Map_710015a500(b, &unk_7101cc5e80);
    }
    s32 ma = unk_7101cc5e78;
    s32 mb = unk_7101cc5e80;
    unk_7101cc54b0(&unk_7101cc5cd8, ma, mb, ma, mb);
}
