// CMissileBaseGun unit (tools/tu_map.py): a tunable float of CTunableMissileBaseGun
// (g_unk_7172727148, +0x24 or +0x28) when the game manager's current object (fn_7100df5460) is in
// state 6, else 0. An object of kind 7 takes its state from the sub-object at +0x20. Names are
// placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

namespace base {
namespace reflection {
struct UnkTunable;
}
}  // namespace base
extern base::reflection::UnkTunable g_unk_7172727148;

struct UnkMissileGunTunable_b49a4 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
};

struct UnkMissileGunSub_b49a4 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkMissileGunState_b49a4 {
    u8 pad_0x0[0x20];
    UnkMissileGunSub_b49a4* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 kind;

    __attribute__((always_inline)) s32 GetState() {
        if (kind == 7)
            return field_0x20 ? field_0x20->state : 0;
        return kind;
    }
};

void* fn_7100df5460(UnkGameManager* manager);

static inline UnkMissileGunTunable_b49a4& Tunable() {
    return reinterpret_cast<UnkMissileGunTunable_b49a4&>(g_unk_7172727148);
}

f32 fn_710084f768() {
    auto* s = static_cast<UnkMissileGunState_b49a4*>(fn_7100df5460(g_unk_7101d47310));
    if (s == nullptr)
        return 0.0f;
    s32 state = s->GetState();
    f32 value = Tunable().field_0x24;
    return state == 6 ? value : 0.0f;
}

f32 fn_710084f7c8() {
    auto* s = static_cast<UnkMissileGunState_b49a4*>(fn_7100df5460(g_unk_7101d47310));
    if (s == nullptr)
        return 0.0f;
    s32 state = s->GetState();
    f32 value = Tunable().field_0x28;
    return state == 6 ? value : 0.0f;
}
