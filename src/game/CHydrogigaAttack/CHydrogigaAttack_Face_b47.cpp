// CHydrogigaAttack unit (tools/tu_map.py unit 874): when the owner has a character class and a target,
// plays track 4 or 5 on it by whether the target is left of the owner (x at +0x44), then runs the base
// step (fn_71003627d0). Names are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkActorTarget;

struct UnkHydrogigaAttack_b47 {
    void** vtable;
    UnkActor* owner;  // 0x8
};

// Signature as the other AI units declare it.
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);
void fn_71003627d0(UnkHydrogigaAttack_b47* self);

void fn_71005b2f74(UnkHydrogigaAttack_b47* self) {
    void* cls = self->owner->field_0x220;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (cls && target)
        fn_710117662c(reinterpret_cast<UnkActorTarget*>(cls), !(target->field_0x44 >= self->owner->field_0x44) ? 4 : 5,
                      1, false, false, -1.0f);
    fn_71003627d0(self);
}
