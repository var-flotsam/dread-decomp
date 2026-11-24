// Unit 2689 (the player movement): fn_71010d9a1c copies the hold and held states of another actor's movement
// into the owner's, then carries over the first of four timeline ids the other has. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkChargeComponent.h"
#include "game/UnkTU2689.h"
#include "types.h"

extern const base::reflection::CClass g_unk_7172a03fd0 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164ced8;
extern const u64 g_unk_710164cee0;
extern const u64 g_unk_710164cee8;
extern const u64 g_unk_710164cef0;
extern f32 g_unk_7101cb0c90;

struct UnkTimelineOwner;
f32 fn_7100a2d4d8(UnkTimelineOwner* self, const u64* key);

struct UnkMoveCopyFrom_l0d9a1c;
void fn_7100976140(UnkMoveCopyFrom_l0d9a1c* self);

struct UnkHoldMovement_l0d9a1c;
void fn_71010d9f88(UnkHoldMovement_l0d9a1c* self);
void fn_71010d80e8(UnkHoldMovement_l0d9a1c* self, u32 kind);
void fn_71010d7d0c(UnkHoldMovement_l0d9a1c* self, u32 kind);
void fn_71010da220(UnkHoldMovement_l0d9a1c* self, f32 a, f32 b, f32 c);

struct UnkHoldMovement_l0d9a1c {
    void** vtable;
    u8 pad_0x8[0x7b3];
    u8 field_0x7bb;
    u8 pad_0x7bc[0x10e8];
    bool holding;  // 0x18a4
    u8 pad_0x18a5[0x3];
    f32 field_0x18a8;
    f32 field_0x18ac;
    bool held;  // 0x18b0
    u8 pad_0x18b1[0x3];
    u32 field_0x18b4;
    u32 field_0x18b8;
};
static_assert(__builtin_offsetof(UnkHoldMovement_l0d9a1c, field_0x7bb) == 0x7bb, "layout");
static_assert(__builtin_offsetof(UnkHoldMovement_l0d9a1c, holding) == 0x18a4, "layout");
static_assert(__builtin_offsetof(UnkHoldMovement_l0d9a1c, held) == 0x18b0, "layout");

struct UnkHoldActor_l0d9a1c {
    u8 pad_0x0[0x1f0];
    UnkHoldMovement_l0d9a1c* movement;  // 0x1f0
    u8 pad_0x1f8[0xf0];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkMoveCopyFrom_l0d9a1c {
    u8 pad_0x0[0x30];
    UnkHoldActor_l0d9a1c* owner;  // 0x30
    u8 pad_0x38[0x783];
    u8 field_0x7bb;
};
static_assert(__builtin_offsetof(UnkMoveCopyFrom_l0d9a1c, field_0x7bb) == 0x7bb, "layout");

// The movement of an actor, when it is one.
static UnkHoldMovement_l0d9a1c* MovementOf(UnkHoldActor_l0d9a1c* actor) {
    UnkHoldMovement_l0d9a1c* movement = actor->movement;
    if (movement) {
        const base::reflection::CClass* cls = reinterpret_cast<base::reflection::UnkReflObject*>(movement)->vf_0x0();
        fn_71010d68bc();
        if (!fn_710009d50c(cls, &g_unk_7172a03fd0))
            movement = nullptr;
    }
    return movement;
}

void fn_71010d9a1c(UnkMoveCopyFrom_l0d9a1c* self, UnkHoldActor_l0d9a1c* other) {
    fn_7100976140(self);
    if (!other)
        return;
    UnkHoldMovement_l0d9a1c* from = MovementOf(other);
    UnkHoldMovement_l0d9a1c* to = MovementOf(reinterpret_cast<UnkHoldActor_l0d9a1c*>(self->owner));
    if (from->holding) {
        fn_71010d9f88(to);
        to->field_0x18a8 = from->field_0x18a8;
        to->field_0x18ac = from->field_0x18ac;
    } else {
        fn_71010d80e8(to, 1);
    }
    if (from->held) {
        const f32* charge = g_unk_71726e0aa8.field_0xd8;
        fn_71010da220(to, charge[0], charge[1],
                      *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172a03d80) + 0x60));
        to->field_0x18b4 = from->field_0x18b4;
        to->field_0x18b8 = from->field_0x18b8;
    } else {
        fn_71010d7d0c(to, 1);
    }
    self->field_0x7bb = from->field_0x7bb;
    UnkTimelineComponent* toTimeline = self->owner->timeline;
    if (!toTimeline)
        return;
    UnkTimelineComponent* fromTimeline = other->timeline;
    if (!fromTimeline)
        return;
    bool done = false;
    // Moves the key from the other's timeline to the owner's when it is there, unless an earlier key already was.
    auto carry = [&](const u64* id) {
        if (fn_7100a2be94(fromTimeline, id)) {
            fn_7100a2b7b8(fromTimeline, id);
            if (!done) {
                if (!(fn_7100a2d4d8(reinterpret_cast<UnkTimelineOwner*>(fromTimeline), id) > g_unk_7101cb0c90)) {
                    base::reflection::UnkReflTypeIdArg value;
                    value.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(toTimeline, id, &value.id, false, -1.0f);
                    done = true;
                }
            }
        }
    };
    carry(&g_unk_710164ced8);
    carry(&g_unk_710164cee0);
    carry(&g_unk_710164cee8);
    carry(&g_unk_710164cef0);
}
