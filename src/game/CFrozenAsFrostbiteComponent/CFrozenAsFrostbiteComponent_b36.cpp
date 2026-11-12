// Functions in the CFrozenAsFrostbiteComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkStruct_71001436f4;
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);
extern const base::math::CVector4D g_unk_710164ab98 __attribute__((visibility("hidden")));

struct UnkFrostbiteDefHolder {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFrostbiteDef {
    u8 pad_0x0[0xb8];
    UnkFrostbiteDefHolder* holder;
};

struct UnkFrostbiteSlotsOwner {
    u8 pad_0x0[0x50];
    UnkStruct_71001436f4* slots;
};

struct __attribute__((packed, aligned(4))) UnkFrozenAsFrostbite {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x378];
    UnkFrostbiteDef* def;  // 0x3b0
    s32 count;             // 0x3b8
    u64 field_0x3bc;
};

static inline void AddKey(UnkActor* actor, u64 key) {
    fn_7100d7c828(actor, &key, false);
}

void fn_71007fa080(UnkFrozenAsFrostbite* self, u32 state);  // the base method

// After the base method: while frozen (+0x3b8 positive), gives the actor's timeline the
// definition's id; then clears the count and +0x3bc and runs request 2 on the actor's slots.
void fn_71007f4860(UnkFrozenAsFrostbite* self, u32 state) {
    fn_71007fa080(self, state);
    if (self->count >= 1) {
        AddKey(self->actor, self->def->holder->id);
    }
    self->count = 0;
    self->field_0x3bc = 0;
    UnkFrostbiteSlotsOwner* owner = static_cast<UnkFrostbiteSlotsOwner*>(self->actor->field_0x228);
    fn_71001436f4(owner->slots, 2, &g_unk_710164ab98, 1);
}
