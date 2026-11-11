// behaviortree::CShelmitIsShelteredCondition (tools/tu_map.py unit 1052, the name from reflection): true
// when the owner's (+0x18) component at +0x240 is a CShelmitAIComponent and fn_71006bc178 says no for it.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkShelmitAI_sis {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkShelmitAI_sis*)>(vtable[0])(this);
    }
};

struct UnkShelmitCondition_sis {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
};

bool fn_71006b8588(UnkShelmitCondition_sis* self) {
    UnkActor* owner = self->owner;
    if (owner) {
        UnkShelmitAI_sis* ai = static_cast<UnkShelmitAI_sis*>(owner->field_0x240);
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_71006b98c0()))
            return fn_71006bc178(ai) == 0;
    }
    return false;
}
