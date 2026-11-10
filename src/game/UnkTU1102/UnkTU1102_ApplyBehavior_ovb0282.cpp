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
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

struct UnkParasiteBehavior_b0282 : base::reflection::UnkReflObject {
    bool flag;  // 0x8
    u8 unk_0x9[7];
    f32 param_0x10;
    f32 param_0x14;
    f32 param_0x18;
    f32 param_0x1c;
    virtual s32 vf_0x18();
};

struct UnkParasiteBlock_b0282 {
    f32 v0, v1, v2, v3;
};

struct UnkParasiteAI_b0282 {
    u8 unk_0x0[0x14728];
    UnkParasiteBlock_b0282 block;  // 0x14728
};

struct UnkParasiteNameData_b0282 {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;  // 0x30
};

struct UnkXParasiteAI_b64a4;
struct UnkXParasiteAI_A3_b75;
struct UnkXParasiteMode_A3_b75;
struct UnkXParasiteSelect_9fcc;
void fn_71006ea3fc(UnkXParasiteAI_b64a4* self, bool flag);
void fn_71006ea584(UnkParasiteAI_b0282* self, void* inner);
void fn_71006ea824(UnkXParasiteSelect_9fcc* self, UnkActor* target);
void fn_71006e9384(UnkXParasiteAI_A3_b75* self, const UnkXParasiteMode_A3_b75* mode);
CStrId fn_71007cd398(void* behavior);

void fn_71006ea210(UnkParasiteAI_b0282* self, UnkParasiteBehavior_b0282* behavior) {
    u64 key;
    alignas(8) u32 kind_value;
    UnkParasiteBlock_b0282* block = &self->block;
    fn_71006ea3fc(reinterpret_cast<UnkXParasiteAI_b64a4*>(self), behavior->flag);
    if (behavior->param_0x10 >= 0.0f)
        block->v1 = behavior->param_0x10;
    if (behavior->param_0x14 >= 0.0f)
        block->v0 = behavior->param_0x14;
    if (behavior->param_0x18 >= 0.0f)
        block->v3 = behavior->param_0x18;
    if (behavior->param_0x1c >= 0.0f)
        block->v2 = behavior->param_0x1c;
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
            fn_71006ea824(reinterpret_cast<UnkXParasiteSelect_9fcc*>(self), reinterpret_cast<UnkActor*>(entity));
    }
    fn_71006e9384(reinterpret_cast<UnkXParasiteAI_A3_b75*>(self), reinterpret_cast<const UnkXParasiteMode_A3_b75*>(&kind));
}
