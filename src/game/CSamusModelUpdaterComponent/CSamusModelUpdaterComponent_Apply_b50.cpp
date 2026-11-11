// CSamusModelUpdaterComponent unit (tools/tu_map.py, the name from reflection): finds the
// CSamusModelUpdaterComponent that goes with this component's actor (+0x30): the player's
// (fn_7100defcd4) when the actor is not the player, else the one of the object at +0x10 of the game
// manager's +0x29a0 entry; it hands it `id` (fn_7100938ebc), then runs fn_710093aad0.
// Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager* manager);
void fn_7100938ebc(void* updater, const u64* id);

struct UnkRefl_fn_710083a2ac;
extern template base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710083a2ac>(void*);

void fn_710093aca8(char* self, const u64* id) {
    void* actor = *reinterpret_cast<void**>(self + 0x30);
    void* player = fn_7100defcd4(g_unk_7101d47310);
    UnkGameManager* manager = g_unk_7101d47310;
    void* owner;
    if (actor != player) {
        owner = fn_7100defcd4(manager);
    } else {
        char* entry = *reinterpret_cast<char**>(reinterpret_cast<char*>(manager) + 0x29a0);
        owner = entry ? *reinterpret_cast<void**>(entry + 0x10) : nullptr;
    }
    if (owner != nullptr) {
        if (void* updater = base::reflection::refl_FindTyped<UnkRefl_fn_710083a2ac>(owner))
            fn_7100938ebc(updater, id);
    }
    fn_710093aad0(self);
}
