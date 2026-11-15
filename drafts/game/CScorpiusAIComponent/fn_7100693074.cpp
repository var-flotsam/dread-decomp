// CScorpiusAIComponent's unit: tells the manager about the component, then removes its target's id from the
// owner's id list. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2730.h"
#include "types.h"

struct UnkScorpiusTarget_l693074 {
    u8 pad_0x0[0x2a0];
    u64 id;  // 0x2a0
};

struct UnkScorpiusAI_l693074 {
    u8 pad_0x0[0x30];
    UnkScorpiusTarget_l693074* target;  // 0x30
};

struct __attribute__((packed)) UnkIdList_l693074 {
    u8 pad_0x0[0x848];
    u32 count;     // 0x848
    u64 ids[1];    // 0x84c
};

struct UnkIdOwner_l693074 {
    u8 pad_0x0[0x2b0];
    UnkIdList_l693074* list;  // 0x2b0
};

void fn_7100693074(UnkScorpiusAI_l693074* self) {
    fn_7101199818(g_unk_7101d47330, self);
    UnkIdOwner_l693074* owner = reinterpret_cast<UnkIdOwner_l693074*>(fn_7100defcd4(g_unk_7101d47310));
    if (!owner)
        return;
    UnkIdList_l693074* list = owner->list;
    if (!list)
        return;
    u64* begin = list->ids;
    u64* end = begin + list->count;
    u64* it = begin;
    for (; it != end; ++it) {
        if (*it == self->target->id)
            break;
    }
    if (it == end)
        return;
    for (u64* next = it + 1; next != list->ids + list->count; ++next)
        next[-1] = *next;
    u32 newCount = list->count - 1;
    if (list->count == 0)
        __builtin_memset(list->ids, 0, (u64)newCount * 8);
    list->count = newCount;
}
