// Unit 1134 (tools/tu_map.py, no name hint). fn_710070df40: whether a request may replace the
// current entry of the player at +0x28: never without an entry; with flag 0x8000 unless it shares the
// current entry's key at +0x38 with another +0x40; otherwise (flag 2 forces it) unless it is the
// current entry, flag 0x40 is set while the current state (+0x14) is not 1, or the player
// (fn_7100c95ed8) is not done, the request's priority (+0x20) is lower and flag 1 is clear. Names and
// layouts are placeholders.
#include "game/CActionInstance.h"
#include "types.h"

struct UnkEntry_b42 {
    u8 pad_0x0[0x14];
    s32 state;  // 0x14
    u8 pad_0x18[0x8];
    s32 priority;  // 0x20
    u8 pad_0x24[0x14];
    u64 key;  // 0x38
    u64 sub_key;  // 0x40
};

struct UnkPlayer_b42 {
    u8 pad_0x0[0x10];
    UnkEntry_b42* current;  // 0x10
};

struct UnkRequest_b42 {
    UnkEntry_b42* entry;
    u32 flags;
};

struct UnkSelf_1134_b42 {
    u8 pad_0x0[0x28];
    UnkPlayer_b42* player;  // 0x28
};

bool fn_710070df40(UnkSelf_1134_b42* self, UnkRequest_b42* req) {
    UnkEntry_b42* entry = req->entry;
    if (!entry)
        return false;
    if (UnkPlayer_b42* player = self->player) {
        u32 flags = req->flags;
        if (flags & 0x8000) {
            if (player->current->key == entry->key && player->current->sub_key != entry->sub_key)
                return false;
        } else if (!(flags & 2)) {
            if (player->current == entry)
                return false;
            if ((flags & 0x40) && player->current->state != 1)
                return false;
            if (!fn_7100c95ed8(player)) {
                UnkEntry_b42* current = player->current;
                s32 current_priority = current->priority;
                if (req->entry->priority < current_priority && !(req->flags & 1))
                    return false;
            }
        }
    }
    return true;
}
