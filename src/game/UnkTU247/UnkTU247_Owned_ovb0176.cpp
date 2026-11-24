// Unit 247 (no reflected class): an object that owns a pooled object (made like
// src/base/UnkUnit204/UnkPooledCreate_b54.cpp's 0x29d0-byte one) and a list of string ids. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkTU204.h"
#include "game/UnkTU207.h"
#include "game/UnkTU218.h"
#include "game/UnkTU225.h"
#include "game/UnkTU226.h"
#include "types.h"

using base::global::CStrId;

void fn_710016a8dc(void* self, void* arg);
struct UnkKeyOwner;
void* fn_710019964c(void* self);

struct UnkPool_ovb0176 {
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

struct UnkOwnedHolder_ovb0176 {
    u8 pad_0x0[0x78];
    void* object;  // 0x78
    u8 field_0x80[0x58];
    CStrId* ids;  // 0xd8
    u32 id_count;  // 0xe0
};
static_assert(__builtin_offsetof(UnkOwnedHolder_ovb0176, id_count) == 0xe0, "layout");

void fn_71001bbc88(UnkOwnedHolder_ovb0176* self) {
    fn_710016ae30(self->field_0x80);
    for (u64 i = 0; i < self->id_count; ++i)
        self->ids[i].Reset();
    self->id_count = 0;
    if (self->object) {
        fn_710016a8dc(self->field_0x80, nullptr);
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->object));
        self->object = nullptr;
    }
}

void fn_71001bbd6c(UnkOwnedHolder_ovb0176* self, void* owner) {
    if (self->object) {
        fn_710016a8dc(self->field_0x80, nullptr);
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->object));
        self->object = nullptr;
    }
    if (owner) {
        auto* pool = static_cast<UnkPool_ovb0176*>(g_unk_716e34c2e0);
        void* obj = pool->Create<0x29d0, fn_7100198aac>(owner, 0x1a);
        self->object = obj;
        fn_710016a8dc(self->field_0x80, **reinterpret_cast<void***>(reinterpret_cast<u8*>(obj) + 0x48));
    }
}
