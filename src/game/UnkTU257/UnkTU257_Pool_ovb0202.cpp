// Unit 257 of tools/tu_map.py (no class hint): creates the object this one holds (+0x78) in its pool
// (g_unk_716e34c2e0): a 0x1c0-byte object, constructed with fn_710019c6c0 (src/game_s1/UnkUnit224/
// UnkUnit224_Ctor_A1_b69.cpp) and added to the pool's chained hash set of pointers like
// src/base/UnkUnit204/UnkPooledCreate_b54.cpp does. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "game/UnkTU204.h"
#include "types.h"

struct UnkObject224_b69a1;
void fn_710019c6c0(UnkObject224_b69a1* self);

struct UnkPool_b0202 {
    u64 field_0x0;
    base::global::CRntDictionary<u64, u64> objects;  // 0x8: each object its own value
};

struct UnkHolder_b0202 {
    u8 pad_0x0[0x78];
    void* object;  // 0x78
};

void fn_71001c3d80(UnkHolder_b0202* self) {
    UnkPool_b0202* pool = static_cast<UnkPool_b0202*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(0x1c0, 0x3b);
    fn_710019c6c0(static_cast<UnkObject224_b69a1*>(obj));
    if (obj)
        pool->objects.InsertChecked(reinterpret_cast<u64>(obj), reinterpret_cast<u64>(obj));
    self->object = obj;
}
