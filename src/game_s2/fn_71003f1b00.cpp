// Runs fn_7100360b84 and requires the owner's position object to exist. When the owner's object at
// +0x240 is a Bigkran AI component and that all held, passes the negation of its fn_71003f4bc0
// to fn_71003f1ba8 (a tail call). Otherwise returns the combined result.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkComponent {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
};

u32 fn_7100360b84(UnkSelf* self);
u32 fn_710009d50c(const void* from, const void* to);
bool fn_71003f4bc0(UnkComponent* component);
u32 fn_71003f1ba8(UnkSelf* self, bool flag);

// NON_MATCHING(minor): the operands of the final `and` of the two flags are swapped
u32 fn_71003f1b00(UnkSelf* self) {
    u32 ok = fn_7100360b84(self);
    ok &= (u32)(fn_7100d7c990(self->field_0x8) != nullptr);
    if (UnkActor* owner = self->field_0x8) {
        if (UnkComponent* component = static_cast<UnkComponent*>(owner->field_0x240)) {
            u32 is_class = fn_710009d50c(component->vf_0x0(), fn_71003f4454());
            u32 both = ok & is_class;
            if (both == 1)
                return fn_71003f1ba8(self, !fn_71003f4bc0(component));
        }
    }
    return ok;
}
