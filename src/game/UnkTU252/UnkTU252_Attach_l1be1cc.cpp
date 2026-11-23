// Unit 252 (tools/tu_map.py): fn_71001be1cc drops the owner's pooled object (fn_710019e100 takes it out of the
// scene registry's pool), then, given an owner, creates a new one in the pool and appends a reference to each of its
// items to the owner's list at +0xf0. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "game/UnkPool252.h"
#include "game/UnkTU204.h"
#include "game/UnkTU218.h"
#include "game/UnkTU225.h"
#include "game/UnkTU226.h"
#include "types.h"

struct UnkPooledItem_l1be1cc {
    void* object;
    u8 pad_0x8[0x18];
};

struct UnkPooledObject_l1be1cc {
    u8 pad_0x0[0x34];
    u32 item_count;
    UnkPooledItem_l1be1cc* items;  // 0x38
};

struct UnkItemRef_l1be1cc {
    void* object;
    bool active;

    ~UnkItemRef_l1be1cc() {}
};

struct UnkPooledOwner_l1be1cc {
    u8 pad_0x0[0xf0];
    base::global::CRntVector<UnkItemRef_l1be1cc> items;  // 0xf0
    u8 pad_0x118[0xa0];
    UnkPooledObject_l1be1cc* object;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkPooledOwner_l1be1cc, object) == 0x1b8, "");

void fn_71001be1cc(UnkPooledOwner_l1be1cc* self, void* owner) {
    if (self->object) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->object));
        self->object = nullptr;
    }
    if (!owner)
        return;
    UnkPool252* pool = static_cast<UnkPool252*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(0x29d0, 0x1e);
    fn_7100198aac(obj);
    if (obj) {
        fn_710019e95c(obj, owner);
        pool->objects.InsertChecked(reinterpret_cast<u64>(obj), reinterpret_cast<u64>(obj));
    }
    self->object = static_cast<UnkPooledObject_l1be1cc*>(obj);
    u32 n = self->object->item_count;
    for (u32 i = 0; i < n; ++i)
        self->items.EmplaceBack({self->object->items[i].object, true});
}
