// CPerceptionModifierLogicAction unit (tools/tu_map.py, the name from reflection): the action's set-up.
// After the base set-up (fn_7100a55ac8) it resolves its lazy reference (source +0x20 through
// fn_710023b580, state +0x48, handle +0x50) and hands the target actor's name (+0x30) and the value at
// +0x58 to the AI manager g_unk_71726af468 (created on first use): in mode 1 (+0x10) to fn_7100daa92c,
// in mode 0 to fn_7100daa638 with the actor's position (+0x44, +0x54, +0x64). Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkTutoExitAction_b47;

struct UnkPerceptionModifierAction_b55 {
    u8 pad_0x0[0x10];
    u32 mode;  // 0x10
    u8 pad_0x14[0xc];
    u8 source[0x28];  // 0x20
    u32 state;        // 0x48
    u8 pad_0x4c[4];
    base::UnkRefHandle handle;  // 0x50
    u32 value;                  // 0x58

    char* Target() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return *reinterpret_cast<char**>(reinterpret_cast<char*>(handle.ptr) + 0x10);
    }
};

static inline UnkManager_71726af468* AIManager() {
    if (!g_unk_71726af468) {
        auto* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

static inline f32 At(char* p, u32 off) {
    return *reinterpret_cast<f32*>(p + off);
}

// The translation column of the actor's 3x4 transform.
static inline CVector3D Column(char* actor) {
    CVector3D v;
    v.x = At(actor, 0x44);
    v.y = At(actor, 0x54);
    v.z = At(actor, 0x64);
    return v;
}

void fn_7100a5120c(UnkPerceptionModifierAction_b55* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    switch (self->mode) {
    case 0: {
        UnkManager_71726af468* m = AIManager();
        fn_7100daa638(m, CStrId(*reinterpret_cast<CStrId*>(self->Target() + 0x30)), self->value,
                      Column(self->Target()));
        break;
    }
    case 1: {
        UnkManager_71726af468* m = AIManager();
        fn_7100daa92c(m, CStrId(*reinterpret_cast<CStrId*>(self->Target() + 0x30)), self->value);
        break;
    }
    }
}
