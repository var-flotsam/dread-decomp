// CCharClassShootActivatorComponent unit: the shoot activator's constructor (fn_710088fce4). After the base
// constructor (fn_71008e04e8) it sets the vtable, builds four engine arrays, a callback bound to the object,
// copies of the default string and file path ids, a string-id list, two floats, two empty string ids, two
// more arrays and a callback set. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkLifeComponent_A5_b71;

extern void* g_unk_7101a7c540[];  // the component's vtable

void fn_7100893a1c(char* self);  // base::refl_StrIdListCtor<..>, instantiated in src/clones
u32 fn_7100896404();

// A callback set: the base (vtable, an owned array of callbacks and a count), then its owner.
struct UnkShootListBase_l88fce4 {
    char* vtable;
    UnkOwnedErasedVector callables;
    u32 field_0x30;

    UnkShootListBase_l88fce4() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
};

struct UnkShootList_l88fce4 : UnkShootListBase_l88fce4 {
    void* owner;

    UnkShootList_l88fce4() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner = nullptr;
    }
};

// An empty string id: the pool is created if need be.
struct UnkShootEmptyId_l88fce4 {
    UnkStringData* data;

    UnkShootEmptyId_l88fce4() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkShootActivator_l88fce4 {
    char* vtable;
    u8 pad_0x8[0x380];
    UnkOwnedErasedVector field_0x388;
    UnkOwnedErasedVector field_0x3b0;
    UnkOwnedErasedVector field_0x3d8;
    UnkOwnedErasedVector field_0x400;
    u64 field_0x428;
    void* callback_owner;
    u32 (*callback)();
    u64 field_0x440;
    u64 field_0x448;
    CStrId field_0x450;
    CFilePathStrId field_0x458;
    u64 field_0x460;
    u64 field_0x468;
    s64 field_0x470;
    char list[0x38];  // 0x478
    f32 field_0x4b0;
    f32 field_0x4b4;
    u32 field_0x4b8;
    u32 field_0x4bc;
    u32 field_0x4c0;
    u32 field_0x4c4;
    u32 field_0x4c8;
    u32 field_0x4cc;
    UnkShootEmptyId_l88fce4 field_0x4d0;
    UnkShootEmptyId_l88fce4 field_0x4d8;
    UnkOwnedErasedVector field_0x4e0;
    UnkOwnedErasedVector field_0x508;
    UnkShootList_l88fce4 field_0x530;
    u8 field_0x570;
    u32 field_0x574;
};
static_assert(__builtin_offsetof(UnkShootActivator_l88fce4, field_0x450) == 0x450, "layout");
static_assert(__builtin_offsetof(UnkShootActivator_l88fce4, field_0x4d0) == 0x4d0, "layout");
static_assert(__builtin_offsetof(UnkShootActivator_l88fce4, field_0x574) == 0x574, "layout");

void fn_710088fce4(void* self_) {
    UnkShootActivator_l88fce4* self = static_cast<UnkShootActivator_l88fce4*>(self_);
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a7c540) + 0x10;
    new (&self->field_0x388) UnkOwnedErasedVector;
    new (&self->field_0x3b0) UnkOwnedErasedVector;
    new (&self->field_0x3d8) UnkOwnedErasedVector;
    new (&self->field_0x400) UnkOwnedErasedVector;
    self->field_0x428 = 0;
    self->callback_owner = self;
    self->callback = fn_7100896404;
    self->field_0x440 = 0;
    self->field_0x448 = 0;
    new (&self->field_0x450) CStrId(g_unk_7101d470e0);
    new (&self->field_0x458) CFilePathStrId(g_unk_7101d470e8);
    self->field_0x460 = 0;
    self->field_0x468 = 0;
    self->field_0x470 = -1;
    fn_7100893a1c(self->list);
    self->field_0x4b0 = 2.0f;
    self->field_0x4b4 = 0.3f;
    self->field_0x4bc = 0;
    self->field_0x4c0 = 0;
    self->field_0x4c4 = 0;
    self->field_0x4c8 = 0;
    self->field_0x4cc = 0;
    new (&self->field_0x4d0) UnkShootEmptyId_l88fce4;
    new (&self->field_0x4d8) UnkShootEmptyId_l88fce4;
    new (&self->field_0x4e0) UnkOwnedErasedVector;
    new (&self->field_0x508) UnkOwnedErasedVector;
    new (&self->field_0x530) UnkShootList_l88fce4;
    self->field_0x570 = 0;
    self->field_0x574 = 0;
}
