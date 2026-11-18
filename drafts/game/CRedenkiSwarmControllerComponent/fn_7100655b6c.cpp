// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): whether the swarm may go for its target.
// The sub-area manager's current record (+0x3f0) must be among the records at the swarm's position
// (fn_7100ee2bd8); then the route check fn_7100655390 from the target position decides. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkObjMgrB_b189;
struct UnkRecB_b189;
struct UnkObjB_b189;
bool fn_7100ee2bd8(UnkObjMgrB_b189* self, base::global::CRntVector<UnkRecB_b189*>* out, const base::math::CVector2D* pos,
                   u32 mask, UnkObjB_b189* skip);

struct UnkRedenkiCheck_ovb0084;
bool fn_7100655390(UnkRedenkiCheck_ovb0084* self, CVector2D* point, bool flag);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkSubAreaManager_l655b6c {
    u8 pad_0x0[0x3f0];
    UnkRecB_b189* current;  // 0x3f0
};

static inline UnkSubAreaManager_l655b6c* SubAreaManager_l655b6c() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_l655b6c*>(manager);
}

struct UnkRedenkiTransform_l655b6c {
    u8 pad_0x0[0x38];
    f32 matrix[3][4];  // 0x38, the translation in the last column
};

struct UnkRedenkiTuning_l655b6c {
    u8 pad_0x0[0x58];
    bool field_0x58;
};

struct UnkRedenkiPlayerState_l655b6c {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkRedenkiPlayerLogic_l655b6c {
    u8 pad_0x0[0x2d08];
    UnkRedenkiPlayerState_l655b6c* state;  // 0x2d08
};

struct UnkRedenkiPlayer_l655b6c {
    u8 pad_0x0[0x208];
    UnkRedenkiPlayerLogic_l655b6c* logic;  // 0x208

    bool InState6() const { return logic && logic->state && logic->state->state == 6; }
};

struct UnkGameFlags_l655b6c {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    bool field_0xc5;
};

struct UnkRedenkiSwarm_l655b6c {
    u8 pad_0x0[0x30];
    UnkRedenkiTransform_l655b6c* transform;  // 0x30
    u8 pad_0x38[0x560 - 0x38];
    UnkRedenkiTuning_l655b6c* tuning;  // 0x560
};

bool fn_7100655b6c(UnkRedenkiSwarm_l655b6c* self) {
    UnkRecB_b189* current = SubAreaManager_l655b6c()->current;
    if (!current)
        return false;

    CVector2D position2;
    base::global::CRntVector<UnkRecB_b189*> records(0xf);
    UnkSubAreaManager_l655b6c* manager = SubAreaManager_l655b6c();
    {
        UnkRedenkiTransform_l655b6c* transform = self->transform;
        CVector3D position = {transform->matrix[0][3], transform->matrix[1][3], transform->matrix[2][3]};
        fn_710007b430(&position2, &position);
    }

    bool result = false;
    if (fn_7100ee2bd8(reinterpret_cast<UnkObjMgrB_b189*>(manager), &records, &position2, 4, nullptr) &&
        records.count != 0) {
        u32 i = 0;
        UnkRecB_b189* record;
        do {
            record = records.GrowAt(i);
            if (record == current)
                break;
            ++i;
        } while (i < records.count);
        if (record == current) {
            bool lowered = false;
            if (!self->tuning->field_0x58) {
                auto* player = reinterpret_cast<UnkRedenkiPlayer_l655b6c*>(fn_7100dec788(g_unk_7101d47310));
                auto* flags = reinterpret_cast<UnkGameFlags_l655b6c*>(g_unk_7101d47310);
                lowered = player && player->InState6() && !flags->field_0xc5 && flags->field_0xc4;
            }
            UnkVec3Result goal = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
            fn_710007b430(&position2, reinterpret_cast<const CVector3D*>(&goal));
            result = fn_7100655390(reinterpret_cast<UnkRedenkiCheck_ovb0084*>(self), &position2, lowered);
        }
    }
    records.Free();
    return result;
}
