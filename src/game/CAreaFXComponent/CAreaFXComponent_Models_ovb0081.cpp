// CAreaFXComponent unit (tools/tu_map.py unit 1263): the component's models. With the shared flag
// (+0x45, copied from the definition's +0x84) it uses one model (+0xcd0, loaded from the path at
// +0xcc8) placed 400 times (instance handles at +0x48); otherwise a list of up to 50 models of its own
// (count +0xcd8, slots +0xcdc), whose accessor extends the list with empty slots. fn_7100803490 sets
// the component up after the base step and loads the model at +0xe70; fn_71008036a8 loads, fn_7100803394
// shows, fn_71008038cc releases and fn_71008035ec clears them. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::UnkReflObject;

struct UnkModel;
struct UnkMorphBallLauncher_b57;
struct UnkModelData;

void fn_71009f194c(char*);
void fn_7100219e58(UnkRenderManager*, u64, bool);
void fn_7100219e18(UnkRenderManager*, UnkModel*, bool);
void fn_71001436a0(UnkModel* model, u32 flag);
void fn_710014364c(UnkModel* model, u32 flag);
void fn_71006cae2c(UnkRenderPool*, const CFilePathStrId&, UnkModel**, base::global::CStrId*);
void fn_71010c422c(UnkMorphBallLauncher_b57*);
void fn_71001c0968(UnkModelData* data, u32 flag);
UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
void fn_7100219dd0(UnkRenderManager* self, u64 model, const UnkMatrix44* m, u32 a, u32 b, u64* instance, u32 c);

struct UnkAreaFXDef_ovb0081 : UnkReflObject {
    u8 pad_0x8[0x50];
    CFilePathStrId path;  // 0x58
    u8 pad_0x60[0x24];
    bool shared;  // 0x84
};

struct UnkAreaFXSet_ovb0081 {
    u8 pad_0x0[0x10];
    UnkReflObject* def;  // 0x10
    u8 pad_0x18[0x2d];
    bool shared;  // 0x45
    u8 pad_0x46[0x2];
    u64 entries[400];  // 0x48
    CFilePathStrId path;  // 0xcc8
    UnkModel* field_0xcd0;
    u32 model_count;        // 0xcd8
    u8 models_raw[50 * 8];  // 0xcdc, 50 UnkModel*
    u8 pad_0xe6c[0x4];
    UnkModel* model;  // 0xe70

    // A model slot; an index past the end first extends the list with empty slots.
    UnkModel*& Model(u32 i) {
        UnkModel** models = reinterpret_cast<UnkModel**>(models_raw);
        u32 n = i + 1;
        if (i >= model_count) {
            __builtin_memset(&models[model_count], 0, (u64)(n - model_count) * sizeof(UnkModel*));
            model_count = n;
        }
        return models[i];
    }
};
static_assert(__builtin_offsetof(UnkAreaFXSet_ovb0081, model) == 0xe70, "UnkAreaFXSet_ovb0081 layout");

static inline UnkSceneNode* Node_ovb0081(UnkModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

static inline UnkItemBase_71011a91c4* Item_ovb0081(UnkModel* model) {
    return model ? reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

void fn_71008035ec(UnkAreaFXSet_ovb0081* self) {
    fn_71009f194c(reinterpret_cast<char*>(self));
    if (self->shared) {
        for (u64 i = 0; i < 400; ++i)
            self->entries[i] = 0;
        return;
    }
    for (u32 i = 0; i < 50; ++i)
        self->Model(i) = nullptr;
}

void fn_71008038cc(UnkAreaFXSet_ovb0081* self) {
    if (self->shared) {
        for (u64 i = 0; i < 400; ++i) {
            fn_7100219e58(g_unk_7101d47300, self->entries[i], false);
            self->entries[i] = 0;
        }
        fn_7100219e18(g_unk_7101d47300, self->field_0xcd0, false);
        fn_71002199d8(g_unk_7101d47300, Item_ovb0081(self->field_0xcd0), 0);
        self->field_0xcd0 = nullptr;
    } else {
        for (u32 i = 0; i < 50; ++i) {
            fn_71002199d8(g_unk_7101d47300, Item_ovb0081(self->Model(i)), 0);
            self->Model(i) = nullptr;
        }
    }
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
}

void fn_7100803394(UnkAreaFXSet_ovb0081* self) {
    if (self->shared) {
        for (u64 i = 0; i < 400; ++i)
            fn_7100219e58(g_unk_7101d47300, self->entries[i], false);
        return;
    }
    for (u32 i = 0; i < 50; ++i) {
        fn_71001436a0(self->Model(i), 0);
        fn_710014364c(self->Model(i), 0);
    }
}

bool fn_7100803490(UnkAreaFXSet_ovb0081* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* def = self->def;
    self->def = def && fn_710009d50c(def->vf_0x0(), fn_7100804314()) ? def : nullptr;
    self->path = CFilePathStrId(static_cast<UnkAreaFXDef_ovb0081*>(self->def)->path);
    self->shared = static_cast<UnkAreaFXDef_ovb0081*>(self->def)->shared;
    if (!self->model) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkModel* model = nullptr;
        fn_71006cae2c(manager->field_0x218, self->path, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr,
                      0);
        self->model = model;
        if (model) {
            fn_71001436a0(model, 0);
            fn_710014364c(self->model, 0);
        }
    }
    return result;
}

void fn_71008036a8(UnkAreaFXSet_ovb0081* self) {
    if (self->shared) {
        UnkRenderManager* manager = g_unk_7101d47300;
        {
            UnkModel* model = nullptr;
            fn_71006cae2c(manager->field_0x218, self->path, &model, &g_unk_7101d470e0);
            fn_7100219ae0(manager, Node_ovb0081(model), 0);
            self->field_0xcd0 = model;
            fn_71001436a0(model, 0);
        }
        fn_71001c0968(*reinterpret_cast<UnkModelData**>(reinterpret_cast<char*>(self->field_0xcd0) + 0x138), 1);
        const UnkVec3f& zero = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
        for (u64 i = 0; i < 400; ++i) {
            UnkMatrix44 m = fn_71000791a8(zero, zero, zero);
            fn_7100219dd0(g_unk_7101d47300, reinterpret_cast<u64>(self->field_0xcd0), &m, 0, 1, &self->entries[i], 0);
        }
        return;
    }
    for (u32 i = 0; i < 50; ++i) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkModel*& slot = self->Model(i);
        UnkModel* model = nullptr;
        fn_71006cae2c(manager->field_0x218, self->path, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, Node_ovb0081(model), 0);
        slot = model;
        fn_710014364c(self->Model(i), 0);
        fn_71001436a0(self->Model(i), 0);
    }
}
