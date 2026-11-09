// CSamusAimAction's unit (tools/tu_map.py unit 2209): while the animation (+0xb0, its type's hash at +0x30) is one
// of two aim animations, the blend space at +0xc0 (a base::animation::CBlendSpace1D) is posed (fn_71000121b0)
// with an identity matrix and the arguments passed on. Always true. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkAimBlendSpace_ovb0115 {
    virtual const base::reflection::CType* GetClass();
};

struct UnkAimAnimType_ovb0115 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAimAnim_ovb0115 {
    UnkAimAnimType_ovb0115* type;
};

struct UnkSamusAimAction_ovb0115 {
    u8 pad_0x0[0xb0];
    UnkAimAnim_ovb0115* anim;  // 0xb0
    u8 pad_0xb8[0x8];
    UnkAimBlendSpace_ovb0115* blend;  // 0xc0
};

void fn_71000121b0(UnkAimBlendSpace_ovb0115* blend, void* a, const UnkMatrix44* m, void* b, void* c, void* d,
                   bool flag);

bool fn_7100c9adbc(UnkSamusAimAction_ovb0115* self, void* a, void* b, void* c, void* d, bool flag) {
    u64 hash = self->anim->type->hash;
    if (hash == 0x56ffaa4887b8e2e8ull || hash == 0x46cb3337bfed7326ull) {
        UnkAimBlendSpace_ovb0115* blend = self->blend;
        if (blend && fn_710009d50c(blend->GetClass(), fn_71000062ac())) {
            UnkMatrix44 m = {};
            m.m[15] = 1.0f;
            m.m[10] = 1.0f;
            m.m[5] = 1.0f;
            m.m[0] = 1.0f;
            fn_71000121b0(blend, a, &m, b, c, d, flag);
        }
    }
    return true;
}
