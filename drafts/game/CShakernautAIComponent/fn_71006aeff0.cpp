// CShakernautAIComponent unit (tools/tu_map.py unit 1047): after the base step (fn_7101101c44), either turns
// the movement around (a pending turn when no sub-area check is on: the effect, the navigation query and the
// movement's own facing are all set to the reverse of its facing) or, while a check is pending, looks up
// whether the owner stands in the sub-area manager's current area and then tries the move. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2577.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern bool g_unk_7172706401;  // reached through the GOT

struct UnkObjMgrB_b189;
struct UnkObjB_b189;
struct UnkRecB_b189;
bool fn_7100ee2bd8(UnkObjMgrB_b189* self, base::global::CRntVector<UnkRecB_b189*>* out, const CVector2D* pos,
                   u32 mask, UnkObjB_b189* skip);

struct UnkShakernautAI_b65a4;
bool fn_71006af848(UnkShakernautAI_b65a4* self);

void fn_7101101c44(void* self, bool flag);

// A direction that may be unset: three floats and a flag, cleared as 13 bytes.
struct UnkOptDir_l6aeff0 {
    UnkVec3f value;
    bool set;
    u8 pad_0xd[3];

    void Clear() { __builtin_memset(this, 0, 13); }
};

// The block handed to the movement's effect sink (fn_71010c7e6c), with the direction at +0x18.
struct UnkEffectParams_l6aeff0 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    UnkOptDir_l6aeff0 dir;  // 0x18
    UnkOptDir_l6aeff0 field_0x28;
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

// The query handed to the navigation object (+0x298 of the owner), with the direction at +0x8.
struct UnkNavQuery_l6aeff0 {
    void** vtable;
    UnkOptDir_l6aeff0 dir;
    u8 pad_0x18[4];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
};

// The movement (+0x218 of the owner): its facing (vf_0x370) and the facing it keeps at +0x70.
struct UnkShakernautMovement_l6aeff0 {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkVec3f facing;  // 0x70

    UnkVec3f GetFacing() {
        return reinterpret_cast<UnkVec3f (*)(UnkShakernautMovement_l6aeff0*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkSubArea_l6aeff0 {
    void* id;
};

struct UnkSubAreaManager_l6aeff0 {
    u8 pad_0x0[0x3f0];
    UnkSubArea_l6aeff0* current;
};

struct UnkShakernautAI_l6aeff0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x121e - 0x38];
    bool check_pending;  // 0x121e
    u8 pad_0x121f[0x1468a - 0x121f];
    bool turned;  // 0x1468a
    u8 pad_0x1468b[0x148a0 - 0x1468b];
    bool turn_pending;  // 0x148a0
    bool turn_requested;  // 0x148a1
    bool field_0x148a2;
};
static_assert(__builtin_offsetof(UnkShakernautAI_l6aeff0, turned) == 0x1468a, "layout");
static_assert(__builtin_offsetof(UnkShakernautAI_l6aeff0, field_0x148a2) == 0x148a2, "layout");

bool fn_71006af438(UnkShakernautAI_l6aeff0* self, const CVector3D* pos, bool* out);

// The sub-area manager, created on first use.
static inline UnkSubAreaManager_l6aeff0* SubAreaManager_l6aeff0() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_l6aeff0*>(manager);
}

// Whether x is within 0.001 of zero.
static inline bool NearZero_l6aeff0(f32 x) { return -0.001f - x <= 0.0f && 0.001f - x >= 0.0f; }

static inline CVector3D Pos_l6aeff0(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_71006aeff0(UnkShakernautAI_l6aeff0* self, bool flag) {
    fn_7101101c44(self, flag);
    if (!self->check_pending || !g_unk_7172706401) {
        if (!self->turn_requested) {
            self->turn_pending = false;
            return;
        }
        if (fn_7100d7bea0(self->owner, 2))
            return;
        void* sink = self->owner->field_0x218;
        UnkShakernautMovement_l6aeff0* movement = static_cast<UnkShakernautMovement_l6aeff0*>(sink);

        UnkNavQuery_l6aeff0 query;
        query.field_0x1c = 0;
        query.field_0x34 = 0;
        query.field_0x20 = 0;
        query.field_0x28 = 0;
        query.vtable = g_unk_7101a24b58 + 2;
        query.field_0x30 = 0;
        UnkEffectParams_l6aeff0 effect;
        effect.field_0xc = 0;
        query.dir.Clear();
        effect.field_0x14 = 0;
        effect.field_0x28.Clear();
        effect.dir.Clear();
        effect.vtable = g_unk_7101a24db8 + 2;
        effect.field_0x3d = 0;
        effect.field_0x3f = 0;
        effect.field_0x41 = 0;
        effect.field_0x52 = 0;
        __builtin_memset(effect.field_0x44, 0, 13);
        effect.field_0x38 = 1;
        {
            UnkVec3f facing = movement->GetFacing();
            effect.dir.value = UnkVec3f(-facing.x, -facing.y, -facing.z);
            effect.dir.set = true;
        }
        fn_71010c7e6c(static_cast<UnkEffectSink*>(sink), &effect, 0);
        {
            UnkVec3f facing = movement->GetFacing();
            query.dir.value = UnkVec3f(-facing.x, -facing.y, -facing.z);
            query.dir.set = true;
        }
        fn_71006f4b0c(self->owner->field_0x298, &query, 0);
        {
            UnkVec3f facing = movement->GetFacing();
            if (!(NearZero_l6aeff0(facing.x) && NearZero_l6aeff0(facing.y) && NearZero_l6aeff0(facing.z)))
                movement->facing = UnkVec3f(-facing.x, -facing.y, -facing.z);
        }
        self->turn_pending = false;
        self->turned = true;
        return;
    }

    if (!SubAreaManager_l6aeff0()->current) {
        self->check_pending = false;
        self->turn_pending = false;
        return;
    }
    bool inside;
    {
        base::global::CRntVector<UnkRecB_b189*> found(0xf);
        UnkSubAreaManager_l6aeff0* manager = SubAreaManager_l6aeff0();
        CVector3D pos3 = Pos_l6aeff0(self->owner);
        CVector2D pos;
        fn_710007b430(&pos, &pos3);
        inside = fn_7100ee2bd8(reinterpret_cast<UnkObjMgrB_b189*>(manager), &found, &pos, 4, nullptr);
        if (inside) {
            inside = false;
            for (UnkRecB_b189* rec : found) {
                UnkSubArea_l6aeff0* area = reinterpret_cast<UnkSubArea_l6aeff0*>(rec);
                if (SubAreaManager_l6aeff0()->current->id == area->id)
                    inside = true;
            }
        }
        found.Free();
    }
    if (!inside) {
        self->check_pending = false;
        self->turn_pending = false;
        return;
    }

    CVector3D pos = Pos_l6aeff0(self->owner);
    if (!fn_71006af438(self, &pos, &self->field_0x148a2) && self->check_pending) {
        self->check_pending = false;
        self->turn_pending = self->field_0x148a2;
        if (!fn_71006af848(reinterpret_cast<UnkShakernautAI_b65a4*>(self)))
            return;
    } else {
        UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (fn_710117b348(anim, fn_7100d05b88())) {
            if (self->turn_pending)
                self->check_pending = false;
            return;
        }
    }
    self->turn_pending = false;
}
static_assert(__builtin_offsetof(UnkEffectParams_l6aeff0, field_0x52) == 0x52, "layout");
static_assert(__builtin_offsetof(UnkNavQuery_l6aeff0, field_0x34) == 0x34, "layout");
