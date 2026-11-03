// Unit 1441 of tools/tu_map.py (no name hint): fn_7100969da8 updates the target's mode (+0x6c) and
// distance (+0x74) from the movement at +0x38 and the settings at +0x40, while the movement is active
// (+0x19f8) and not refused by its vtable slots 0x170 and 0x378. Names and layouts are placeholders.
#include "types.h"

struct UnkMovement_71010d0e38;
bool fn_71010d0e38(UnkMovement_71010d0e38* self);
struct UnkStateSet_1441;
bool fn_7101179870(UnkStateSet_1441* self);

struct UnkMovement_1441 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0x19f8 - 0xfa];
    bool active;

    bool vf_0x170() { return reinterpret_cast<bool (*)(UnkMovement_1441*)>(vtable[0x170 / 8])(this); }
    bool vf_0x378() { return reinterpret_cast<bool (*)(UnkMovement_1441*)>(vtable[0x378 / 8])(this); }
};

struct UnkSettings_1441 {
    u8 pad_0x0[0x134];
    bool has_distance;
    u8 pad_0x135[3];
    f32 distance;
};

struct UnkTarget_1441 {
    u8 pad_0x0[0x6c];
    u32 mode;
    u8 pad_0x70[4];
    f32 distance;
};

struct UnkOwner_1441 {
    u8 pad_0x0[0x220];
    UnkStateSet_1441* states;
    u8 pad_0x228[0x10];
    UnkTarget_1441* target;
};

struct UnkUnit1441_b48 {
    u8 pad_0x0[0x30];
    UnkOwner_1441* owner;
    UnkMovement_1441* movement;
    UnkSettings_1441* settings;
};

void fn_7100969da8(UnkUnit1441_b48* self) {
    UnkMovement_1441* movement = self->movement;
    if (!movement || !self->settings || !movement->active)
        return;
    if (movement->vf_0x170() && !self->movement->vf_0x378())
        return;
    UnkOwner_1441* owner = self->owner;
    UnkTarget_1441* target = owner->target;
    if (!target)
        return;
    if (fn_7101179870(owner->states)) {
        target->mode = 2;
        return;
    }
    target->mode = 0;
    movement = self->movement;
    if (movement->field_0xf9 || fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(movement))) {
        UnkSettings_1441* settings = self->settings;
        target->distance = settings->has_distance ? settings->distance : 3.0f;
    } else {
        target->distance = 6.0f;
    }
}
