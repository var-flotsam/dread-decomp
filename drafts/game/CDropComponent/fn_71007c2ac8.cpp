// CDropComponent's unit (tools/tu_map.py): marks which of eight directions around the actor a drop
// may not take, from the movement's facing and from a side the parameters choose relative to the
// actor's axes. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCoreXAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU55.h"
#include <math.h>
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkVec3;

// The angle between two neighbouring directions.
extern f32 g_unk_7101a715b8;

// What fn_71004dad10 returns for the actor: its drop component is at +0x10.
struct UnkDropRecord_l7c2ac8 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;
};

// The drop component as far as this function reads it.
struct UnkDropComponent_l7c2ac8 {
    u8 pad_0x0[0x5b];
    u8 field_0x5b;  // the movement's facing blocks directions
};

struct UnkDropMoveState_l7c2ac8 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

// The movement at the actor's +0x218.
struct UnkDropMovement_l7c2ac8 {
    u8 pad_0x0[0x128];
    UnkDropMoveState_l7c2ac8* field_0x128;
    u8 pad_0x130[0x18];
    CVector3D field_0x148;
    u8 pad_0x154[0x10];
    u8 field_0x164;
    u8 pad_0x165[0x87f];
    f32 field_0x9e4;
    f32 field_0x9e8;
};
static_assert(__builtin_offsetof(UnkDropMovement_l7c2ac8, field_0x9e4) == 0x9e4, "layout");

struct UnkDropParams_l7c2ac8 {
    u8 pad_0x0[0x80];
    u32 side;  // 0: none; 1 to 8: which side of the actor
    u8 pad_0x84[0x4];
    bool narrow;
};

static inline CVector3D Column(const UnkMatrix44& m, int i) {
    CVector3D v;
    v.x = m.m[i];
    v.y = m.m[i + 4];
    v.z = m.m[i + 8];
    return v;
}

static inline CVector3D Negated(const CVector3D& v) {
    CVector3D r;
    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;
    return r;
}

static inline bool IsDoorShield(UnkActor* actor) {
    UnkReflObject* life = static_cast<UnkReflObject*>(actor->field_0x1b8);
    return life && fn_710009d50c(life->vf_0x0(), fn_71008ddcf0());
}

// The way the actor faces: a door shield looks along its negated x axis, anything else along z.
static inline CVector3D Forward(UnkActor* actor) {
    if (IsDoorShield(actor))
        return Negated(Column(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)), 0));
    return Column(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)), 2);
}

static inline CVector3D Up(UnkActor* actor) {
    return Column(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)), 1);
}

static inline void Set(CVector2D* out, const CVector3D& v) {
    fn_710007b444(out, reinterpret_cast<const UnkVec3*>(&v));
}

static inline CVector2D Project(const CVector3D& v) {
    CVector2D r;
    fn_710007b430(&r, &v);
    return r;
}

static inline void Normalize(CVector2D& v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y);
    if (len != 0.0f) {
        v.x /= len;
        v.y /= len;
    }
}

void fn_71007c2ac8(UnkActor* self, UnkDropParams_l7c2ac8* params, s32* blocked) {
    UnkDropMovement_l7c2ac8* movement = static_cast<UnkDropMovement_l7c2ac8*>(self->field_0x218);
    bool hasFacing = false;
    CVector2D facing;
    UnkDropRecord_l7c2ac8* record = reinterpret_cast<UnkDropRecord_l7c2ac8*>(fn_71004dad10(self));
    UnkReflObject* component;
    if (record && (component = record->component) &&
        fn_710009d50c(component->vf_0x0(), fn_71007be344())) {
        UnkDropComponent_l7c2ac8* drop = reinterpret_cast<UnkDropComponent_l7c2ac8*>(component);
        facing.x = 0.0f;
        facing.y = 0.0f;
        if (movement && drop->field_0x5b) {
            if (fn_71010cc3a4(movement) == 1) {
                facing.x = -movement->field_0x9e4;
                facing.y = -movement->field_0x9e8;
                hasFacing = true;
            } else if (movement->field_0x128 && movement->field_0x128->field_0x10 &&
                       (movement->field_0x164 & 0x20)) {
                Set(&facing, movement->field_0x148);
                hasFacing = true;
            }
        }
    } else {
        facing.x = 0.0f;
        facing.y = 0.0f;
    }

    u32 side = params->side;
    CVector2D dir;
    dir.x = 0.0f;
    dir.y = 0.0f;
    if (side == 0 && !hasFacing) {
        for (int i = 0; i < 8; i++)
            blocked[i] = 0;
        return;
    }
    {
        switch (side) {
        case 1:
            Set(&dir, Forward(self));
            break;
        case 2:
            Set(&dir, Negated(Forward(self)));
            break;
        case 3:
            Set(&dir, Up(self));
            break;
        case 4:
            Set(&dir, Negated(Up(self)));
            break;
        case 5: {
            Set(&dir, Forward(self));
            CVector2D up = Project(Up(self));
            dir.x += up.x;
            dir.y += up.y;
            Normalize(dir);
            break;
        }
        case 6: {
            Set(&dir, Forward(self));
            CVector2D up = Project(Up(self));
            dir.x -= up.x;
            dir.y -= up.y;
            Normalize(dir);
            break;
        }
        case 7: {
            Set(&dir, Negated(Forward(self)));
            CVector2D up = Project(Up(self));
            dir.x += up.x;
            dir.y += up.y;
            Normalize(dir);
            break;
        }
        case 8: {
            Set(&dir, Negated(Forward(self)));
            CVector2D up = Project(Up(self));
            dir.x -= up.x;
            dir.y -= up.y;
            Normalize(dir);
            break;
        }
        }

        for (int i = 0; i < 8; i++) {
            f32 angle = g_unk_7101a715b8 * (f32)i;
            f32 c = cosf(angle);
            f32 s = sinf(angle);
            if (hasFacing && c * facing.x + s * facing.y > 0.7f)
                blocked[i] = -1;
            else if (side != 0 && c * dir.x + s * dir.y < (params->narrow ? 0.8f : 0.1f))
                blocked[i] = -1;
            else
                blocked[i] = 0;
        }
    }
}
