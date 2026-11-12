// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): the beam's effects. fn_71008186e8 drops the lightning
// models (+0x740, count at +0x748) from the render manager unless asked to keep them, then, while the beam has a
// target, switches its three effects (+0x720 through the owner's world, +0x730 and +0x728) to the flag.
// fn_7100817ffc loads the lightning model once and gives it a cleared array of slots. fn_710081ba20 spawns the
// beam's hit effect from the model manager. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU127.h"
#include "game/UnkTU134.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU315.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkRinkaControl_ruc;
struct UnkWorld;
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);

struct UnkGrappleLightning_ovb0047;

struct UnkGrappleOwner_ovb0047 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkGrappleTarget_ovb0047 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkGrappleEffect_ovb0047 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkGrappleBeamFx_ovb0047 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_ovb0047* owner;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkGrappleTarget_ovb0047* link;  // 0x230
    u8 pad_0x238[0x720 - 0x238];
    UnkGrappleEffect_ovb0047* field_0x720;
    UnkGrappleEffect_ovb0047* field_0x728;
    UnkGrappleEffect_ovb0047* field_0x730;
    u8 pad_0x738[0x8];
    UnkGrappleLightning_ovb0047** lightnings;  // 0x740
    u32 lightning_count;  // 0x748
};
static_assert(__builtin_offsetof(UnkGrappleBeamFx_ovb0047, lightning_count) == 0x748, "layout");

void fn_71008186e8(UnkGrappleBeamFx_ovb0047* self, bool keep) {
    if (!keep) {
        for (u64 i = 0; i < self->lightning_count; ++i) {
            if (UnkGrappleLightning_ovb0047* object = self->lightnings[i]) {
                fn_7100219e10(g_unk_7101d47300, reinterpret_cast<u64>(object));
                self->lightnings[i] = nullptr;
            }
        }
    }
    if (self->link && self->link->field_0x10) {
        fn_710114ffb8(reinterpret_cast<UnkRinkaControl_ruc*>(self->owner->world),
                      reinterpret_cast<void*>(self->field_0x720->hash), keep, 1);
        fn_710114fe8c(self->owner->world, self->field_0x730->hash, keep, 0);
        fn_710114fe8c(self->owner->world, self->field_0x728->hash, keep, 0);
    }
}

using base::global::CFilePathStrId;

struct UnkModel;
struct UnkModelData;
void fn_71006cae2c(UnkRenderPool* pool, const CFilePathStrId& path, UnkModel** out, base::global::CStrId* name);

struct UnkGrappleModel_ovb0047 {
    u8 pad_0x0[0x138];
    UnkModelData* data;  // 0x138
};

struct UnkGrappleBeamLightning_ovb0047 {
    u8 pad_0x0[0x738];
    UnkModel* lightning_model;  // 0x738
    void** lightnings;  // 0x740
    u32 lightning_count;  // 0x748
};
static_assert(__builtin_offsetof(UnkGrappleBeamLightning_ovb0047, lightning_count) == 0x748, "layout");

void fn_7100817ffc(UnkGrappleBeamLightning_ovb0047* self) {
    if (self->lightnings)
        return;
    UnkRenderManager* manager = g_unk_7101d47300;
    {
        UnkModel* model;
        CFilePathStrId id("actors/weapons/grapplebeam/models/grapplelightning_1.bcmdl", true);
        model = nullptr;
        fn_71006cae2c(manager->field_0x218, id, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr, 0);
        self->lightning_model = model;
    }
    fn_71001436a0(self->lightning_model, 0);
    fn_71001c0968(reinterpret_cast<UnkGrappleModel_ovb0047*>(self->lightning_model)->data, 1);
    u64 size = (u64)self->lightning_count * sizeof(void*);
    void* data = fn_7100083ba0(size, 0x42);
    __builtin_memset(data, 0, size);
    self->lightnings = static_cast<void**>(data);
}

// The effect object the model manager hands out (as in
// src/game/CShakernautAIComponent/CShakernautAIComponent_Send_ovb0041.cpp).
struct UnkFxObject_ovb0041;
struct UnkGrappleFxVtable_ovb0047 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};

extern const u8 g_unk_7172723340[] __attribute__((visibility("hidden")));  // the effect's path id

static inline UnkGrappleFxVtable_ovb0047* Vtable_ovb0047(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkGrappleFxVtable_ovb0047**>(fx);
}

static inline UnkModelManager* ModelManager_ovb0047() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkGrappleBeam_ovb0047;

// Spawns the beam's effect at `at`, turned by `rotation`.
void fn_710081ba20(UnkGrappleBeam_ovb0047* self, const UnkVec3f* at, const UnkVec3f* rotation) {
    UnkModelManager* manager = ModelManager_ovb0047();
    UnkVec3f position(*at);
    UnkVec3f angles(*rotation);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_7172723340, false, true);
    if (fx) {
        Vtable_ovb0047(fx)->set_position(fx, &position);
        Vtable_ovb0047(fx)->set_rotation(fx, &angles);
        Vtable_ovb0047(fx)->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        Vtable_ovb0047(fx)->set_loop(fx, false);
        Vtable_ovb0047(fx)->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }
}
