// Per-frame step of a component: runs the base step, and when the state at +0x14a08 is 1 builds a
// counted reference with fn_710023b580 on the member at +0x149e0, hands it to the one at +0x14a10
// (fn_7100234f78) and moves to state 2. Then, if the object that member points at has a target at
// +0x10, runs fn_710049b920 on the component.
#include "base/UnkRefHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkBossAIComponent.h"

// The weak handle the holder refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkTarget_49b874 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkComponent_49b874 {
    u8 pad_0x0[0x149e0];
    u8 field_0x149e0[0x28];
    u32 field_0x14a08;
    u8 pad_0x14a0c[4];
    base::UnkRefHandle field_0x14a10;
};

void fn_710049b920(UnkComponent_49b874* self);

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_710049b874(UnkComponent_49b874* self, void* arg) {
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    if (self->field_0x14a08 == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x149e0);
            // The reference's pointer is the handle's.
            fn_7100234f78(&self->field_0x14a10, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->field_0x14a08 = 2;
    }
    UnkTarget_49b874* target = static_cast<UnkTarget_49b874*>(self->field_0x14a10.ptr);
    if (target && target->field_0x10)
        fn_710049b920(self);
}
