// CShakernautAIComponent unit (tools/tu_map.py): a reflected property setter handing a copy of the
// CVector3D argument (inline, or behind a pointer for 9 or more entries or kind 1) to fn_71006b1ddc
// with the object at 8. Returns an empty result of kind 2. fn_71006b1d64 picks one of four ids by the two
// flags at +0x1488c; fn_71006b0e98 is the per-frame update that picks the attack mode, and fn_71006b1f04
// whether the target stands above the movement's limit. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/math/CVector.h"
#include "game/CAttackComponent.h"
#include "game/CComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkValueSet_sna {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_sna {
    void* holder;
    UnkValueSet_sna* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_sna {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkShakernaut_sna;

struct UnkOwner_sna {
    u8 unk_0x0[8];
    UnkShakernaut_sna* target;
};

void fn_71006b1ddc(UnkShakernaut_sna* self, const base::math::CVector3D* v);

UnkResult_0x18_sna fn_71006b22a4(void*, UnkOwner_sna* owner, UnkValueSource_sna** source) {
    UnkValueSource_sna* src = *source;
    UnkShakernaut_sna* target = owner->target;
    const base::math::CVector3D* p;
    if (src->kind == 1 || src->set->count >= 9)
        p = static_cast<const base::math::CVector3D*>(src->holder);
    else
        p = reinterpret_cast<const base::math::CVector3D*>(src);
    base::math::CVector3D v;
    v.x = p->x;
    v.y = p->y;
    v.z = p->z;
    fn_71006b1ddc(target, &v);
    UnkResult_0x18_sna result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

extern base::global::CStrId g_unk_7172706478 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172706480 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172706488 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172706490 __attribute__((visibility("hidden")));

struct UnkShakernautFlags_sna {
    u8 pad_0x0[0x1488c];
    bool field_0x1488c;
    bool field_0x1488d;
};

base::global::CStrId fn_71006b1d64(UnkShakernautFlags_sna* self) {
    if (self->field_0x1488c) {
        if (self->field_0x1488d)
            return g_unk_7172706478;
        return g_unk_7172706480;
    }
    if (self->field_0x1488d)
        return g_unk_7172706488;
    return g_unk_7172706490;
}

struct UnkParent;
bool fn_71009e12d0(void* obj);

struct UnkShakernautParams_sna {
    u8 pad_0x0[0x720];
    f32 near_range;  // 0x720
    f32 threshold;  // 0x724
};

struct UnkShakernautState_sna {
    UnkShakernautParams_sna* params;  // 0x147f8
    u32 mode;  // 0x14800
    u8 pad_0xc[8];
    f32 value;  // 0x1480c
};

struct UnkShakernautActor_sna {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x210];
    void* field_0x258;
};

struct UnkShakernautAI_sna {
    u8 pad_0x0[0x30];
    UnkShakernautActor_sna* actor;  // 0x30
    u8 pad_0x38[7];
    bool field_0x3f;
    u8 pad_0x40[0x147f8 - 0x40];
    UnkShakernautState_sna state;  // 0x147f8
};

void fn_71006b0f58(UnkShakernautAI_sna* self);
void fn_71006ae948(UnkShakernautAI_sna* self);

void fn_71006b0e98(UnkShakernautAI_sna* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!fn_7101136308(self->actor->field_0x258)) {
        UnkShakernautState_sna* state = &self->state;
        UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor));
        u32 mode;
        if (target && target->field_0x208 && fn_71009e12d0(target->field_0x208)) {
            mode = 2;
        } else if (!self->field_0x3f) {
            mode = 2;
        } else {
            f32 dx = __builtin_fabsf(self->actor->x - reinterpret_cast<UnkShakernautActor_sna*>(target)->x);
            if (dx < state->params->near_range)
                mode = state->value >= state->params->threshold ? 2 : 1;
            else
                mode = state->value >= state->params->threshold ? 2 : 0;
        }
        state->mode = mode;
        fn_71006b0f58(self);
    }
    fn_71006ae948(self);
}

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

UnkActorPosition fn_7100d7b7a8(UnkActor* actor);

struct UnkEnemyMovement_sna {
    void** vtable;
    u8 pad_0x8[0x970];
    f32 limit;  // 0x978
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEnemyMovement_sna*)>(vtable[0])(this);
    }
};

struct UnkShakernautActor2_sna {
    u8 pad_0x0[0x1f0];
    UnkEnemyMovement_sna* movement;  // 0x1f0
};

struct UnkShakernautAI2_sna {
    u8 pad_0x0[0x30];
    UnkShakernautActor2_sna* actor;  // 0x30
};

bool fn_71006b1f04(UnkShakernautAI2_sna* self) {
    UnkEnemyMovement_sna* movement = self->actor->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) && fn_71010cc3a4(movement) == 1) {
        f32 limit = movement->limit;
        UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor));
        base::math::CVector2D p;
        UnkActorPosition pos = fn_7100d7b7a8(target);
        fn_710007b430(&p, reinterpret_cast<const base::math::CVector3D*>(&pos));
        if (limit < p.y)
            return true;
    }
    return false;
}
