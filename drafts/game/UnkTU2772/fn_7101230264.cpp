// nn::atk unit: takes a voice slot from the sound system's pool, sets its priority, links it with this
// handle both ways and resets the handle's parameters to their defaults. Names and layouts are placeholders.
#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkSlot;
struct UnkAtkSlotPool;
struct UnkAtkVoice_b48a3;
UnkAtkSlot* fn_7101230118(UnkAtkSlotPool* pool);
void fn_710122f5a4(UnkAtkVoice_b48a3* self, s32 priority);

struct UnkAtkParams_l230264 {
    f32 values[24];
};

__attribute__((visibility("hidden"))) extern const UnkAtkParams_l230264 g_unk_7101653990;

struct UnkAtkHandle_l230264;

// The slot as this function sees it: its back link to the handle that owns it.
struct UnkAtkSlotOwner_l230264 {
    u8 pad_0x00[0xe0];
    UnkAtkHandle_l230264* owner;
};

struct UnkAtkHandle_l230264 {
    s32 priority;
    u32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    UnkAtkParams_l230264 params;
    u64 field_0x70;
    u32 field_0x78;
    u16 field_0x7c;
    u8 field_0x7e;
    u8 pad_0x7f;
    u32 field_0x80;
    u32 field_0x84;
    u8 pad_0x88[0x20];
    u64 field_0xa8;
    u8 pad_0xb0[8];
    u64 field_0xb8;
    u8 pad_0xc0[0x18];
    UnkAtkSlotOwner_l230264* slot;
};
static_assert(__builtin_offsetof(UnkAtkHandle_l230264, slot) == 0xd8, "slot offset");

bool fn_7101230264(UnkAtkHandle_l230264* self, s32 priority) {
    UnkAtkSlotPool* pool = reinterpret_cast<UnkAtkSlotPool*>(static_cast<u8*>(fn_71001039f8()) + 0x530);
    UnkAtkSlot* slot = fn_7101230118(pool);
    if (slot == nullptr)
        return false;
    fn_710122f5a4(reinterpret_cast<UnkAtkVoice_b48a3*>(slot), priority);
    UnkAtkSlotOwner_l230264* owned = reinterpret_cast<UnkAtkSlotOwner_l230264*>(slot);
    owned->owner = self;
    self->slot = owned;
    self->priority = priority;
    self->field_0x4 = 1;
    self->params = g_unk_7101653990;
    self->field_0x8 = 1.0f;
    self->field_0xc = 1.0f;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x7c = 0;
    self->field_0x7e = 0;
    self->field_0x80 = 1;
    self->field_0x84 = 32000;
    self->field_0xa8 = 0;
    self->field_0xb8 = 0;
    return true;
}
