// Resets the three special-burst rocket ball effects on the object at +0x280 of the owner's owner
// (looked up by the hash of their names), then, when the handle at +0x400 is still current (its
// serial at +0x8 equals the one at +0x408, which is not -1), runs fn_7100072328 on its target with
// 0.2 and 0 and clears the handle (copies the null handle).
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkTU50.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkObject;

// The actor's object at +0x280: the world the effects are in.
struct UnkOwnerObject {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkWorld;

void fn_71003053c4(CAbilityEnergyWave* self) {
    fn_710114fe8c(reinterpret_cast<UnkWorld*>(self->component->Owner<UnkOwnerObject>()->field_0x280),
                  fn_7100001570("specialburst_rocketball", 23), 0, 0);
    fn_710114fe8c(reinterpret_cast<UnkWorld*>(self->component->Owner<UnkOwnerObject>()->field_0x280),
                  fn_7100001570("specialburst_rocketballcenter", 29), 0,
                  0);
    fn_710114fe8c(reinterpret_cast<UnkWorld*>(self->component->Owner<UnkOwnerObject>()->field_0x280),
                  fn_7100001570("specialburst_rocketballcharging", 31), 0,
                  0);
    UnkEnergyWaveHandle* handle = &self->handle;
    if (handle->target && handle->target->serial == handle->serial && handle->serial != ~0ull &&
        handle->target->object) {
        fn_7100072328(handle->target->object, 0.2f, 0.0f);
        // The null handle, declared as game/UnkTU50.h's UnkHandle.
        *handle = reinterpret_cast<const UnkEnergyWaveHandle&>(g_unk_7101d586c0);
    }
}
