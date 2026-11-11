// Unit 1443 of tools/tu_map.py (no name hint): fn_7100971358 updates the target's mode (+0x6c) and
// distance (+0x74) like unit 1441's fn_7100969da8 (src/game/UnkUnit1441/UnkUnit1441_b48.cpp), then
// steps the movement at +0x38 (its vtable slot 0x310). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "types.h"

struct UnkMovement_71010d0e38;
struct UnkStateSet_1441;

struct UnkMovement_1443 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    bool vf_0x170() { return reinterpret_cast<bool (*)(UnkMovement_1443*)>(vtable[0x170 / 8])(this); }
    bool vf_0x378() { return reinterpret_cast<bool (*)(UnkMovement_1443*)>(vtable[0x378 / 8])(this); }
    void vf_0x310(f32 dt) { reinterpret_cast<void (*)(UnkMovement_1443*, f32)>(vtable[0x310 / 8])(this, dt); }
};

struct UnkSettings_1443 {
    u8 pad_0x0[0x134];
    bool has_distance;
    u8 pad_0x135[3];
    f32 distance;
};

struct UnkTarget_1443 {
    u8 pad_0x0[0x6c];
    u32 mode;
    u8 pad_0x70[4];
    f32 distance;
};

struct UnkOwner_1443 {
    u8 pad_0x0[0x220];
    UnkStateSet_1441* states;
    u8 pad_0x228[0x10];
    UnkTarget_1443* target;
};

struct UnkUnit1443 {
    u8 pad_0x0[0x30];
    UnkOwner_1443* owner;
    UnkMovement_1443* movement;
    UnkSettings_1443* settings;
};

static inline void UpdateTarget(UnkUnit1443* self) {
    if (self->movement->vf_0x170() && !self->movement->vf_0x378())
        return;
    UnkOwner_1443* owner = self->owner;
    UnkTarget_1443* target = owner->target;
    if (!target)
        return;
    if (fn_7101179870(owner->states)) {
        target->mode = 2;
        return;
    }
    target->mode = 0;
    UnkMovement_1443* movement = self->movement;
    if (movement->field_0xf9 || fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(movement))) {
        UnkSettings_1443* settings = self->settings;
        target->distance = settings->has_distance ? settings->distance : 3.0f;
    } else {
        target->distance = 6.0f;
    }
}

void fn_7100971358(UnkUnit1443* self, f32 dt) {
    UpdateTarget(self);
    self->movement->vf_0x310(dt);
}
