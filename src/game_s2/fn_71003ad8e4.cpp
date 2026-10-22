// Per-frame update that records the current area name: unless the flag at +0x50c of the object at
// +0x14908 is set, passes the owner's actor (when it is a CCharClass) to fn_7100df5cec, then, when
// the owner's object at +0x1b8 has a usable child at +0x48 (running fn_710115fe20 to make one),
// stores fn_710112bf20's string id in the manager's name member at +0x2230.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkActor {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkActor*)>(vtable[0])(this); }
};

struct UnkFlagObject {
    u8 pad_0x0[0x50c];
    u8 field_0x50c;
};

struct UnkChild {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHolder {
    u8 pad_0x0[0x48];
    UnkChild* field_0x48;
};

struct UnkOwner {
    u8 pad_0x0[0x28];
    UnkActor* field_0x28;
    u8 pad_0x30[0x188];
    UnkHolder* field_0x1b8;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
    u8 pad_0x38[0x148d0];
    UnkFlagObject* field_0x14908;
};

struct UnkManager {
    u8 pad_0x0[0x2230];
    CStrId field_0x2230;
};

extern UnkManager* g_unk_7101d47310;
void* fn_71011abbd4();  // CCharClass::refl_GetClass
bool fn_710009d50c(const void* from, void* to);
void fn_7100df5cec(UnkManager* manager, void* target);
void fn_710115fe20(UnkHolder* holder);
CStrId* fn_710112bf20();

// NON_MATCHING(minor): the child pointer tests load into x8 instead of x0 (register choice only)
void fn_71003ad8e4(UnkSelf* self) {
    if (self->field_0x14908->field_0x50c)
        return;
    UnkActor* actor = self->field_0x30->field_0x28;
    if (actor && fn_710009d50c(actor->vf_0x0(), fn_71011abbd4())) {
        UnkManager* manager = g_unk_7101d47310;
        UnkActor* object = self->field_0x30->field_0x28;
        char* cast = nullptr;
        if (object && fn_710009d50c(object->vf_0x0(), fn_71011abbd4()))
            cast = reinterpret_cast<char*>(object);
        fn_7100df5cec(manager, cast + 0x98);
    }
    UnkHolder* holder = self->field_0x30->field_0x1b8;
    if (!holder)
        return;
    if (!holder->field_0x48 || !holder->field_0x48->field_0x10) {
        fn_710115fe20(holder);
        if (!holder->field_0x48 || !holder->field_0x48->field_0x10)
            return;
    }
    UnkManager* manager = g_unk_7101d47310;
    manager->field_0x2230 = *fn_710112bf20();
}
