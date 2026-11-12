// engine::scene::CSceneSlot (tools/tu_map.py unit 281, beside CSceneSlot_Forward.cpp): when the
// object at +0xa0 is present, forwards the flag and the addresses of two 8-byte keys, taken by value,
// to fn_71001ef494 and returns its result (a word, not a flag: it comes back unmasked), 0 without the
// object. Names are placeholders.
#include "types.h"

struct UnkKeyTarget281;

u32 fn_71001ef494(UnkKeyTarget281* target, bool flag, const u64* first, const u64* second);

struct UnkKeyOwner281 {
    u8 pad_0x0[0xa0];
    UnkKeyTarget281* target;  // 0xa0
};

u32 fn_7100202e7c(UnkKeyOwner281* self, bool flag, u64 first, u64 second) {
    UnkKeyTarget281* target = self->target;
    if (target != nullptr)
        return fn_71001ef494(target, flag, &first, &second);
    return 0;
}
