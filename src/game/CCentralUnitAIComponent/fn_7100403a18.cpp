// Runs the base step, clears the flag at 0x146f8 and the word at 0x1472c, then asks the two objects
// the owner's object at +0x220 (+0x30 of the component) has registered under g_unk_71726c40d8 and
// g_unk_71726c40e0 to run their slot 0x80 when they are of the component's class (fn_71002e6474).
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::reflection::CClass;

struct UnkEntry {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkEntry*)>(vtable[0])(this); }
    void vf_0x80() { reinterpret_cast<void (*)(UnkEntry*)>(vtable[16])(this); }
};

struct UnkOwner {
    u8 pad_0x0[0x40];
    void* field_0x40;
};

struct UnkOwnerRef {
    u8 pad_0x0[0x220];
    UnkOwner* field_0x220;
};

struct UnkSelf : UnkBehaviorTreeAI {
    u8 pad_0x38[0x146c0];
    UnkOwnerRef* Owner() const { return static_cast<UnkOwnerRef*>(field_0x30); }
    u8 field_0x146f8;
    u8 pad_0x146f9[0x33];
    u32 field_0x1472c;
};

extern __attribute__((visibility("hidden"))) char g_unk_71726c40d8[];
extern __attribute__((visibility("hidden"))) char g_unk_71726c40e0[];
UnkEntry* fn_71002d57ac(void* registry, const void* key);

void fn_7100403a18(UnkSelf* self) {
    fn_71003695b8(self);
    self->field_0x146f8 = 0;
    self->field_0x1472c = 0;
    if (UnkOwner* owner = self->Owner()->field_0x220) {
        if (UnkEntry* entry = fn_71002d57ac(owner->field_0x40, g_unk_71726c40d8)) {
            if (fn_710009d50c(entry->vf_0x0(), fn_71002e6474()))
                entry->vf_0x80();
        }
        if (UnkEntry* entry = fn_71002d57ac(owner->field_0x40, g_unk_71726c40e0)) {
            if (fn_710009d50c(entry->vf_0x0(), fn_71002e6474()))
                entry->vf_0x80();
        }
    }
}
