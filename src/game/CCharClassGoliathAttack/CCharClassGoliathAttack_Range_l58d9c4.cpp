// CCharClassGoliathAttack unit: fn_710058d9c4, a distance the attack wants to keep from its target.
// Zero unless the actor's AI component is a CGoliathAIComponent with a value; the attack's own
// distance (or the tunable's fallback) bounds the result. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassGoliathAttack.h"
#include "game/CCharacterMovement.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkGoliathAI_l58d9c4 {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGoliathAI_l58d9c4*)>(vtable[0])(this);
    }
    f32 vf_0x3c8() {
        return reinterpret_cast<f32 (*)(UnkGoliathAI_l58d9c4*)>(vtable[0x3c8 / 8])(this);
    }
};

struct UnkGoliathTunables_gla {
    u8 pad_0x0[0x28];
    f32 field_0x28;
    f32 field_0x2c;
};

struct UnkGoliathAttack_l58d9c4 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

struct UnkGoliathActor_l58d9c4 {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
    u8 pad_0x200[0x240 - 0x200];
    UnkGoliathAI_l58d9c4* field_0x240;
};

bool fn_7100591b14(UnkGoliathAI_l58d9c4* ai);

f32 fn_710058d9c4(UnkGoliathAttack_l58d9c4* self, UnkGoliathActor_l58d9c4* actor) {
    f32 value;
    if (actor) {
        UnkGoliathAI_l58d9c4* ai = actor->field_0x240;
        if (ai && fn_710009d50c(ai->vf_0x0(), (const base::reflection::CType*)fn_71005908ac())) {
            value = -1.0f;
            if (!fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &value))
                return 0.0f;
            UnkGoliathTunables_gla* t = &g_unk_71726e5e70;
            // picks between the two bit patterns, as the original does
            f32 mine = self->field_0x44;
            u32 mine_bits, fallback_bits;
            __builtin_memcpy(&mine_bits, &mine, 4);
            __builtin_memcpy(&fallback_bits, &t->field_0x28, 4);
            u32 base_bits = mine < 0.0f ? fallback_bits : mine_bits;
            if (value + 10.0f >= 0.0f && value - 10.0f <= 0.0f) {
                UnkAIReach reach = fn_7100977b90(actor->field_0x1f8);
                f32 radius = reach.radius;
                f32 extra = ai->vf_0x3c8();
                f32 result = __builtin_fmaxf(radius, extra);
                result += t->field_0x2c;
                f32 base;
                __builtin_memcpy(&base, &base_bits, 4);
                return result < base ? base : result;
            }
            if (fn_7100591b14(ai))
                return t->field_0x2c;
            f32 base;
            __builtin_memcpy(&base, &base_bits, 4);
            return base;
        }
    }
    value = -1.0f;
    return 0.0f;
}
