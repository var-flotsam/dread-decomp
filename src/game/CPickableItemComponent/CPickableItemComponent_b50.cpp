// In the CPickableItemComponent unit (unit 1493, name from reflection): fn_71009d0184 runs the base
// step (fn_71009cd5c4), clears +0x1b4, registers the object with the game manager under the id
// g_unk_717273ffc0 (fn_7100de9540) and hands the member at +0x168 a counted copy of the owner's
// (+0x18) name id at +0x30 (fn_71009d023c). Names and layouts are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"


extern u64 g_unk_717273ffc0 __attribute__((visibility("hidden")));
void fn_71009cd5c4(char*);
struct UnkRefl_fn_71009d023c;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_71009d023c>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

void fn_71009d0184(char* self) {
    fn_71009cd5c4(self);
    *reinterpret_cast<u32*>(self + 0x1b4) = 0;
    fn_7100de9540(g_unk_7101d47310, &g_unk_717273ffc0, reinterpret_cast<UnkWeakTarget*>(self));
    char* m = self + 0x168;
    base::global::UnkStringData* d =
        *reinterpret_cast<base::global::UnkStringData* const*>(*reinterpret_cast<char**>(self + 0x18) + 0x30);
    {
        union Id {
            base::global::CStrId id;
            Id() {}
            ~Id() { id.~CStrId(); }
        } u;
        u.id.data = d;
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        base::reflection::refl_RegistryMapInit<UnkRefl_fn_71009d023c>(
            reinterpret_cast<base::reflection::UnkReflRegistryMap*>(m), u.id);
    }
}

// fn_71009d377c: whether the player's inventory (+0x270) holds the item named by the id at +0x98,
// when the flag at +0x129 is set (built through a string-id handle, fn_7100ae986c).
struct UnkPlayer;
struct UnkInventory;
struct UnkStrIdHandle;
using base::global::CStrId;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

bool fn_71009d377c(char* self) {
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player || !self[0x129])
        return false;
    UnkInventory* inventory = *reinterpret_cast<UnkInventory**>(reinterpret_cast<char*>(player) + 0x270);
    alignas(UnkQualifiedId) char storage[sizeof(UnkQualifiedId)];
    UnkQualifiedId& q = *reinterpret_cast<UnkQualifiedId*>(storage);
    fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(&q), *reinterpret_cast<const base::global::CStrId*>(self + 0x98));
    bool r = fn_71011428bc(inventory, q);
    q.id.~CStrId();
    return r;
}
