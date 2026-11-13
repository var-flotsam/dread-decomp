#pragma once

// The X parasite AI component's state block at +0x146e0 and the objects it links to (the grabbed object, its
// listener and its partner), as the two sources of unit 1102 that step and pick the state see them. They are
// one more view of the component of game/UnkXParasiteAI.h, not merged with it yet (docs/JOURNAL.md,
// 2026-11-13). Names and layouts are placeholders.
#include "types.h"

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
