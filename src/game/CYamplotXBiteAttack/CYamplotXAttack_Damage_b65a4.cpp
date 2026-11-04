// The YamplotX attacks' damage set-up, in two units with the same body over their own pair of string ids
// (tools/tu_map.py: CYamplotXBiteAttack fn_71006ecf98 with g_unk_717270bd40, CYamplotXStepAttack
// fn_71006edc1c with g_unk_717270bef0): the request's id at +0xb8 becomes the first id; its value
// (fn_71008c1538) is what fn_7100789f5c gives for the first id when the owner's (+0x8) AI component
// (+0x240, cast to CYamplotXAIComponent) has its object at +0x14728 in mode 1 (+0x6fc), else for the
// second; unless the owner's component at +0x2c8 is frozen (fn_71007f9f20), the word at +0xc8 is set
// to 1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_717270bd40[2] __attribute__((visibility("hidden")));
extern CStrId g_unk_717270bef0[2] __attribute__((visibility("hidden")));

// As src/game/CBaseDamageTriggerComponent/CBaseDamageTriggerComponent_b34.cpp and
// src/game/CAreaFXComponent/CAreaFXComponent.cpp declare them.
struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
bool fn_71007f9f20(void*);

f32 fn_7100789f5c(const CStrId* id);

struct UnkYamplotMode_b65a4 {
    u8 pad_0x0[0x6fc];
    s32 mode;  // 0x6fc
};

struct UnkYamplotAI_b65a4 {
    u8 pad_0x0[0x14728];
    UnkYamplotMode_b65a4* field_0x14728;
};

struct UnkYamplotOwner_b65a4 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x80];
    void* field_0x2c8;
};

struct UnkYamplotAttack_b65a4 {
    u8 pad_0x0[0x8];
    UnkYamplotOwner_b65a4* owner;  // 0x8
};

struct UnkYamplotRequest_b65a4 {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
};

__attribute__((always_inline)) static inline void SetUpDamage_b65a4(UnkYamplotAttack_b65a4* self,
                                                                     UnkYamplotRequest_b65a4* request,
                                                                     CStrId* ids) {
    request->id = ids[0];
    base::reflection::UnkReflObject* ai = self->owner->ai;
    UnkYamplotAI_b65a4* cast = fn_710009d50c(ai->vf_0x0(), fn_71006ef064())
                                   ? reinterpret_cast<UnkYamplotAI_b65a4*>(ai)
                                   : nullptr;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request),
                  fn_7100789f5c(cast->field_0x14728->mode == 1 ? &ids[0] : &ids[1]));
    UnkYamplotOwner_b65a4* owner = self->owner;
    if (!owner || !owner->field_0x2c8 || !fn_71007f9f20(owner->field_0x2c8))
        request->field_0xc8 = 1;
}

void fn_71006ecf98(UnkYamplotAttack_b65a4* self, void* unused, UnkYamplotRequest_b65a4* request) {
    SetUpDamage_b65a4(self, request, g_unk_717270bd40);
}

void fn_71006edc1c(UnkYamplotAttack_b65a4* self, void* unused, UnkYamplotRequest_b65a4* request) {
    SetUpDamage_b65a4(self, request, g_unk_717270bef0);
}
