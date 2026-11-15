// Unit 2702: an AI component resets all of its state: the base component, the flags, the pending lists and its
// owner's flag. Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "game/CComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkTU108.h"
#include "types.h"

struct UnkResetAI_l0ff3c8;
struct UnkResetVtable_l0ff3c8 {
    void* pad_0x0[0x5e];
    void (*vf_0x2f0)(UnkResetAI_l0ff3c8* self);
};

struct UnkResetEntry_l0ff3c8;
struct UnkResetEntryVtable_l0ff3c8 {
    void* pad_0x0[0xa];
    void (*vf_0x50)(UnkResetEntry_l0ff3c8* self);
};
struct UnkResetEntry_l0ff3c8 {
    UnkResetEntryVtable_l0ff3c8* vtable;
};

struct UnkResetFlag_l0ff3c8 {
    u8 pad_0x0[0x35c];
    u8 field_0x35c;
};

struct UnkResetPart_l0ff3c8 {
    u8 pad_0x0[0x138];
    UnkResetFlag_l0ff3c8* field_0x138;
};

struct UnkResetSlot_l0ff3c8 {
    u8 pad_0x0[0x50];
    UnkResetPart_l0ff3c8* field_0x50;
};

struct UnkResetOwner_l0ff3c8 {
    u8 pad_0x0[0x228];
    UnkResetSlot_l0ff3c8* field_0x228;
};

// The state at 0x14510 that the component's request sets.
struct UnkResetRequest_l0ff3c8 {
    bool pending;  // 0x0
    u8 pad_0x1[0x3];
    u32 field_0x4;
    u8 pad_0x8[0x10];
    u32 field_0x18;
    u8 pad_0x1c[0x24];
    u32 field_0x40;
};

struct __attribute__((packed, aligned(4))) UnkResetEntryRef_l0ff3c8 {
    UnkResetEntry_l0ff3c8* p;
};

struct UnkResetList_l0ff3c8 {
    u32 count;  // 0x1618
    union {
        UnkResetEntryRef_l0ff3c8 items[31];  // 0x161c
        struct {
            u8 pad_0x0[0xd4];
            u32 field_0x16f0;
            u8 pad_0xd8[0x1c];
            u32 field_0x1710;
        };
    };
    bool field_0x1714;
    bool field_0x1715;
};

struct UnkResetAI_l0ff3c8 {
    UnkResetVtable_l0ff3c8* vtable;
    u8 pad_0x8[0x28];
    UnkResetOwner_l0ff3c8* owner;  // 0x30
    u8 pad_0x38[0x30];
    u8 field_0x68[0x8];
    u32 field_0x70;
    u8 pad_0x74[0x1c];
    u32 field_0x90;
    bool field_0x94;
    bool field_0x95;
    u8 pad_0x96[0x22];
    u32 field_0xb8;
    u8 pad_0xbc[0x1c];
    u8* field_0xd8;  // 4-byte elements
    u32 field_0xe0;
    u8 pad_0xe4[0x1c];
    u32 field_0x100;
    u8 pad_0x104[0x1004];
    u64 field_0x1108;
    u8 pad_0x1110[0x508];
    UnkResetList_l0ff3c8 list;  // 0x1618
    u8 pad_0x1718[0x12df8];
    UnkResetRequest_l0ff3c8 request;  // 0x14510
};
static_assert(__builtin_offsetof(UnkResetAI_l0ff3c8, request) == 0x14510, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0ff3c8, list) == 0x1618, "layout");

void fn_71010ff3c8(UnkResetAI_l0ff3c8* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    self->field_0x70 = 0;
    self->field_0x90 = 0;
    self->field_0x94 = false;
    self->field_0x95 = true;
    self->field_0xb8 = 0;
    for (u64 i = 0; i < self->field_0xe0; ++i)
        fn_71002874dc(self->field_0xd8 + i * 4);
    self->field_0xe0 = 0;
    self->field_0x100 = 0;
    fn_7100119f98(reinterpret_cast<UnkTileOutline*>(self->field_0x68), false);
    self->field_0x1108 = 0;
    fn_7100874460(reinterpret_cast<u8*>(self) + 0x18a8, reinterpret_cast<UnkWeakTarget**>(&self->owner));
    self->list.field_0x16f0 = 0;
    self->list.field_0x1710 = 0;
    self->list.field_0x1714 = false;
    self->list.field_0x1715 = true;
    bool pending = self->request.pending;
    self->request.field_0x18 = 0;
    self->request.field_0x40 = 0;
    if (pending) {
        self->request.pending = false;
        self->request.field_0x4 = 0;
        self->vtable->vf_0x2f0(self);
    }
    for (UnkResetEntryRef_l0ff3c8* it = self->list.items; it != self->list.items + self->list.count; ++it)
        it->p->vtable->vf_0x50(it->p);
    UnkResetSlot_l0ff3c8* slot = self->owner->field_0x228;
    if (slot && slot->field_0x50)
        slot->field_0x50->field_0x138->field_0x35c = 1;
}
