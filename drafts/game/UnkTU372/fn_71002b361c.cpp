// Unit 372 (no reliable name): tests a set of condition flags against a keyed owner; the answer is true when the
// key list is not ready, a flag holds, or one of the owner's linked entries reports a lock. Names and layouts are placeholders.
#include "game/CBreakableTileGroup.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU367.h"
#include "game/UnkTU39.h"
#include "types.h"

void* fn_7100ca719c(void* manager, u32 id);
UnkQueryObj* fn_7100ca1ea8(void* entry, u8 sub);
u32 fn_71008d0db4(char* lock);

struct UnkLinkSlot_l2b361c {
    struct Target {
        u8 pad_0x0[0x10];
        UnkWeakTarget* field_0x10;
    };
    Target* target;  // 0x0
    u8 pad_0x8[0x10];
};

struct UnkCondOwner_l2b361c {
    u8 pad_0x0[0x18];
    u8 keys[0x10];  // 0x18
    u8 pad_0x28[0xfc];
    u32 slot_count;  // 0x124
    u8 pad_0x128[0x8];
    UnkLinkSlot_l2b361c slots[1];  // 0x130
    u8 pad_0x148[0xa4];
    u16 field_0x1ec;
    u8 field_0x1ee;
};

// The state of the entry this owner points at, or -1 when there is none.
static inline UnkQueryObj* FindEntryInfo(UnkCondOwner_l2b361c* self) {
    if (!self->field_0x1ec)
        return nullptr;
    void* entry = fn_7100ca719c(g_unk_7101d47310->field_0x1af8, self->field_0x1ec);
    if (!entry)
        return nullptr;
    return fn_7100ca1ea8(entry, self->field_0x1ee);
}

bool fn_71002b361c(UnkCondOwner_l2b361c* self, u32 flags) {
    bool notReady = !fn_710002fb70(self->keys);
    if (notReady)
        return true;

    u32 hit;
    if ((flags & 3) == 2) {
        UnkQueryObj* info = FindEntryInfo(self);
        if (info)
            hit = (flags & 1) | u32(fn_7100c9e960(info) == 1);
        else
            hit = 0;
    } else {
        hit = flags & 1;
    }
    // keeps the flag word unfolded so its low bit is tested, as the original does
    asm("" : "+r"(hit));
    if ((flags & 4) && !(hit & 1)) {
        UnkQueryObj* info = FindEntryInfo(self);
        hit = info && fn_7100c9e960(info) == 7;
    }
    if ((flags & 8) && !(hit & 1)) {
        UnkQueryObj* info = FindEntryInfo(self);
        hit = info && fn_7100c9e960(info) == 2;
    }
    if ((flags & 16) && !(hit & 1)) {
        UnkLinkSlot_l2b361c* slot = self->slots;
        for (UnkLinkSlot_l2b361c* end = slot + self->slot_count; slot != end; ++slot) {
            UnkLinkSlot_l2b361c::Target* t = slot->target;
            if (!t || !t->field_0x10)
                continue;
            if (UnkDoorLock* lock = fn_71002af860(t->field_0x10))
                if (fn_71008d0db4(reinterpret_cast<char*>(lock)) & 1)
                    return true;
        }
    }
    return hit | notReady;
}
