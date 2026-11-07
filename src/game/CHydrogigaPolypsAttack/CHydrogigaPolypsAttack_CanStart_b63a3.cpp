// CHydrogigaPolypsAttack's unit (tools/tu_map.py unit 877): fn_71005b6274. True when the base test
// (fn_71005b2ec4) holds and, if the owner has a CHydrogigaAIComponent (+0x240) and a target with a
// helper (+0x208), that helper holds the target neither way (fn_71009e6f9c, fn_71009e6434).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkHelper;
bool fn_71009e6f9c(UnkHelper*);
bool fn_71009e6434(UnkHelper*);

struct UnkHydrogigaPolypsAttack_b63a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};
struct UnkHydrogigaAttack_b63a3;
bool fn_71005b2ec4(UnkHydrogigaAttack_b63a3* self, void* arg, u32 mode);

struct UnkHydrogigaPolypsAICast_b63a3 {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkHydrogigaPolypsAICast_b63a3*)>(vtable[0])(this);
    }
};

// `arg` and `mode` (x1, w2) reach the base check untouched in the original (tools/forwarded_args.py).
bool fn_71005b6274(UnkHydrogigaPolypsAttack_b63a3* self, void* arg, u32 mode) {
    if (!fn_71005b2ec4(reinterpret_cast<UnkHydrogigaAttack_b63a3*>(self), arg, mode))
        return false;
    auto* ai = static_cast<UnkHydrogigaPolypsAICast_b63a3*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71005ba098()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    auto* helper = target ? static_cast<UnkHelper*>(target->field_0x208) : nullptr;
    if (!ai || !helper)
        return true;
    if (fn_71009e6f9c(helper))
        return false;
    return !fn_71009e6434(helper);
}
