// CChozoWarriorAIComponent unit (tools/tu_map.py, the name from reflection): two positions along one axis,
// a base value (fn_71010fc284 / fn_71010fc31c) plus or minus the component's vf_0x3c8 less half the
// tracked object's reach radius (fn_7100977b90). Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/UnkTU2702.h"
#include "types.h"

// As src/game/CAIComponent/CAIComponent_ReachRadius.cpp defines it.
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkChozoWarriorOwner_b66a2 {
    u8 pad_0x0[0x1f8];
    void* tracked;
};

struct UnkChozoWarriorAI_b66a2;

struct UnkChozoWarriorAIVtbl_b66a2 {
    u8 pad_0x0[0x3c8];
    f32 (*vf_0x3c8)(UnkChozoWarriorAI_b66a2* self);
};

struct UnkChozoWarriorAI_b66a2 {
    UnkChozoWarriorAIVtbl_b66a2* vtable;
    u8 pad_0x8[0x28];
    UnkChozoWarriorOwner_b66a2* owner;
};

f32 fn_710049b2fc(UnkChozoWarriorAI_b66a2* self) {
    f32 base;
    fn_71010fc284(self, &base);
    UnkAIReach reach = fn_7100977b90(self->owner->tracked);
    f32 offset = self->vtable->vf_0x3c8(self) + reach.radius * -0.5f;
    return base + offset;
}

f32 fn_710049b360(UnkChozoWarriorAI_b66a2* self) {
    f32 base;
    fn_71010fc31c(self, &base);
    UnkAIReach reach = fn_7100977b90(self->owner->tracked);
    f32 offset = self->vtable->vf_0x3c8(self) + reach.radius * -0.5f;
    return base - offset;
}
