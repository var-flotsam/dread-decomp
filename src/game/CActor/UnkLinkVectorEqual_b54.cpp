// CActor's unit (tools/tu_map.py): equality of two base::UnkVectors of 0x40-byte links; a copy of
// fn_710113192c (src/game/CSpawnPointComponent/CSpawnPointComponent_c3_1.cpp) that resolves a link
// with fn_710023b580 and fn_7100234f78. A link whose state (+0x30) is 1 is resolved first and its
// state set to 2; links are equal when their handles' targets (+0x10) are. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLinkHandle_b54a1 {
    u8 pad_0x0[0x10];
    void* target;
};

struct UnkLink_b54a1 {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_b54a1* handle;

    void* Get() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(key);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&handle),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return handle ? handle->target : nullptr;
    }
};
static_assert(sizeof(UnkLink_b54a1) == 0x40, "UnkLink_b54a1 layout");

bool fn_710024693c(base::UnkVector<UnkLink_b54a1>* self, base::UnkVector<UnkLink_b54a1>* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i) {
        UnkLink_b54a1& a = self->data[i];
        UnkLink_b54a1& b = other->data[i];
        void* target = a.Get();
        if (b.Get() != target)
            return false;
    }
    return true;
}
