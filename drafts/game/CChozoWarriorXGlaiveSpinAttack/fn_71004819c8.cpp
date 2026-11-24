// CChozoWarriorXGlaiveSpinAttack's unit (tools/tu_map.py unit 715): an event handler. On a kind-1 event whose
// source carries a fixed hash, with a target (fn_7100d7c990) and a character class, the owner either goes on
// spinning (when it faces the side its target is on) or first turns: its navigation is handed a parameter block
// (kind 5) pointing back along its facing, and the turn track is sent instead. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

// The direction the owner looks at its target from, by the side the target is on.
struct UnkSpinSide_l4819c8 {
    f32 x, y, z;
};
__attribute__((visibility("hidden"))) extern const UnkSpinSide_l4819c8 g_unk_7101649850;  // target on the right
__attribute__((visibility("hidden"))) extern const UnkSpinSide_l4819c8 g_unk_710164985c;  // target on the left

struct UnkSpinHashed_l4819c8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkSpinSourceInner_l4819c8 {
    u8 pad_0x0[0x38];
    UnkSpinHashed_l4819c8* field_0x38;
};

struct UnkSpinSource_l4819c8 {
    u8 pad_0x0[0x10];
    UnkSpinSourceInner_l4819c8* field_0x10;
};

struct UnkSpinEvent_l4819c8 {
    s32 kind;
    UnkSpinSource_l4819c8* source;  // 0x8
};

struct UnkSpinAttack_l4819c8 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

// The parameter block of kind 5 (game_s2/UnkParams.h) with a direction.
struct UnkSpinParams_l4819c8 {
    void** vtable;
    f32 x, y, z;         // 0x8
    bool has_direction;  // 0x14
    u8 pad_0x15[3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkSpinParams_l4819c8, kind) == 0x34, "layout");

static const u64 kKind = 0xf4061ff25a2163d6;

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

bool fn_71004819c8(UnkSpinAttack_l4819c8* self, const UnkSpinEvent_l4819c8* event) {
    // At function scope (the original shares none of their stack slots; ours still shares two).
    base::reflection::UnkReflTypeIdArg spin_mode, spin_mask;
    UnkSpinParams_l4819c8 params;
    base::reflection::UnkReflTypeIdArg turn_mode, turn_mask;
    if (event->kind != 1 || event->source->field_0x10->field_0x38->hash != 0xedd10d119562e460)
        return true;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target || !self->owner->field_0x220)
        return true;

    const UnkMatrix44& m = Matrix(self->owner);
    const UnkSpinSide_l4819c8* side =
        target->field_0x44 > self->owner->field_0x44 ? &g_unk_7101649850 : &g_unk_710164985c;
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    if (fx * side->x + fy * side->y + fz * side->z > 0.0f) {
        void* cls = self->owner->field_0x220;
        spin_mode.id = 0;
        spin_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(cls, 0xffb95702ebd47524, kKind, &spin_mode, &g_unk_710164b958, &g_unk_7172769740,
                      &spin_mask);
        return true;
    }

    if (UnkAIComponent* ai = static_cast<UnkAIComponent*>(self->owner->field_0x240)) {
        if (UnkNavigation* navigation = fn_71010fccf8(ai)) {
            params.x = 0.0f;
            params.y = 0.0f;
            params.z = 0.0f;
            params.has_direction = false;
            params.field_0x30 = false;
            params.field_0x20 = 0;
            params.field_0x28 = 0;
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = false;
            params.field_0x3b = false;
            params.field_0x3d = false;
            params.kind = 5;
            params.field_0x18 = 0;
            params.field_0x1c = true;
            const UnkMatrix44& facing = Matrix(self->owner);
            f32 bx = -facing.m[2];
            f32 by = -facing.m[6];
            f32 bz = -facing.m[10];
            params.x = bx;
            params.y = by;
            params.z = bz;
            params.has_direction = true;
            fn_71006f4b0c(navigation, &params, 1);
        }
    }
    void* cls = self->owner->field_0x220;
    turn_mode.id = 0;
    turn_mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(cls, 0xcd64baacf1736baf, kKind, &turn_mode, &g_unk_710164b958, &g_unk_7172769740, &turn_mask);
    return true;
}
