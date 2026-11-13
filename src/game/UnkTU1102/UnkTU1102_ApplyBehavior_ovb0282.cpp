// Unit 1102 (CXParasiteAIComponent hint): applies a parasite behavior's four (non-negative) float
// parameters to the component's block at +0x14728, then, by the behavior's kind (vf_0x18), hands
// the cast behavior's inner part to a setter; kind 5 also looks up the entity its name's hash
// designates in the game manager. Finally reports the kind. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteAI.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;

struct UnkParasiteBehavior_b0282 : base::reflection::UnkReflObject {
    bool flag;  // 0x8
    u8 unk_0x9[7];
    f32 param_0x10;
    f32 param_0x14;
    f32 param_0x18;
    f32 param_0x1c;
    virtual s32 vf_0x18();
};

struct UnkParasiteNameData_b0282 {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;  // 0x30
};

struct UnkXParasiteMode_A3_b75;
void fn_71006ea584(UnkXParasiteAI* self, void* inner);
CStrId fn_71007cd398(void* behavior);

void fn_71006ea210(UnkXParasiteAI* self, UnkParasiteBehavior_b0282* behavior) {
    u64 key;
    alignas(8) u32 kind_value;
    UnkXParasiteWeights* weights = &self->weights;
    fn_71006ea3fc(self, behavior->flag);
    if (behavior->param_0x10 >= 0.0f)
        weights->weight_2 = behavior->param_0x10;
    if (behavior->param_0x14 >= 0.0f)
        weights->weight_0 = behavior->param_0x14;
    if (behavior->param_0x18 >= 0.0f)
        weights->weight_3 = behavior->param_0x18;
    if (behavior->param_0x1c >= 0.0f)
        weights->weight_1 = behavior->param_0x1c;
    s32 kind = behavior->vf_0x18();
    if (kind == 4) {
        u8* cast = fn_710009d50c(behavior->vf_0x0(), fn_71007cc85c()) ? reinterpret_cast<u8*>(behavior) : nullptr;
        fn_71006ea584(self, cast + 0x20);
    } else if (kind == 5) {
        u8* cast = fn_710009d50c(behavior->vf_0x0(), fn_71007ccf84()) ? reinterpret_cast<u8*>(behavior) : nullptr;
        fn_71006ea584(self, cast + 0x60);
        UnkGameManager* manager = g_unk_7101d47310;
        UnkWeakTarget* entity;
        {
            CStrId name = fn_71007cd398(cast);
            u32 kv = g_unk_71727bebb8;
            key = reinterpret_cast<UnkParasiteNameData_b0282*>(name.data)->hash;
            kind_value = kv;
            entity = fn_7100debbc8(manager, &key, &kind_value);
        }
        if (entity)
            fn_71006ea824(self, reinterpret_cast<UnkActor*>(entity));
    }
    fn_71006e9384(self, reinterpret_cast<const UnkXParasiteMode_A3_b75*>(&kind));
}
