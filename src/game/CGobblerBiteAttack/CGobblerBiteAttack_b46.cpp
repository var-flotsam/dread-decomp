// CGobblerBiteAttack's unit (tools/tu_map.py unit 830): whether the attack can start: the base
// attack's check (fn_7100360b84) holds, the owner (+8) has a target (fn_7100d7c990) and the bite's
// own check (fn_710058033c) holds. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkGobblerArg_b46a1;

struct UnkGobblerBiteAttack_b46a1 {
    void* vtable;
    UnkActor* owner;
};

struct UnkSelf;
// Declared as the other attack units declare it; this caller passes two more arguments.
u32 fn_7100360b84(UnkSelf*);
bool fn_710058033c(UnkGobblerBiteAttack_b46a1* self);

bool fn_71005802e8(UnkGobblerBiteAttack_b46a1* self, UnkGobblerArg_b46a1* a, UnkGobblerArg_b46a1* b) {
    using Check = bool (*)(UnkGobblerBiteAttack_b46a1*, UnkGobblerArg_b46a1*, UnkGobblerArg_b46a1*);
    bool ok = reinterpret_cast<Check>(fn_7100360b84)(self, a, b);
    UnkActor* target = fn_7100d7c990(self->owner);
    if (ok && target && fn_710058033c(self))
        return true;
    return false;
}
