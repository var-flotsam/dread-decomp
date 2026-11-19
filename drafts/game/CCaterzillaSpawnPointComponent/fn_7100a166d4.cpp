// CCaterzillaSpawnPointComponent's unit (tools/tu_map.py unit 1527): whether the spawn point may spawn. After
// the base check, a pending spawn answers by whether the point is enabled; otherwise the owner or the linked
// spawn point's owner must be usable, and then the owner or one of the linked entities. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAutsharpSpawn_la141e8;
bool fn_710112a558(UnkAutsharpSpawn_la141e8* self);

// The resolved link's weak handle: the entity it holds at +0x10.
struct UnkCaterzillaLinkHandle_la166d4 {
    u8 pad_0x0[0x10];
    UnkReflObject* entity;  // 0x10
};

struct UnkCaterzillaSpawn_la166d4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x38c];
    u32 field_0x3c4;
    u8 pad_0x3c8[4];
    u32 field_0x3cc;
    bool field_0x3d0;
    u8 pad_0x3d1[0xb];
    bool field_0x3dc;
    u8 pad_0x3dd;
    bool field_0x3de;
    u8 pad_0x3df[0x41];
    UnkCaterzillaSpawn_la166d4* linked;  // 0x420
    u8 pad_0x428[0x10];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x438
};
static_assert(__builtin_offsetof(UnkCaterzillaSpawn_la166d4, field_0x3cc) == 0x3cc, "layout");
static_assert(__builtin_offsetof(UnkCaterzillaSpawn_la166d4, links) == 0x438, "layout");

// Resolves the link once (while its state is 1) and returns the entity it holds, or null.
inline UnkReflObject* CaterzillaLinkEntity_la166d4(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    UnkCaterzillaLinkHandle_la166d4* handle = reinterpret_cast<UnkCaterzillaLinkHandle_la166d4*>(link.owner);
    return handle ? handle->entity : nullptr;
}

bool fn_7100a166d4(UnkCaterzillaSpawn_la166d4* self) {
    if (!fn_710112a558(reinterpret_cast<UnkAutsharpSpawn_la141e8*>(self)))
        return false;
    bool enabled = self->field_0x3de && self->field_0x3dc && self->field_0x3d0;
    bool pending = self->field_0x3cc != self->field_0x3c4 && self->field_0x3cc != 0;
    bool result = enabled && pending;
    if (pending || !enabled)
        return result;
    if (self->linked && !fn_7100d7bea0(self->owner, 0)) {
        result = fn_7100d7bea0(self->linked->owner, 0);
        if (!result)
            return result;
    }
    if (self->links.count == 0 || fn_7100d7bea0(self->owner, 0))
        return true;

    result = false;
    for (UnkCallbackSet_7101a20950 *it = self->links.data, *end = it + self->links.count; it != end; ++it) {
        UnkCallbackSet_7101a20950 link(*it);
        UnkReflObject* entity = CaterzillaLinkEntity_la166d4(link);
        if (entity && fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()) &&
            fn_7100d7bea0(reinterpret_cast<UnkActor*>(entity), 0)) {
            result = true;
            break;
        }
    }
    return result;
}