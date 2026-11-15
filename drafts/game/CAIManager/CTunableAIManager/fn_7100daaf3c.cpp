// CTunableAIManager's unit: takes the first active slot of the pool for a kind picked by a hash (one of two
// pool sets by the low bit of the flag), clears its flag, switches its object on and returns it; null if none.
// Names and layouts are placeholders.
#include "game/UnkSwitchObject.h"
#include "types.h"

struct UnkSlot_ldaaf3c {
    UnkSwitchObject* object;
    u8 active;
    u8 pad_0x9[7];
};

struct UnkPool_ldaaf3c {
    u32 count;
    UnkSlot_ldaaf3c slots[63];
};

struct UnkMgr_ldaaf3c {
    u8 pad_0x0[0xbf8];
    UnkPool_ldaaf3c pools_a[3];  // 0xbf8
    UnkPool_ldaaf3c pools_b[3];  // 0x1804
};

UnkSwitchObject* fn_7100daaf3c(UnkMgr_ldaaf3c* self, const u64* hash, u32 flag) {
    u64 kind;
    switch (*hash) {
    case 0x775f496a8fc614bcull:
        kind = 1;
        break;
    case 0x30f533832421eb88ull:
        kind = 0;
        break;
    case 0xc47105d48c614bd3ull:
        kind = 2;
        break;
    default:
        return nullptr;
    }
    UnkPool_ldaaf3c* pools = (flag & 1) ? self->pools_b : self->pools_a;
    UnkPool_ldaaf3c& pool = pools[kind];
    for (u64 i = 0; i < pool.count; ++i) {
        UnkSlot_ldaaf3c& slot = pool.slots[i];
        if (slot.active) {
            slot.active = 0;
            fn_7100145e94(slot.object, true);
            return slot.object;
        }
    }
    return nullptr;
}
