// Runs fn_7100360b84 (with the pointer and mode it is given) and requires the owner's position object to exist. When the owner's object at
// +0x240 is a Bigkran AI component and that all held, passes the negation of its fn_71003f4bc0
// to fn_71003f1ba8 (a tail call). Otherwise returns the combined result.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkComponent {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkComponent*)>(vtable[0])(this);
    }
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
};

bool fn_71003f4bc0(UnkComponent* component);
bool fn_71003f1ba8(UnkSelf* self, bool flag);

bool fn_71003f1b00(UnkSelf* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode);
    ok &= fn_7100d7c990(self->field_0x8) != nullptr;
    if (UnkActor* owner = self->field_0x8) {
        if (UnkComponent* component = static_cast<UnkComponent*>(owner->field_0x240)) {
            bool is_class = fn_710009d50c(component->vf_0x0(), fn_71003f4454());
            if (ok & is_class)
                return fn_71003f1ba8(self, !fn_71003f4bc0(component));
        }
    }
    return ok;
}
