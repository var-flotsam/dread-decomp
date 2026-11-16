// The collision world's unit (game::logic::collision, src/game/logic/collision/UnkCollisionWorld.cpp): whether two
// collision objects hit, through the dispatcher fn_7100d5761c with each object's own layer mask (vf_0x58); false
// when either is missing. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2510.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::reflection::UnkReflObject;

static inline u32 Layers_A6_b72(UnkReflObject* obj) {
    void** vtable = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<u32 (*)(UnkReflObject*)>(vtable[0x58 / 8])(obj);
}

bool fn_7100d57f44(UnkReflObject* a, UnkReflObject* b, void* c, u32 d) {
    if (!a || !b)
        return false;
    u32 la = Layers_A6_b72(a);
    u32 lb = Layers_A6_b72(b);
    return fn_7100d5761c(a, b, la, lb, c, d).bits & 1;
}
