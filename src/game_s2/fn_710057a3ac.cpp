// Whether any entry of the list has an infester AI component with its flag at 0x146e1 set, or
// fn_7101136308 of its object at 0x278 holds.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent {
    virtual const CClass* vf_0x0();
};
struct UnkInfesterComponent : UnkAIComponent {
    u8 unk_0x8[0x146d9];
    u8 field_0x146e1;
};
struct UnkEntity {
    u8 unk_0x0[0x240];
    UnkAIComponent* component;
    u8 unk_0x248[0x30];
    void* field_0x278;
};
struct UnkNode {
    UnkEntity* entity;
    UnkNode* next;
};
struct UnkSelf {
    u8 unk_0x0[0x18];
    UnkNode* head;
};

const CClass* fn_71005ca678();  // CInfesterAIComponent::refl_GetClass
bool fn_7101136308(void* a0);

bool fn_710057a3ac(UnkSelf* self) {
    for (UnkNode* node = self->head; node; node = node->next) {
        UnkEntity* entity = node->entity;
        UnkAIComponent* component = entity->component;
        UnkInfesterComponent* infester = nullptr;
        if (fn_710009d50c(component->vf_0x0(), fn_71005ca678()))
            infester = static_cast<UnkInfesterComponent*>(component);
        // The original reads the flag through the (possibly null) result of the cast; volatile
        // keeps the compiler from assuming it is non-null.
        if (*reinterpret_cast<volatile u8*>(reinterpret_cast<u8*>(infester) + 0x146e1) ||
            fn_7101136308(entity->field_0x278))
            return true;
    }
    return false;
}
