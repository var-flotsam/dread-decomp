// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): whether the plasma ray may start. After the base check,
// a Shelmit AI (the owner's +0x240) in state 0 that fn_71006bc178 does not hold needs its charge (+0x147a8)
// full; in state 1, fn_71006b5c00 and the lazily created manager's list at +0x3f0 must accept the owner.
// Either way the owner's target (fn_7100d7c990) must not be dead (fn_7101161088 on its +0x1b8).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CLifeComponent.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkPlasmaRayAttack_ovb0126;
bool fn_71006b5c00(UnkPlasmaRayAttack_ovb0126* self);
bool fn_71006bd1e0(void* list, UnkActor* actor);

struct UnkShelmitAI_l6b5e10 {
    u8 pad_0x0[0x14784];
    u32 state;  // 0x14784
    u8 pad_0x14788[0x20];
    f32 charge;  // 0x147a8
};

static inline UnkManager_71726af468* GetManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

// Keeps ok only while the owner's target is alive.
static inline bool TargetAlive(CAttack* self, bool ok) {
    if (self->field_0x8) {
        if (UnkActor* target = fn_7100d7c990(self->field_0x8)) {
            if (ok && target->field_0x1b8)
                ok = !fn_7101161088(target->field_0x1b8);
        }
    }
    return ok;
}

bool fn_71006b5e10(CAttack* self, void* arg, u32 mode) {
    if (!fn_7100360b84(self, arg, mode))
        return false;
    UnkActor* owner = self->field_0x8;
    if (!owner || !owner->field_0x240)
        return true;
    UnkReflObject* component = static_cast<UnkReflObject*>(owner->field_0x240);
    bool is_shelmit = fn_710009d50c(component->vf_0x0(), fn_71006b98c0());
    UnkShelmitAI_l6b5e10* ai = is_shelmit ? reinterpret_cast<UnkShelmitAI_l6b5e10*>(component) : nullptr;
    if (!ai)
        return true;
    bool ok = true;
    if (ai->state == 0 && !fn_71006bc178(reinterpret_cast<UnkShelmitAI_sis*>(ai)))
        ok = TargetAlive(self, ai->charge >= 0.999);
    if (is_shelmit && ok && reinterpret_cast<UnkShelmitAI_l6b5e10*>(component)->state == 1) {
        ok = fn_71006b5c00(reinterpret_cast<UnkPlasmaRayAttack_ovb0126*>(self)) &&
             fn_71006bd1e0(reinterpret_cast<u8*>(GetManager()) + 0x3f0, self->field_0x8);
        ok = TargetAlive(self, ok);
    }
    return ok;
}
