// Looks up two entries by key in the owner's collision component (the object at +0x2a0 of the
// owner when it is of that class) and, when both exist, runs fn_7100d35a98 on the second with 0 and
// on the first with 1.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkComponent {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkOwner {
    u8 pad_0x0[0x2a0];
    UnkComponent* field_0x2a0;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

struct UnkEntry;

extern u64 g_unk_71726bd8d0[2] __attribute__((visibility("hidden")));

void fn_7100369650();
UnkEntry* fn_7100864194(UnkComponent* component, const u64* key);
void fn_7100d35a98(UnkEntry* entry, u32 flag);

void fn_71003bb408(UnkSelf* self) {
    fn_7100369650();
    UnkComponent* object = self->field_0x30->field_0x2a0;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710085fc44()))
        component = object;
    UnkEntry* first = fn_7100864194(component, &g_unk_71726bd8d0[0]);
    UnkEntry* second = fn_7100864194(component, &g_unk_71726bd8d0[1]);
    if (first && second) {
        fn_7100d35a98(second, 0);
        fn_7100d35a98(first, 1);
    }
}
