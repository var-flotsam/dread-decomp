// A CTakumakuDashAttack check: the base attack's check (fn_7100360b84) passes and the actor has a target
// (fn_7100d7c990; the lookup runs whatever the base check says). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAttack.h"

// Declared as its other callers declare it (src/game_s2/fn_71003f1b00.cpp); it takes three arguments.
struct UnkSelf;
u32 fn_7100360b84(UnkSelf* self);

bool fn_71006ce7d4(UnkAttackBase* self, void* a, void* b) {
    bool ok = reinterpret_cast<bool (*)(UnkAttackBase*, void*, void*)>(fn_7100360b84)(self, a, b);
    UnkActor* target = self->field_0x8 ? fn_7100d7c990(self->field_0x8) : nullptr;
    return ok && target;
}
