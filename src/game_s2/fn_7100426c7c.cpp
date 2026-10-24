// Asks the aim action of the actor's target whether the actor can shoot ("DC_Shot") at the aim
// angle held in the component (in radians at 0x28), passing the target's position (values at
// 0x44, 0x54 and 0x64). When the action accepts, returns the (possibly adjusted) angle in radians,
// otherwise 0.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAimAction {
    void** vtable;
    const CClass* vf_0x18() { return reinterpret_cast<const CClass* (*)(UnkAimAction*)>(vtable[3])(this); }
};

struct UnkAimActor {
    u8 pad_0x0[0x38];
    u8 field_0x38[8];
    u8 pad_0x40[4];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkAimOwner {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x18];
    f32 field_0x28;
};

struct UnkPosition {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

void* fn_71011771e0(void* handle, u64 key);
UnkAimAction* fn_710070d500(void* found);
const CClass* fn_7100b8f4e8();  // CAIAimAction::refl_GetClass
u64 fn_7100001570(const char* str, u64 length);
bool fn_7100b8fa7c(UnkAimAction* action, f32* angle, const UnkMatrix44* matrix, u64* hash, UnkPosition* position,
                   bool flag, bool flag2);

f32 fn_7100426c7c(UnkAimOwner* self, bool flag) {
    f32 result = 0.0f;
    if (!self->field_0x8->field_0x220)
        return result;
    UnkAimActor* actor = reinterpret_cast<UnkAimActor*>(fn_7100d7c990(self->field_0x8));
    u32 x = actor->field_0x44;
    u32 y = actor->field_0x54;
    u32 z = actor->field_0x64;
    UnkPosition position = {x, y, z};
    UnkAimAction* action = fn_710070d500(fn_71011771e0(self->field_0x8->field_0x220, 0xf4061ff25a2163d6));
    if (action && fn_710009d50c(action->vf_0x18(), fn_7100b8f4e8())) {
        f32 angle = (f64)(self->field_0x28 * 180.0f) / 3.141592653589793;
        const UnkMatrix44* matrix =
            &fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
        u64 hash = fn_7100001570("DC_Shot", 7);
        if (fn_7100b8fa7c(action, &angle, matrix, &hash, &position, flag, false))
            return (f32)((f64)angle * 3.141592653589793 / 180.0);
    }
    return result;
}
