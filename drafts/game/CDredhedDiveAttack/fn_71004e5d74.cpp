// CDredhedDiveAttack's unit, fn_71004e5d74: whether the owner's animation component is playing one of the
// two dive tracks (by the hash of the id it holds under the usual track kind). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A pooled string as this id reads it: a plain (not union) reference count, so the pool is not
// reloaded after the count changes.
struct UnkDredhedString_l4e5d74 {
    u8 pad_0x0[0x8];
    s32 ref_count;
    u8 pad_0xc[0x24];
    u64 hash;
};

// A copied string id whose destructor does not test for a null id.
struct UnkDredhedId_l4e5d74 {
    UnkDredhedString_l4e5d74* data;
    UnkDredhedId_l4e5d74(const UnkDredhedId_l4e5d74& other) : data(other.data) {
        if (Empty(g_unk_7101d47108) != data)
            ++data->ref_count;
    }
    ~UnkDredhedId_l4e5d74() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != Empty(pool) && --data->ref_count == 0)
                StringPool_Release(pool, reinterpret_cast<UnkStringData*>(data));
        }
    }
    static UnkDredhedString_l4e5d74* Empty(UnkStringPool* pool) {
        return reinterpret_cast<UnkDredhedString_l4e5d74*>(&pool->empty);
    }
};

struct UnkDredhedOwner_l4e5d74 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkDredhedDiveAttack_l4e5d74 {
    u8 pad_0x0[0x8];
    UnkDredhedOwner_l4e5d74* owner;
};

bool fn_71004e5d74(UnkDredhedDiveAttack_l4e5d74* self) {
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (!anim)
        return false;
    UnkDredhedId_l4e5d74 id(*reinterpret_cast<const UnkDredhedId_l4e5d74*>(fn_7101177ae8(anim, 0xf4061ff25a2163d6)));
    u64 hash = id.data->hash;
    return hash == 0x3449ef4cb8152ea6 || hash == 0x80486e0c85027bfd;
}
