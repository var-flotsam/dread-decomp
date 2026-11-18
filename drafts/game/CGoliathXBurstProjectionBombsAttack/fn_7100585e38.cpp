// CGoliathXBurstProjectionBombsAttack's unit (tools/tu_map.py, unit 835): picks the id the owner's AI
// component stores at +0x1780 for what follows the attack: g_unk_71726e55c8 when this attack was the one
// before the current one, or when the target is ahead and +0x64 is set; otherwise, four times in ten, the
// definition's id (+0x28, +0x38). Returns 0. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

#include <stdlib.h>

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern __attribute__((visibility("hidden"))) CStrId g_unk_71726e55c8;

// A node of the AI component's list of attacks.
struct UnkAttackNode_l585e38 {
    u8 pad_0x0[0x8];
    void* attack;  // 0x8
    u8 pad_0x10[0x8];
    UnkAttackNode_l585e38* prev;  // 0x18
    UnkAttackNode_l585e38* next;  // 0x20
};

struct UnkAIComponent_l585e38 : UnkReflObject {
    u8 pad_0x8[0x1168];
    u32 count;                    // 0x1170
    UnkAttackNode_l585e38* last;  // 0x1178
    UnkAttackNode_l585e38* first;  // 0x1180
    u8 pad_0x1188[0x5f8];
    CStrId next_id;  // 0x1780

    UnkAttackNode_l585e38* At(u32 index) {
        UnkAttackNode_l585e38* node = nullptr;
        if (index * 2 < count) {
            UnkAttackNode_l585e38** link = &first;
            for (u32 steps = index + 1; steps != 0; --steps) {
                node = *link;
                link = &node->next;
            }
        } else {
            node = last;
            for (u32 steps = count - index; steps > 1; --steps)
                node = node->prev;
        }
        return node;
    }
};
static_assert(__builtin_offsetof(UnkAIComponent_l585e38, next_id) == 0x1780, "next_id");

struct UnkBombsDef_l585e38 {
    u8 pad_0x0[0x38];
    CStrId next_id;  // 0x38
};

struct UnkBombsAttack_l585e38 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkBombsDef_l585e38* def;  // 0x28
    u8 pad_0x30[0x34];
    bool field_0x64;
};

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

f32 fn_7100585e38(UnkBombsAttack_l585e38* self) {
    UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_71010f6b50()))
        return 0.0f;
    UnkAIComponent_l585e38* ai = static_cast<UnkAIComponent_l585e38*>(object);
    const CStrId* id;
    if (ai->count >= 2 && ai->At(ai->count - 2)->attack == self) {
        id = &g_unk_71726e55c8;
    } else {
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        f32 fx = m.m[2];
        f32 fy = m.m[6];
        f32 fz = m.m[10];
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkActor* actor = self->owner;
        f32 ahead = fx * (target->field_0x44 - actor->field_0x44) + fy * (target->field_0x54 - actor->field_0x54) +
                    fz * (target->field_0x64 - actor->field_0x64);
        if ((ahead < 0.0f || !self->field_0x64) && Random01() <= 0.4f)
            id = &self->def->next_id;
        else
            id = &g_unk_71726e55c8;
    }
    ai->next_id = *id;
    return 0.0f;
}
