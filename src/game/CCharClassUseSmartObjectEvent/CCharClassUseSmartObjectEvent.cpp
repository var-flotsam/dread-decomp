// A function in the CCharClassUseSmartObjectEvent unit: fn_7100ce7c3c, for an actor that is set, lets the
// first of its two components that exists (fn_71007cfd8c: slot 0x250; fn_7100a81ed0: slot 0x178) handle the
// event with null, else hands the event to the actor's object at +0x200 (fn_71010d99d0). Names and layouts
// are placeholders.
#include "game/CElectricReactionComponent.h"
#include "game/CElevatorUsableComponent.h"
#include "types.h"

struct UnkSmartObjectUserA {
    void** vtable;
    void vf_0x250(void* arg) { reinterpret_cast<void (*)(UnkSmartObjectUserA*, void*)>(vtable[0x250 / 8])(this, arg); }
};

struct UnkSmartObjectUserB {
    void** vtable;
    void vf_0x178(void* arg) { reinterpret_cast<void (*)(UnkSmartObjectUserB*, void*)>(vtable[0x178 / 8])(this, arg); }
};

struct UnkSmartObjectActor {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

void fn_71010d99d0(void* object, void* event);

void fn_7100ce7c3c(void* self, UnkSmartObjectActor* const* actor) {
    if (!*actor)
        return;
    UnkSmartObjectUserA* a = fn_71007cfd8c(*actor);
    UnkSmartObjectUserB* b = fn_7100a81ed0(*actor);
    if (a)
        a->vf_0x250(nullptr);
    else if (b)
        b->vf_0x178(nullptr);
    else if (void* object = (*actor)->field_0x200)
        fn_71010d99d0(object, self);
}
