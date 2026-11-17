// CAreaFXComponent unit (tools/tu_map.py unit 1263): loads the effect actor the definition names, keeps a
// weak handle to it, scales its model to the rectangle and places it at the rectangle's centre, then starts
// its timeline. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

extern const u64 g_unk_710164abd8;

void fn_71001435f0(void* self, const CVector3D* scale);

// Counters raised through accesses that may alias anything: the original reloads the pointers it
// reached them through.
typedef u32 __attribute__((may_alias)) UnkAliasU32_l7ff358;

struct UnkAreaFXName_l7ff358 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 field_0x44;
};

// The type id the entry is looked up under.
struct alignas(8) UnkPowerUpLifeType_b52 {
    u32 id;
};

struct UnkAreaFXRect_l7ff358 {
    f32 min_x, min_y;
    f32 max_x, max_y;
};

// The model's size before scaling.
struct UnkAreaFXModelSize_l7ff358 {
    u8 pad_0x0[0x388];
    f32 x;  // 0x388
    f32 y;  // 0x38c
    f32 z;  // 0x390
};

struct UnkAreaFXModel_l7ff358 {
    u8 pad_0x0[0x30];
    UnkAreaFXModelSize_l7ff358* size;  // 0x30
    u8 pad_0x38[0x18];
    void* scale_target;  // 0x50
};

struct UnkAreaFXActor_l7ff358;

struct UnkAreaFXActorVt_l7ff358 {
    void* pad_0x0[35];
    void (*SetTransform)(UnkAreaFXActor_l7ff358* self, f32 (*transform)[4], const u32* flags);  // 0x118
};

struct UnkAreaFXActor_l7ff358 {
    const UnkAreaFXActorVt_l7ff358* vt;
    u8 pad_0x8[0x30];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x1c0];
    UnkAreaFXModel_l7ff358* model;  // 0x228
    u8 pad_0x230[0x180];
    bool transform_set;  // 0x3b0
};
static_assert(__builtin_offsetof(UnkAreaFXActor_l7ff358, transform_set) == 0x3b0, "UnkAreaFXActor layout");

struct UnkAreaFXDef_l7ff358 {
    u8 pad_0x0[0x100];
    f32 depth;  // 0x100
    u8 pad_0x104[0x4];
    CStrId actor;  // 0x108
};

struct UnkAreaFXSelf_l7ff358 {
    u8 pad_0x0[0x3b0];
    UnkWeakHandle* effect;  // 0x3b0
    CVector3D center;       // 0x3b8
    u8 pad_0x3c4[0x34];
    UnkAreaFXDef_l7ff358* def;  // 0x3f8
};
static_assert(__builtin_offsetof(UnkAreaFXSelf_l7ff358, def) == 0x3f8, "UnkAreaFXSelf layout");

static inline CVector3D MakeVec_l7ff358(f32 x, f32 y, f32 z) {
    CVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline UnkAreaFXActor_l7ff358* ActorOf(UnkWeakHandle* handle) {
    return reinterpret_cast<UnkAreaFXActor_l7ff358*>(handle->target);
}

void fn_71007ff358(UnkAreaFXSelf_l7ff358* self, const UnkAreaFXRect_l7ff358* rect) {
    UnkAreaFXDef_l7ff358* def = self->def;
    UnkActorDefFactory* factory = g_unk_71726a0220;
    u32 type = g_unk_7172a0ba48;
    ++*reinterpret_cast<UnkAliasU32_l7ff358*>(&reinterpret_cast<UnkAreaFXName_l7ff358*>(def->actor.data)->field_0x44);
    UnkPowerUpLifeEntry_b52* entry =
        fn_710119438c(factory, reinterpret_cast<UnkAreaFXName_l7ff358*>(def->actor.data)->text, &def->actor,
                      UnkPowerUpLifeType_b52{type}, nullptr);
    UnkWeakHandle* handle = nullptr;
    if (entry) {
        base::reflection::UnkReflObject* object = reinterpret_cast<base::reflection::UnkReflObject*>(entry);
        if (fn_710009d50c(object->vf_0x0(), fn_7100d70c20()))
            handle = fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entry));
    }
    AssignRef(&self->effect, handle);
    if (!handle || !handle->target)
        return;

    {
        UnkAreaFXModel_l7ff358* model = ActorOf(handle)->model;
        UnkAreaFXModelSize_l7ff358* size = model->size;
        CVector3D scale = MakeVec_l7ff358((rect->max_x - rect->min_x) / size->x, (rect->max_y - rect->min_y) / size->y,
                                          self->def->depth / size->z);
        fn_71001435f0(model->scale_target, &scale);
    }

    self->center = CVector3D{(rect->min_x + rect->max_x) * 0.5f, (rect->min_y + rect->max_y) * 0.5f, 0.0f};
    UnkAreaFXActor_l7ff358* actor = ActorOf(self->effect);
    actor->transform[0][3] = self->center.x;
    actor->transform[1][3] = self->center.y;
    actor->transform[2][3] = 0.0f;
    u32 flags = 1;
    actor->vt->SetTransform(actor, actor->transform, &flags);
    actor->transform_set = true;

    UnkWeakHandle* effect = self->effect;
    fn_7100d7c828(reinterpret_cast<UnkActor*>(effect ? effect->target : nullptr), &g_unk_710164abd8, false);
}
