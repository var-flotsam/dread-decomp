// CBillboardComponent's unit (tools/tu_map.py, unit 1152): more of the billboard component (the entries of
// its two ids, +0x48 and +0x4c, in the manager g_unk_71726da048, created on first use as in
// CBillboardComponent_b39.cpp). fn_71007274f4: after the base (fn_71010c4230), both entries' toggles (+0x18 of
// their data) are switched off, +0xa8 is reset and a live effect handle (+0x98) is stopped through the
// owner's (+0x30) effect player (+0x2b8, fn_7100719894) and cleared to the empty handle g_unk_7108b6c460.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CAudioComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU243.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU272.h"
#include "types.h"

extern void* g_unk_71726da048;
struct UnkHandleOwner_71010c4230;

struct UnkBillboardData_ovb0113 {
    u8 pad_0x0[0x18];
    u8 toggle[8];  // 0x18
    u8 pad_0x20[0x113 - 0x20];
    bool field_0x113;
    u8 pad_0x114[0x128 - 0x114];
    void* field_0x128;
};

struct UnkBillboardEntry_ovb0113 {
    u8 pad_0x0[0x4];
    u32 field_0x4;
    u8 pad_0x8[0x8];
    UnkBillboardData_ovb0113* data;  // 0x10
    void* field_0x18;
};
struct UnkBillboardEntry_bbc;  // as CBillboardComponent_b39.cpp declares it

static inline UnkBillboardEntry_ovb0113* Find_ovb0113(void* manager, u32 id) {
    return reinterpret_cast<UnkBillboardEntry_ovb0113*>(fn_7100e1caf0(manager, id));
}

static inline void* Manager_ovb0113() {
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    return manager;
}

// A handle to a played effect (as src/game/CAreaSoundComponent/CAreaSoundComponent_Paths_ovb0062.cpp names it):
// live while its reference's serial matches.
struct UnkSoundRef {
    void* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkBillboardOwner_ovb0113 {
    u8 pad_0x0[0x2b8];
    void* effects;  // 0x2b8
};

struct UnkBillboardComponent_ovb0113 {
    u8 pad_0x0[0x30];
    UnkBillboardOwner_ovb0113* owner;  // 0x30
    u8 pad_0x38[0x10];
    u32 id;         // 0x48
    u32 glow_id;    // 0x4c
    struct UnkBillboardItem_ovb0113* items;  // 0x50
    u8 pad_0x58[0x98 - 0x58];
    UnkSoundHandle effect;  // 0x98
    s32 field_0xa8;
    u8 pad_0xac[0x110 - 0xac];
    s32 item_count;  // 0x110
};

struct UnkBillboardItem_ovb0113 {
    u8 pad_0x0[0x148];
};

static inline void SwitchOff(const u32* id) {
    void* manager = Manager_ovb0113();
    UnkBillboardEntry_ovb0113* entry = Find_ovb0113(manager, *id);
    if (entry && entry->data)
        fn_71001f3224(entry->data->toggle, 0);
}

void fn_71007274f4(UnkBillboardComponent_ovb0113* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    SwitchOff(&self->id);
    SwitchOff(&self->glow_id);
    self->field_0xa8 = -1;
    UnkSoundRef* ref = self->effect.ref;
    if (ref && ref->serial == self->effect.serial && self->effect.serial != -1 && ref->object) {
        fn_7100719894(self->owner ? self->owner->effects : nullptr, self->effect, -1.0f, 0.0f);
        self->effect = g_unk_7108b6c460;
    }
}

// fn_7100727ee4: the update. After the base (fn_71010c424c), with the first entry's data: every item (+0x50,
// 0x148 bytes each, +0x110 of them) is updated (fn_7100745dac), the data is given the entry's resource
// (fn_71001b7128), marked (+0x113) and switched on, and fn_7100728064 runs; the second entry's data then gets
// the first entry's resource the same way.
struct UnkParent;
void fn_7100745dac(UnkBillboardItem_ovb0113* item, f32 dt);
void fn_7100728064(UnkBillboardComponent_ovb0113* self);

void fn_7100727ee4(UnkBillboardComponent_ovb0113* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    void* manager = Manager_ovb0113();
    UnkBillboardEntry_ovb0113* first = Find_ovb0113(manager, self->id);
    if (first && first->data) {
        for (s32 i = 0; i < self->item_count; ++i)
            fn_7100745dac(&self->items[i], dt);
        UnkBillboardData_ovb0113* data = first->data;
        void* resource = first->field_0x18;
        u32 index = first->field_0x4;
        fn_71001b7128(data->field_0x128, resource, index);
        data->field_0x113 = true;
        fn_71001f3224(first->data->toggle, 1);
        fn_7100728064(self);
    }
    manager = Manager_ovb0113();
    UnkBillboardEntry_ovb0113* second = Find_ovb0113(manager, self->glow_id);
    if (first && second && first->data && second->data) {
        UnkBillboardData_ovb0113* data = second->data;
        void* resource = first->field_0x18;
        u32 index = first->field_0x4;
        fn_71001b7128(data->field_0x128, resource, index);
        data->field_0x113 = true;
        fn_71001f3224(second->data->toggle, 1);
    }
}
