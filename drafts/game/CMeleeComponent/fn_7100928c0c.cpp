// CMeleeComponent's unit: the component's constructor: the component base, the vtable, the input listener
// named "MELEE_INPUT" (as CAbilityComponent's), a second listener, string ids, three arrays and cleared
// fields. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityLayout.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a83750[];

struct UnkMeleeListener_l928c0c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkMeleeComponent_l928c0c {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAbilityInputListener input;  // 0x40
    void* target;                   // 0x88
    bool field_0x90;
    u8 pad_0x91[3];
    f32 field_0x94;
    bool field_0x98;
    u8 pad_0x99[3];
    f32 field_0x9c;
    u32 field_0xa0;
    bool field_0xa4;
    u8 pad_0xa5[3];
    UnkMeleeListener_l928c0c listener;  // 0xa8
    u32 field_0xc8;
    u8 pad_0xcc[4];
    CStrId name;                     // 0xd0
    CStrId field_0xd8;
    u32 field_0xe0;
    u8 field_0xe4[0x80];
    u32 field_0x164;
    u8 field_0x168[0x80];
    bool field_0x1e8;
    u8 pad_0x1e9[7];
    CStrId field_0x1f0;
    base::global::CRntVector<void*> lists[3];  // 0x1f8
    bool field_0x270;
    u8 pad_0x271[7];
    CStrId field_0x278;
    f32 field_0x280;
    u8 pad_0x284[4];
    u64 field_0x288;
};
static_assert(__builtin_offsetof(UnkMeleeComponent_l928c0c, field_0x1e8) == 0x1e8, "layout");
static_assert(__builtin_offsetof(UnkMeleeComponent_l928c0c, field_0x1f0) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkMeleeComponent_l928c0c, field_0x288) == 0x288, "layout");

// An empty string id: the default constructor, inlined.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100928c0c(UnkMeleeComponent_l928c0c* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a83750 + 2;
    self->input.vtable = g_unk_7101a22878 + 2;
    fn_710007e704(&self->input.name, "MELEE_INPUT");
    self->input.capacity = 20;
    // An array of 20 0x1c-byte entries, after its element count.
    u64* block = static_cast<u64*>(fn_7100083ba0(0x238, 5));
    *block = 20;
    self->field_0x94 = 3.4028235e38f;
    self->field_0x90 = false;
    self->field_0x98 = false;
    self->input.entries = block + 1;
    self->target = nullptr;
    self->field_0x9c = -1.0f;
    self->field_0xa0 = 0;
    self->input.vtable = g_unk_7101a22858 + 2;
    self->field_0xa4 = false;
    self->listener.vtable = g_unk_7101a22d48 + 2;
    self->listener.field_0x8 = 0;
    self->listener.field_0x10 = 0;
    self->listener.field_0x18 = 0;
    self->field_0xc8 = 0;
    new (&self->name) CStrId(g_unk_710162e2ed, true);
    MakeEmpty(&self->field_0xd8);
    self->field_0xe0 = 0;
    self->field_0x164 = 0;
    self->field_0x1e8 = false;
    MakeEmpty(&self->field_0x1f0);
    for (base::global::CRntVector<void*>& list : self->lists)
        new (&list) base::global::CRntVector<void*>(0xf);
    self->field_0x270 = false;
    new (&self->field_0x278) CStrId(g_unk_7101d470e0);
    self->field_0x280 = 1.0f;
    self->field_0x288 = 0;
}
