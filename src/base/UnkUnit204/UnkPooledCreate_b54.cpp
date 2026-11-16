// Units 204, 254 and 259 of tools/tu_map.py: three identical creators of a pooled object (0x58,
// 0x188 and 0x29d0 bytes, each with its own constructor): the object is allocated with the given
// flags and constructed, given its owner (fn_710019e95c) when there is one, and added to the pool's
// chained hash set of pointers (the layout of src/base/spatial/UnkModelInstanceSet.cpp's set, here
// at +8). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "game/UnkTU218.h"
#include "game/UnkTU223.h"
#include "game/UnkTU226.h"
#include "types.h"

void fn_71001a9f84(void* self);

struct UnkPool_b54a1 {
    u64 field_0x0;
    base::global::CRntDictionary<u64, u64> objects;  // 0x8: each object its own value

    template <u64 Size, void (*Ctor)(void*)>
    void* Create(void* owner, u32 flags) {
        void* obj = fn_71000839a0(Size, flags);
        Ctor(obj);
        if (obj) {
            if (owner)
                fn_710019e95c(obj, owner);
            objects.InsertChecked(reinterpret_cast<u64>(obj), reinterpret_cast<u64>(obj));
        }
        return obj;
    }
};

void* fn_7100169e28(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x58, fn_71001a9f84>(owner, flags);
}
void* fn_71001c35b4(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x188, fn_710019bd2c>(owner, flags);
}
void* fn_71001cb410(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x29d0, fn_7100198aac>(owner, flags);
}
