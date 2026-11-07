// The CGoliathXSlamAttack unit: fn_7100586d0c, another update. After the base update (fn_710058e848),
// when the owner (+0x8) has its AI component (+0x240, a CGoliathAIComponent) and animation (+0x220) and
// the animation is playing "attack_slam_walk" (fn_710117ae38): when the actor it acts against is behind
// the owner (its offset against the owner's facing, matrix column 2, is negative), sends the animation a
// one under the id of the animation's own name (+0x2b8); otherwise, unless the AI's value
// (fn_71010fc68c) is above its +0x14a28 plus 10, sends a one under the id fn_71003627f8 builds and keeps
// in +0x78 whether the send failed. Descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkGoliathAI_b58a3;
void fn_71010fc68c(UnkGoliathAI_b58a3* ai, f32* out);
struct UnkGoliathXSlamAttack_b58a3;
CStrId fn_71003627f8(UnkGoliathXSlamAttack_b58a3* self, UnkActor* owner);

struct UnkGoliathAI_b59a3 {
    void** vtable;
    u8 pad_0x8[0x14a28 - 0x8];
    f32 field_0x14a28;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGoliathAI_b59a3*)>(vtable[0])(this);
    }
};

struct UnkGoliathXSlamWalk_b59a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x68];
    bool field_0x78;
};

struct Vec_b59a3 {
    f32 x, y, z;
};

static inline Vec_b59a3 operator-(const Vec_b59a3& a, const Vec_b59a3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline f32 Dot_b59a3(const Vec_b59a3& a, const Vec_b59a3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

__attribute__((always_inline)) static inline Vec_b59a3 Pos_b59a3(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_710058e848(UnkGoliathXSlamWalk_b59a3* self);

static inline u64 Hash_b59a3(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100586d0c(UnkGoliathXSlamWalk_b59a3* self, f32 dt) {
    reinterpret_cast<void (*)(UnkGoliathXSlamWalk_b59a3*, f32)>(fn_710058e848)(self, dt);
    auto* comp = static_cast<UnkGoliathAI_b59a3*>(self->owner->field_0x240);
    if (!comp)
        return;
    bool is_goliath = fn_710009d50c(comp->vf_0x0(), fn_71005908ac());
    UnkGoliathAI_b59a3* ai = is_goliath ? comp : nullptr;
    if (!is_goliath)
        return;
    auto* target = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!target)
        return;
    if (!fn_710117ae38(target, fn_7100001570("attack_slam_walk", 16), false))
        return;
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    {
        UnkActor* other = fn_7100d7c990(self->owner);
        if (!other)
            goto ahead;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        Vec_b59a3 facing = {m.m[2], m.m[6], m.m[10]};
        Vec_b59a3 d = Pos_b59a3(other) - Pos_b59a3(self->owner);
        if (!(Dot_b59a3(facing, d) < 0.0f))
            goto ahead;
    }
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    f32 value;
    {
        u64 hash = *reinterpret_cast<u64*>(*reinterpret_cast<char**>(reinterpret_cast<char*>(target) + 0x2b8) + 0x30);
        mode1.id = 1;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740, &mask1);
        return;
    }
ahead:
    value = 0.0f;
    fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &value);
    if (!(value <= ai->field_0x14a28 + 10.0f))
        return;
    {
        CStrId id = fn_71003627f8(reinterpret_cast<UnkGoliathXSlamAttack_b58a3*>(self), self->owner);
        base::reflection::UnkReflTypeIdArg mode2;
        base::reflection::UnkReflTypeIdArg mask2;
        u64 hash = Hash_b59a3(id);
        mode2.id = 1;
        mask2.id = UnkAll_SpitSend().value;
        self->field_0x78 =
            !fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
}
