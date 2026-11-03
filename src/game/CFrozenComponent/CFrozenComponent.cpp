// CFrozenComponent: starts the frozen state (fn_71007fa0c4). Stores the remaining time (+0x58 minus
// +0x5c of the definition at +0x2d8) at +0x40 and applies it (fn_71007fa144), hands the actor (+0x30)
// the definition's timeline id (+0x60, +0x30; fn_7100d7c828) and, when the actor has a movement
// component (+0x218), stops it with a unit-local zero vector (fn_71010d08d4). Names and layouts are
// placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

// Declared as src/game/CMovementComponent/fn_71010d08d4.cpp defines it.
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start);

extern const u8 g_unk_710164aba8[] __attribute__((visibility("hidden")));

struct UnkFrozenTimeline {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFrozenDef {
    u8 pad_0x0[0x58];
    f32 duration;
    f32 elapsed;
    UnkFrozenTimeline* timeline;
};

struct UnkFrozenActor {
    u8 pad_0x0[0x218];
    void* movement;
};

struct UnkFrozenComponent {
    u8 pad_0x0[0x30];
    UnkFrozenActor* actor;
    u8 pad_0x38[8];
    f32 field_0x40;
    u8 pad_0x44[0x294];
    UnkFrozenDef* def;
};

void fn_71007fa144(UnkFrozenComponent* self, f32 time);

void fn_71007fa0c4(UnkFrozenComponent* self) {
    f32 time = self->def->duration - self->def->elapsed;
    self->field_0x40 = time;
    fn_71007fa144(self, time);
    UnkActor* actor = reinterpret_cast<UnkActor*>(self->actor);
    u64 id = self->def->timeline->id;
    fn_7100d7c828(actor, &id, false);
    if (void* movement = self->actor->movement)
        fn_71010d08d4(static_cast<UnkMovement_71010d08d4*>(movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(g_unk_710164aba8), false);
}
