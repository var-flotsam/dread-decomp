// CChozombieFXComponent's unit (tools/tu_map.py, unit 1270): fn_710080706c keeps the definition cast to
// CCharClassChozombieFXComponent, copies its model path and creates the model, as
// src/game/CAreaFXComponent/CAreaFXComponent_Models_ovb0081.cpp does. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::UnkReflObject;

struct UnkModel;
// As src/game/CAreaFXComponent/CAreaFXComponent_Models_ovb0081.cpp declares them.

struct UnkChozombieFXDef_ovb0130 : UnkReflObject {
    u8 pad_0x8[0x50];
    CFilePathStrId path;  // 0x58
};

struct UnkChozombieFX_ovb0130 {
    u8 pad_0x0[0x10];
    UnkReflObject* def;  // 0x10
    u8 pad_0x18[0x30];
    CFilePathStrId path;  // 0x48
    u8 pad_0x50[0x288];
    UnkModel* model;  // 0x2d8
};
static_assert(__builtin_offsetof(UnkChozombieFX_ovb0130, model) == 0x2d8, "layout");

static inline UnkSceneNode* Node_ovb0130(UnkModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

bool fn_710080706c(UnkChozombieFX_ovb0130* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* def = self->def;
    self->def = def && fn_710009d50c(def->vf_0x0(), fn_71008054e4()) ? def : nullptr;
    self->path = CFilePathStrId(static_cast<UnkChozombieFXDef_ovb0130*>(self->def)->path);
    if (!self->model) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkModel* model = nullptr;
        fn_71006cae2c(manager->field_0x218, self->path, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, Node_ovb0130(model), 0);
        self->model = model;
        if (model) {
            fn_71001436a0(model, 0);
            fn_710014364c(self->model, 0);
        }
    }
    return result;
}
