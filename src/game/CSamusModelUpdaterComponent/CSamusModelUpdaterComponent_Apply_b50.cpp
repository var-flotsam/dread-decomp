// CSamusModelUpdaterComponent unit (tools/tu_map.py, the name from reflection): finds the
// CSamusModelUpdaterComponent that goes with this component's actor (+0x30): the player's
// (fn_7100defcd4) when the actor is not the player, else the one of the object at +0x10 of the game
// manager's +0x29a0 entry; it hands it `id` (fn_7100938ebc), then runs fn_710093aad0.
// Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkOwner;

struct UnkRefl_fn_710083a2ac;
extern template base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710083a2ac>(void*);

struct UnkHandle_710093aca8 {
    u8 pad_0x0[0x10];
    void* field_0x10;  // 0x10
};

struct UnkGameManagerHandles_710093aca8 {
    u8 pad_0x0[0x29a0];
    UnkHandle_710093aca8* field_0x29a0;  // 0x29a0
};

void fn_710093aca8(char* self, const u64* id) {
    void* actor = *reinterpret_cast<void**>(self + 0x30);
    void* player = fn_7100defcd4(g_unk_7101d47310);
    UnkGameManager* manager = g_unk_7101d47310;
    void* owner;
    if (actor != player) {
        owner = fn_7100defcd4(manager);
    } else {
        UnkHandle_710093aca8* entry = reinterpret_cast<UnkGameManagerHandles_710093aca8*>(manager)->field_0x29a0;
        owner = entry ? entry->field_0x10 : nullptr;
    }
    if (owner != nullptr) {
        if (void* updater = base::reflection::refl_FindTyped<UnkRefl_fn_710083a2ac>(owner))
            CMultiModelUpdaterComponent_SelectModelByName(updater, id);
    }
    fn_710093aad0(self);
}
