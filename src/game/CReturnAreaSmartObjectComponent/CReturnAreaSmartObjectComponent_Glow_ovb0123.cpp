// CReturnAreaSmartObjectComponent's unit (tools/tu_map.py): the glow of the owner's model (its
// slots, through the render part +0x228 and its model +0x50) and the use request. fn_7100a0610c fades
// one of the id table's two glow parameters (+0x18e8 or +0x18f0) on or off over the given time
// (values at +0x148 and +0x158); fn_7100a05ec8 fades it out when the component is busy and the glow is full;
// fn_7100a05fec hands the player's grabber (+0x260) the grab request of the component (named by the
// argument, or by +0x68 with the glow on). Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71001c2bac(UnkSlots* self, u64 key, u32 kind, CColor4F* out, u32 mode);
void fn_71001c3040(void* object, u64 hash, u32 a, const f32* values, u32 count, f32 f);
struct UnkKraidGrab_b57a4;
void fn_710113d940(void* grabber, UnkKraidGrab_b57a4* grab, void* owner);

struct UnkReturnAreaKey_ovb0123 {
    u8 pad_0x0[0x30];
    u64 key;
};

// The id table (fn_7100031894 builds it; as src/game/CPickableComponent/CPickableComponent_Pickup_ovb0023.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x18e8];
    UnkReturnAreaKey_ovb0123* field_0x18e8;
    UnkReturnAreaKey_ovb0123* field_0x18f0;
    u8 pad_0x18f8[0x22b8 - 0x18f8];
    UnkReturnAreaKey_ovb0123* field_0x22b8;
    u8 pad_0x22c0[0x2578 - 0x22c0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& ReturnAreaIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkReturnAreaModel_ovb0123 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkReturnAreaRender_ovb0123 {
    u8 pad_0x0[0x50];
    UnkReturnAreaModel_ovb0123* model;
};

struct UnkReturnAreaOwner_ovb0123 {
    u8 pad_0x0[0x228];
    UnkReturnAreaRender_ovb0123* render;
};

// The grab request handed to the player's grabber (as CUnlockAreaSmartObjectComponent's).
struct UnkReturnAreaGrab_ovb0123 {
    u8 pad_0x0[0x10];
    CStrId name;  // 0xd0 of the component
    u8 pad_0x18[0xc];
    u32 field_0x24;
    u8 pad_0x28[3];
    bool field_0x2b;
};

struct UnkReturnAreaSmartObject_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkReturnAreaOwner_ovb0123* owner;  // 0x30
    u8 pad_0x38[8];
    u32 field_0x40;
    u8 pad_0x44[0x24];
    CStrId field_0x68;
    u8 pad_0x70[0x50];
    UnkReturnAreaGrab_ovb0123 grab;  // 0xc0
    u8 pad_0xf0[0x58];
    f32 fade_on[4];   // 0x148
    f32 fade_off[4];  // 0x158

    bool vf_0x188(bool flag) {
        return reinterpret_cast<bool (*)(UnkReturnAreaSmartObject_ovb0123*, bool)>(vtable[0x188 / 8])(this, flag);
    }
};
static_assert(__builtin_offsetof(UnkReturnAreaSmartObject_ovb0123, fade_off) == 0x158, "layout");

void fn_7100a0610c(UnkReturnAreaSmartObject_ovb0123* self, f32 time, u32 which, bool on) {
    u64 key;
    switch (which) {
    case 0:
        key = ReturnAreaIdTable().field_0x18f0->key;
        break;
    case 1:
        key = ReturnAreaIdTable().field_0x18e8->key;
        break;
    default:
        key = static_cast<u64>(-1);
        break;
    }
    UnkReturnAreaModel_ovb0123* model = self->owner->render->model;
    if (!model)
        return;
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    UnkSlots** slots;
    const f32* fade;
    if (on) {
        color.a = 1.0f;
        slots = &model->slots;
        fn_71001c28f8(*slots, key, 1, &color, 1);
        fade = self->fade_on;
    } else {
        color.a = 0.0f;
        slots = &model->slots;
        fn_71001c28f8(*slots, key, 1, &color, 1);
        fade = self->fade_off;
    }
    fn_71001c3040(*slots, key, 0, fade, 1, time);
}

// While the component is busy (vf_0x188) and the +0x18e8 glow is full, fades it out over 2.5.
void fn_7100a05ec8(UnkReturnAreaSmartObject_ovb0123* self) {
    if (!self->vf_0x188(false))
        return;
    UnkReturnAreaModel_ovb0123* model = self->owner->render->model;
    if (!model)
        return;
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    u64 key = ReturnAreaIdTable().field_0x18e8->key;
    fn_71001c2bac(model->slots, key, 0, &color, 1);
    f32 missing = 1.0f - color.a;
    if (!(missing + 0.001f >= 0.0f) || !(missing - 0.001f <= 0.0f))
        return;
    fn_7100a0610c(self, 2.5f, 1, false);
}

struct UnkReturnAreaPlayer_ovb0123 {
    u8 pad_0x0[0x260];
    void* grabber;  // 0x260
};

void fn_7100a05fec(UnkReturnAreaSmartObject_ovb0123* self, const CStrId* name) {
    auto* player = reinterpret_cast<UnkReturnAreaPlayer_ovb0123*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->grabber)
        return;
    if (name->data != &g_unk_7101d47108->empty) {
        if (self->field_0x68.data == &g_unk_7101d47108->empty)
            return;
        self->grab.name = self->field_0x68;
        fn_7100a0610c(self, 0.25f, 1, true);
    } else {
        self->grab.name = *name;
    }
    self->grab.field_0x24 = self->field_0x40;
    self->grab.field_0x2b = false;
    fn_710113d940(player->grabber, reinterpret_cast<UnkKraidGrab_b57a4*>(&self->grab), self->owner);
}

