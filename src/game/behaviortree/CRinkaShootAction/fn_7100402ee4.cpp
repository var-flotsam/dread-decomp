// Starts the central unit's reaction: -1 request id gives 6, otherwise the central unit AI
// component of the owner at 0x18 (when it is one) is handed to fn_7100405d58, which gives 5.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCentralUnitAIComponent.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent {
    virtual const CClass* vf_0x0();
};
struct UnkOwner_7100402ee4 {
    u8 unk_0x0[0x240];
    UnkAIComponent* component;
};
struct UnkSelf_7100402ee4 {
    u8 unk_0x0[0x18];
    UnkOwner_7100402ee4* owner;
    u8 unk_0x20[0xc];
    s32 iSpawnPointIndex;
    u32 eRinkaType;
};

u32 CRinkaShootAction_ForwardSpawnPointAndTypeToCentralUnitAI(UnkSelf_7100402ee4* self) {
    if (self->iSpawnPointIndex < 0)
        return 6;
    UnkAIComponent* component = self->owner->component;
    UnkAIComponent* found = nullptr;
    if (component && fn_710009d50c(component->vf_0x0(), fn_71004030f4()))
        found = component;
    fn_7100405d58(found, self->iSpawnPointIndex, self->eRinkaType);
    return 5;
}
