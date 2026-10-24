// Passes the object to the global manager (fn_7100de9540) under a fixed name, runs fn_7100ac3880
// and takes the "damage" entry's value of its class definition (the component is used when its
// kind is 2, read without a null check as in the original). Then the counter at 0x600 is cleared and the
// flag at 0x64 of the object at 0x1b8 set.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkDamageComponent {
    u8 unk_0x0[0xa8];
    u32 field_0xa8;
};
struct UnkComponentHolder {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};
struct UnkDef {
    u8 unk_0x0[0x158];
    UnkComponentHolder* field_0x158;
};
struct UnkSubObject {
    u8 unk_0x0[0x64];
    u8 field_0x64;
};
struct UnkOwner {
    u8 unk_0x0[0x1b8];
    UnkSubObject* field_0x1b8;
    u8 unk_0x1c0[0xe0];
    void* field_0x2a0;
};
struct UnkSelf {
    u8 unk_0x0[0x30];
    UnkOwner* owner;
    u8 unk_0x38[0x5b8];
    u32 field_0x5f0;
    u8 unk_0x5f4[0xc];
    u32 field_0x600;
};

extern char g_unk_71726c9e08[] __attribute__((visibility("hidden")));
void fn_7100de9540(UnkGameManager* manager, const char* name, UnkSelf* self);
void fn_7100ac3880(UnkSelf* self);
UnkDef* fn_71008640a4(void* class_def, const CStrId* id);

void fn_7100459e6c(UnkSelf* self) {
    fn_7100de9540(g_unk_7101d47310, g_unk_71726c9e08, self);
    fn_7100ac3880(self);
    void* class_def = self->owner->field_0x2a0;
    {
        CStrId id("damage", true);
        UnkComponentHolder* holder = fn_71008640a4(class_def, &id)->field_0x158;
        UnkDamageComponent* component =
            holder->vf_0x18() == 2 ? reinterpret_cast<UnkDamageComponent*>(holder) : nullptr;
        self->field_0x5f0 = component->field_0xa8;
    }
    self->field_0x600 = 0;
    if (UnkSubObject* sub = self->owner->field_0x1b8)
        sub->field_0x64 = 1;
}
