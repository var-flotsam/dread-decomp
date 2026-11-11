// Unit 1102 (CXParasiteAIComponent's neighbourhood, tools/tu_map.py): fn_71006e73f4 picks one of four
// weighted choices at random (the fourth only while the player holds the item at the singleton's +0x4c8),
// fn_71006e75e8 toggles the shape's flag and, when turning on outside state 4, sets three slot values from
// the bindings of the current state before handing over to fn_71006e8208. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "types.h"

struct UnkStruct_710083e9d4;
struct UnkItem_710083e9d4;

struct UnkXParasitePlayer_ovo0307 {
    u8 pad_0x0[0x2a8];
    UnkStruct_710083e9d4* guns;  // 0x2a8
};

struct UnkXParasitePlayerLink_ovo0307 {
    u8 pad_0x0[0x10];
    UnkXParasitePlayer_ovo0307* player;  // 0x10
};

// The weights at +0x14728 and the mask of those in use.
struct UnkXParasiteWeights_ovo0307 {
    f32 weight_0;  // 0x0
    f32 weight_2;  // 0x4
    f32 weight_1;  // 0x8
    f32 weight_3;  // 0xc
    u32 mask;      // 0x10
};

struct UnkXParasitePick_ovo0307 {
    u8 pad_0x0[0x14728];
    UnkXParasiteWeights_ovo0307 w;
};

static inline const u64* ItemId() {
    return reinterpret_cast<const u64*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0x4c8);
}

u32 fn_71006e73f4(UnkXParasitePick_ovo0307* self) {
    UnkXParasiteWeights_ovo0307* w = &self->w;
    UnkItem_710083e9d4* item = nullptr;
    auto* link = *reinterpret_cast<UnkXParasitePlayerLink_ovo0307**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0);
    if (link && link->player) {
        if (link->player->guns)
            item = fn_710083e9d4(link->player->guns, ItemId(), false);
    }
    f32 total = 0.0f;
    if (w->mask & 4)
        total = w->weight_2 + 0.0f;
    if (w->mask & 2)
        total += w->weight_1;
    if (w->mask & 1)
        total += w->weight_0;
    if ((w->mask & 8) && item)
        total += w->weight_3;
    f32 pick = total * (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    f32 t = pick + 0.0f;
    f32 acc = 0.0f;
    if (w->mask & 4) {
        f32 v = w->weight_2;
        if (v > 0.0f) {
            if (!(t > v))
                return 1;
            acc = v + 0.0f;
        }
    }
    if (w->mask & 2) {
        f32 v = w->weight_1;
        if (v > 0.0f) {
            acc += v;
            if (!(t > acc))
                return 2;
        }
    }
    if (w->mask & 1) {
        f32 v = w->weight_0;
        if (v > 0.0f) {
            acc += v;
            if (!(t > acc))
                return 0;
        }
    }
    if (w->mask & 8) {
        f32 v = w->weight_3;
        acc += v;
        if (v > 0.0f && item != nullptr && !(t > acc))
            return 3;
    }
    return 0;
}

// UnkXParasiteSelf_8a18 as src/game/UnkTU1102/UnkTU1102_Parasite_ovb0252.cpp defines it (a complete type
// keeps the original's addressing; the fields used here are read through UnkXParasiteSelf_ovo0307).
struct UnkStruct_710024fa0c;
struct UnkXParasitePartner_8a18 {
    u8 pad_0x0[0x2bc];
    u32 field_0x2bc;
    u32 field_0x2c0;
};

struct UnkXParasitePartnerLink_8a18 {
    u8 pad_0x0[0x10];
    UnkXParasitePartner_8a18* partner;  // 0x10
};

struct UnkXParasiteListener_8a18 {
    void** vtable;
    void Notify(void* self) { reinterpret_cast<void (*)(UnkXParasiteListener_8a18*, void*)>(vtable[0x410 / 8])(this, self); }
};

struct UnkXParasiteGrabbed_8a18 {
    u8 pad_0x0[0x240];
    UnkXParasiteListener_8a18* listener;  // 0x240
    u8 pad_0x248[0x150];
    UnkXParasitePartnerLink_8a18* partner_link;  // 0x398
};

struct UnkXParasiteLink_8a18 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkXParasiteGrabbed_8a18* object;  // 0x10
};

struct UnkXParasiteOwner_8a18 {
    u8 pad_0x0[0x218];
    UnkStruct_710024fa0c* field_0x218;
};

struct UnkXParasiteState_8a18 {
    s32 state;  // 0x146e0
    s32 field_0x4;
    u8 pad_0x8[8];
    u64 field_0x10;
    u32 field_0x18;
    u8 pad_0x1c[0x18];
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    u8 pad_0x41[0x6f];
    UnkXParasiteLink_8a18* link;  // 0xb0
    u8 pad_0xb8[0x21];
    bool released;  // 0xd9
};
static_assert(__builtin_offsetof(UnkXParasiteState_8a18, link) == 0xb0, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteState_8a18, released) == 0xd9, "placeholder layout");

struct UnkXParasiteSelf_8a18 {
    u8 pad_0x0[0x30];
    UnkXParasiteOwner_8a18* owner;  // 0x30
    u8 pad_0x38[0x146e0 - 0x38];
    UnkXParasiteState_8a18 s;  // 0x146e0
};

struct UnkXParasiteSelf_8208;
void fn_71006e8208(UnkXParasiteSelf_8208* self, bool on);

struct UnkXParasiteBinding_ovo0307 {
    u8 pad_0x0[0x18];
    base::math::CVector4D first;   // 0x18
    base::math::CVector4D second;  // 0x28
};
static_assert(sizeof(UnkXParasiteBinding_ovo0307) == 0x38, "placeholder layout");

struct UnkXParasiteBindings_ovo0307 {
    UnkXParasiteBinding_ovo0307* values;
    u32* keys;
    u32 count;
};

struct UnkXParasiteShape_ovo0307 {
    u8 pad_0x0[0x114];
    bool field_0x114;
    u8 pad_0x115[7];
    u8 field_0x11c;
    u8 pad_0x11d[0x138 - 0x11d];
    UnkSlots* slots;  // 0x138
};

struct UnkXParasiteHolder_ovo0307 {
    u8 pad_0x0[0x50];
    UnkXParasiteShape_ovo0307* shape;  // 0x50
};

struct UnkXParasiteOwner_ovo0307 {
    u8 pad_0x0[0x228];
    UnkXParasiteHolder_ovo0307* holder;  // 0x228
};

struct UnkXParasiteSelf_ovo0307 {
    u8 pad_0x0[0x30];
    UnkXParasiteOwner_ovo0307* owner;  // 0x30
    u8 pad_0x38[0x14724 - 0x38];
    s32 state;  // 0x14724
    u8 pad_0x14728[0x14768 - 0x14728];
    UnkXParasiteBindings_ovo0307 bindings;  // 0x14768
};
static_assert(__builtin_offsetof(UnkXParasiteSelf_ovo0307, bindings) == 0x14768, "placeholder layout");

// The binding of the current state (one past the end when none).
static inline UnkXParasiteBinding_ovo0307* Find(UnkXParasiteSelf_ovo0307* self) {
    u64 i = 0;
    for (; i < self->bindings.count; ++i) {
        if (self->bindings.keys[i] == (u32)self->state)
            break;
    }
    return &self->bindings.values[i];
}

void fn_71006e75e8(UnkXParasiteSelf_8a18* self_, bool on) {
    UnkXParasiteSelf_ovo0307* self = reinterpret_cast<UnkXParasiteSelf_ovo0307*>(self_);
    if (UnkXParasiteHolder_ovo0307* holder = self->owner->holder) {
        if (holder->shape->field_0x11c)
            *reinterpret_cast<char*>(&holder->shape->field_0x114) = !on;
    }
    if (on) {
        if (self->state != 4) {
            if (UnkXParasiteHolder_ovo0307* holder = self->owner->holder) {
                if (UnkXParasiteShape_ovo0307* shape = holder->shape) {
                    base::math::CVector4D* first = &Find(self)->first;
                    fn_71001c2764(shape->slots, 0, 0, first, 1, true);
                    base::math::CVector4D* second = &Find(self)->second;
                    fn_71001c2764(shape->slots, 0, 1, second, 1, true);
                    base::math::CVector4D zero = {};
                    fn_71001c2764(shape->slots, 0, 2, &zero, 1, true);
                }
            }
        }
    }
    fn_71006e8208(reinterpret_cast<UnkXParasiteSelf_8208*>(self), on);
}
