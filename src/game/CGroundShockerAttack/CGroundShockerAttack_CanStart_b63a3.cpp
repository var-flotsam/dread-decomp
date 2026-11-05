// CGroundShockerAttack's unit (tools/tu_map.py unit 856): fn_71005a0a48. The base attack's check
// (fn_7100360b84) with a target; when the owner's AI component (+0x240) is a
// CGroundShockerAIComponent, also that it is not busy (fn_71005a4be8), and then its own answer
// (fn_71005a4cf0). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

// Declared as its other callers declare it (src/game_s2/fn_71003f1b00.cpp).
struct UnkSelf;
u32 fn_7100360b84(UnkSelf* self);

struct UnkShockerAI_b62a3;
u32 fn_71005a4be8(UnkShockerAI_b62a3* self);
bool fn_71005a4cf0(UnkShockerAI_b62a3* self);

struct UnkShockerAICast_b63a3 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkShockerAICast_b63a3*)>(vtable[0])(this);
    }
};

struct UnkGroundShockerAttack_b63a3 {
    void** vtable;
    UnkActor* owner;
};

bool fn_71005a0a48(UnkGroundShockerAttack_b63a3* self) {
    bool ok = reinterpret_cast<bool (*)(UnkSelf*)>(fn_7100360b84)(reinterpret_cast<UnkSelf*>(self));
    auto* ai = static_cast<UnkShockerAICast_b63a3*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71005a4004()))
        ai = nullptr;
    ok = ok & (fn_7100d7c990(self->owner) != nullptr);
    auto* shocker = reinterpret_cast<UnkShockerAI_b62a3*>(ai);
    if (ai && ok)
        ok = !reinterpret_cast<bool (*)(UnkShockerAI_b62a3*)>(fn_71005a4be8)(shocker);
    if (ai && ok)
        return fn_71005a4cf0(shocker);
    return ok;
}
