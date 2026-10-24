// With the flag at 0x5d8 set, notifies the cooldown X boss AI component of the owner at 0x230
// (when it is one), then fn_7100232c68 on the object at 0x30, and forwards all arguments to
// fn_7100ac4c04; otherwise false.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent {
    virtual const CClass* vf_0x0();
};
struct UnkOwnerObject {
    u8 unk_0x0[0x240];
    UnkAIComponent* component;
};
struct UnkOwner {
    u8 unk_0x0[0x10];
    UnkOwnerObject* object;
};
struct UnkSelf {
    u8 unk_0x0[0x30];
    void* field_0x30;
    u8 unk_0x38[0x1f8];
    UnkOwner* owner;
    u8 unk_0x238[0x3a0];
    u32 field_0x5d8;
};

void fn_71004d43dc(UnkAIComponent* component, void* a1);
void fn_7100232c68(void* object);
bool fn_7100ac4c04(UnkSelf* self, void* a1, void* a2, void* a3, void* a4, void* a5);

bool fn_71004d771c(UnkSelf* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    if (!self->field_0x5d8)
        return false;
    if (self->owner && self->owner->object) {
        UnkAIComponent* component = self->owner->object->component;
        if (component && fn_710009d50c(component->vf_0x0(), fn_71004cf318()))
            fn_71004d43dc(component, self->field_0x30);
    }
    fn_7100232c68(self->field_0x30);
    return fn_7100ac4c04(self, a1, a2, a3, a4, a5);
}
