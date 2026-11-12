// CLifeComponent's unit: returns the object the component's cached weak handle (+0x48) refers to,
// looking it up through the owner's id the first time. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSpawnedActor_ovb0065;

struct UnkLifeOwner_l15fe20 {
    u8 pad_0x0[0x30];
    UnkSpawnedActor_ovb0065* owner;  // 0x30
    u8 pad_0x38[0x10];
    UnkWeakHandle* handle;  // 0x48
};

// NON_MATCHING(minor): the GOT load of the camera kind and the load of the key swap places (two instructions)
void* fn_710115fe20(UnkLifeOwner_l15fe20* self) {
    UnkWeakHandle* handle = self->handle;
    if (!handle || !handle->target) {
        handle = nullptr;
        const base::global::CStrId* id = fn_7100d7ba84(self->owner);
        if (reinterpret_cast<const void* const&>(*id) != &g_unk_7101d47108->empty) {
            UnkGameManager* manager = g_unk_7101d47310;
            const u32* kind_source = &g_unk_71727bebb8;
            u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(reinterpret_cast<const void* const&>(*id)) + 0x30);
            alignas(8) u32 kind = *kind_source;
            if (void* entry = fn_7100deba8c(manager, &key, &kind)) {
                if (UnkSpawnRegistered_ovb0065* reg = fn_7100360044(entry))
                    handle = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(reg));
            }
        }
        AssignRef(&self->handle, handle);
    }
    return handle ? handle->target : nullptr;
}
