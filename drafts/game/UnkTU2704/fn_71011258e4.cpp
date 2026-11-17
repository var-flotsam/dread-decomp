// UnkTU2704 (tools/tu_map.py unit 2704, CSpawnPointComponent's unit): the spawn point component's
// constructor: the base component's, then string ids, strings, flags, callback sets, a string-id list
// and arrays. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101cb28c8[];  // The spawn point component's vtable.
void fn_710112ef98(char* self);   // base::refl_StrIdListCtor<..>, instantiated in src/clones

// A callback set: an array of callables after the vtable, and a word.
struct UnkCallbackSetBase_l1258e4 {
    void** vtable;
    CRntVector<void*> callbacks;  // 0x8
    u32 field_0x30;

    UnkCallbackSetBase_l1258e4() : vtable(g_unk_7101a20998 + 2), callbacks(0xf), field_0x30(0) {}
};

struct UnkCallbackSet_l1258e4 : UnkCallbackSetBase_l1258e4 {
    void* field_0x38;

    UnkCallbackSet_l1258e4() : field_0x38(nullptr) { vtable = g_unk_7101a20950 + 2; }
};
static_assert(sizeof(UnkCallbackSet_l1258e4) == 0x40, "UnkCallbackSet_l1258e4 layout");

struct UnkSpawnPointComponent_l1258e4 {
    void** vtable;
    u8 pad_0x8[0x38];
    u64 field_0x40;
    CStrId field_0x48;
    u64 field_0x50;
    UnkStringData* field_0x58;
    UnkCallbackSet_l1258e4 field_0x60;
    UnkStringData* field_0xa0;
    CRntString field_0xa8;
    CRntString field_0xd8;
    bool field_0x108;
    bool field_0x109;
    bool field_0x10a;
    u32 field_0x10c;
    s32 field_0x110;
    bool field_0x114;
    u64 field_0x118;
    UnkCallbackSet_l1258e4 field_0x120;
    u64 field_0x160;
    u64 field_0x168;
    UnkCallbackSet_l1258e4 field_0x170;
    u64 field_0x1b0;
    UnkCallbackSet_l1258e4 field_0x1b8[3];
    u32 field_0x278;
    bool field_0x27c;
    u8 pad_0x27d[3];
    char field_0x280[0x38];  // string-id list
    bool field_0x2b8;
    u32 field_0x2bc;
    u32 field_0x2c0;
    CRntVector<void*> field_0x2c8;
    UnkCallbackSet_l1258e4 field_0x2f0;
    bool field_0x330;
    bool field_0x331;
    bool field_0x332;
    u32 field_0x334;
    bool field_0x338;
    u64 field_0x340;
    u64 field_0x348;
    bool field_0x350;
    bool field_0x351;
    CRntVector<void*> field_0x358;
    u64 field_0x380;
    u32 field_0x388;
    CRntVector<void*> field_0x390;
};
static_assert(__builtin_offsetof(UnkSpawnPointComponent_l1258e4, field_0x1b8) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkSpawnPointComponent_l1258e4, field_0x280) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkSpawnPointComponent_l1258e4, field_0x2c8) == 0x2c8, "layout");
static_assert(__builtin_offsetof(UnkSpawnPointComponent_l1258e4, field_0x2f0) == 0x2f0, "layout");
static_assert(__builtin_offsetof(UnkSpawnPointComponent_l1258e4, field_0x358) == 0x358, "layout");
static_assert(sizeof(UnkSpawnPointComponent_l1258e4) == 0x3b8, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71011258e4(void* p) {
    UnkSpawnPointComponent_l1258e4* self = static_cast<UnkSpawnPointComponent_l1258e4*>(p);
    fn_71010c4148(static_cast<UnkStateObject*>(p));
    self->vtable = g_unk_7101cb28c8 + 2;
    self->field_0x40 = 0;
    new (&self->field_0x48) CStrId(g_unk_7101d470e0);
    self->field_0x50 = 0;
    MakeEmpty(&self->field_0x58);
    new (&self->field_0x60) UnkCallbackSet_l1258e4();
    MakeEmpty(&self->field_0xa0);
    fn_710007e6c4(&self->field_0xa8);
    fn_710007e6c4(&self->field_0xd8);
    self->field_0x108 = false;
    self->field_0x109 = true;
    self->field_0x10a = false;
    self->field_0x10c = 0;
    self->field_0x110 = -1;
    self->field_0x114 = false;
    self->field_0x118 = 0;
    new (&self->field_0x120) UnkCallbackSet_l1258e4();
    self->field_0x160 = 0;
    self->field_0x168 = 0;
    new (&self->field_0x170) UnkCallbackSet_l1258e4();
    self->field_0x1b0 = 0;
    for (UnkCallbackSet_l1258e4& set : self->field_0x1b8)
        new (&set) UnkCallbackSet_l1258e4();
    self->field_0x27c = false;
    fn_710112ef98(self->field_0x280);
    self->field_0x2b8 = false;
    self->field_0x2bc = 0;
    self->field_0x2c0 = 0;
    new (&self->field_0x2c8) CRntVector<void*>(0xf);
    new (&self->field_0x2f0) UnkCallbackSet_l1258e4();
    self->field_0x330 = false;
    self->field_0x331 = false;
    self->field_0x332 = false;
    self->field_0x334 = 5;
    self->field_0x338 = false;
    self->field_0x340 = 0;
    self->field_0x348 = 0;
    self->field_0x350 = false;
    self->field_0x351 = false;
    new (&self->field_0x358) CRntVector<void*>(0xf);
    self->field_0x380 = 0;
    self->field_0x388 = 0;
    new (&self->field_0x390) CRntVector<void*>(0xf);
}
