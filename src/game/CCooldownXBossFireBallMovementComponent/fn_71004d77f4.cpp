// A cooldown-boss component's update step: if it is active, lets the owner's boss AI know and
// runs the base update with the same arguments.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkBossAI {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkBossAI*)>(vtable[0])(this); }
};

struct UnkBossHolder {
    u8 pad_0x0[0x10];
    struct UnkBossSub* field_0x10;
};

struct UnkBossSub {
    u8 pad_0x0[0x240];
    UnkBossAI* field_0x240;
};

struct UnkBossComponent {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x1f8];
    UnkBossHolder* field_0x230;
    u8 pad_0x238[0x3a0];
    u32 field_0x5d8;
};

void fn_71004d43dc(UnkBossAI* ai, void* target);
void fn_7100232c68(void* target);
bool fn_7100ac4c60(UnkBossComponent* self, u64 a, u64 b, u64 c, u64 d);

bool fn_71004d77f4(UnkBossComponent* self, u64 a, u64 b, u64 c, u64 d) {
    if (!self->field_0x5d8)
        return false;
    UnkBossHolder* holder = self->field_0x230;
    if (holder && holder->field_0x10) {
        UnkBossAI* ai = holder->field_0x10->field_0x240;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318()))
            fn_71004d43dc(ai, self->field_0x30);
    }
    fn_7100232c68(self->field_0x30);
    return fn_7100ac4c60(self, a, b, c, d);
}
