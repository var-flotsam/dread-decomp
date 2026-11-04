// The collision world's unit (game::logic::collision, src/game/logic/collision/UnkCollisionWorld.cpp): whether two
// collision objects hit, through the dispatcher fn_7100d5761c with each object's own layer mask (vf_0x58); false
// when either is missing. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The result of a query (as in UnkCollisionWorld.cpp): bit 0 a hit. Returned in memory: its copy is user-written.
struct UnkColliderResult {
    u32 bits;

    UnkColliderResult() : bits(0) {}
    UnkColliderResult(u32 b) : bits(b) {}
    UnkColliderResult(const UnkColliderResult& o) : bits(o.bits) {}
};

UnkColliderResult fn_7100d5761c(UnkReflObject* objA, UnkReflObject* objB, u32 layersA, u32 layersB, void* c, u32 d);

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
