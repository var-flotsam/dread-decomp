// tools/tu_map.py unit 132 (beside src/base/UnkTaggedObjectDtors.cpp): two identical setters that,
// unless locked (+0x158), pass `on` to the part at +0x20 (fn_7100206ecc), and always keep its
// inverse as a word at +0x15c. Names are placeholders.
#include "game/UnkTU132.h"
#include "types.h"

struct UnkPart132;
void fn_7100206ecc(UnkPart132* part, bool on);

struct UnkOwner132 {
    u8 pad_0x0[0x20];
    u8 part[0x138];  // 0x20
    bool locked;     // 0x158
    u8 pad_0x159[0x3];
    u32 off;  // 0x15c
};

static inline void SetEnabled132(UnkOwner132* self, bool on) {
    if (!self->locked) {
        fn_7100206ecc(reinterpret_cast<UnkPart132*>(self->part), on);
    }
    self->off = !on;
}

void fn_7100145818(UnkOwner132* self, bool on) {
    SetEnabled132(self, on);
}

void fn_71001458b8(UnkOwner132* self, bool on) {
    SetEnabled132(self, on);
}
