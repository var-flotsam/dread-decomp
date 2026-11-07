// A CDredhedDiveAttack check: the base attack's check (fn_7100360b84) passes, fn_71004e468c does
// not, and the target's object at +0x1c0, when it is accepted by fn_7101161088, is not further
// than half the attack's range (+0x4c). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/CAttack.h"

bool fn_7101161088(void*);

struct UnkDredhedDiveTargetObject {
    u8 pad_0x0[0x68];
    f32 field_0x68;
};

struct UnkDredhedDiveAttack {
    void** vtable;
    UnkActor* actor;
    u8 pad_0x10[0x3c];
    f32 field_0x4c;
};

bool fn_71004e468c(UnkDredhedDiveAttack* self);

bool fn_71004e460c(UnkDredhedDiveAttack* self, void* a, u32 b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), a, b);
    if (!ok || fn_71004e468c(self))
        return false;
    if (UnkActor* actor = self->actor) {
        if (UnkActor* target = fn_7100d7c990(actor)) {
            UnkDredhedDiveTargetObject* obj = *reinterpret_cast<UnkDredhedDiveTargetObject**>(target->pad_0x1c0);
            if (obj && fn_7101161088(obj) && obj->field_0x68 > self->field_0x4c * 0.5f)
                return false;
        }
    }
    return true;
}
