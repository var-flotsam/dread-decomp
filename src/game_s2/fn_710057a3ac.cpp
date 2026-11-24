// Whether any entry of the list has an infester AI component with its flag at 0x146e1 set, or
// fn_7101136308 of its object at 0x278 holds.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent_57a3ac {
    virtual const CClass* vf_0x0();
};
struct UnkInfesterComponent : UnkAIComponent_57a3ac {
    u8 unk_0x8[0x146d9];
    u8 field_0x146e1;
};
struct UnkEntity {
    u8 unk_0x0[0x240];
    UnkAIComponent_57a3ac* component;
    u8 unk_0x248[0x30];
    void* field_0x278;
};
struct UnkNode_57a3ac {
    UnkEntity* entity;
    UnkNode_57a3ac* next;
};
struct UnkSelf_57a3ac {
    u8 unk_0x0[0x18];
    UnkNode_57a3ac* head;
};

// The checked cast to the infester AI component: null when the component is of another class.
static inline UnkInfesterComponent* cast_to_infester(UnkAIComponent_57a3ac* component) {
    return fn_710009d50c(component->vf_0x0(), fn_71005ca678()) ? static_cast<UnkInfesterComponent*>(component) : nullptr;
}

bool fn_710057a3ac(UnkSelf_57a3ac* self) {
    for (UnkNode_57a3ac* node = self->head; node; node = node->next) {
        UnkEntity* entity = node->entity;
        // The original reads the flag through the result of the cast without testing it.
        if (cast_to_infester(entity->component)->field_0x146e1 || fn_7101136308(entity->field_0x278))
            return true;
    }
    return false;
}
