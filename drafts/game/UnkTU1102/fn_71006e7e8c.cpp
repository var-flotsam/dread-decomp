// UnkTU1102 (the X parasite AI component's unit): stops the parasite. After the base step and
// fn_71006e808c it lets go of what it grabbed, switches its effect off, resets the owner's timeline to the
// current state's binding, takes the owner out of the player's list and reports it to the game manager.
// Names and layouts are placeholders.
#include <string.h>

#include <algorithm>

#include "game/CFXComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXParasiteAI.h"
#include "types.h"

struct UnkXParasiteKeyed_8208 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};
extern UnkXParasiteKeyed_8208* g_unk_717270b910[2] __attribute__((visibility("hidden")));

struct UnkXParasitePartHandle_8208 {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

extern const u64 g_unk_710164a838;
extern UnkSpawnGroupKey g_unk_717270b8b0 __attribute__((visibility("hidden")));

void fn_71006e808c(UnkXParasiteAI* self);

// The owner's id in the player's list.
struct UnkXParasiteOwnerId_l6e7e8c {
    u8 pad_0x0[0x2a0];
    u64 id;  // 0x2a0
};

// The ids the player keeps (a fixed array with its count in front).
#pragma pack(push, 4)
struct UnkXParasiteIdList_l6e7e8c {
    u8 pad_0x0[0x848];
    u32 count;     // 0x848
    u64 ids[];     // 0x84c, as many as count says

    void Resize(u32 n) {
        if (n > count)
            memset(&ids[count], 0, (u64)(n - count) * sizeof(u64));
        count = n;
    }

    void Remove(u64 id) {
        u64* begin = ids;
        u64* end = begin + count;
        u64* it = std::find(begin, end, id);
        if (it == end)
            return;
        for (u64* next = it + 1; next != begin + count; ++next)
            next[-1] = *next;
        Resize(count - 1);
    }
};
#pragma pack(pop)

struct UnkXParasitePlayer_l6e7e8c {
    u8 pad_0x0[0x2b0];
    UnkXParasiteIdList_l6e7e8c* ids;  // 0x2b0
};

void fn_71006e7e8c(UnkXParasiteAI* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    fn_71006e808c(self);
    if (self->link && self->link->target) {
        fn_7100232c68(self->link->target);
        ResetRef(&self->link);
    }

    if (self->field_0x14724 != 4) {
        if (void* control = self->owner->field_0x280) {
            UnkPart* part = fn_7101152b90(static_cast<UnkWorld*>(control), g_unk_717270b910[0]->key);
            if (part && fn_710114e668(reinterpret_cast<UnkObject*>(control),
                                      reinterpret_cast<UnkXParasitePartHandle_8208*>(part)->field_0xb)) {
                fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
                fn_710115596c(static_cast<UnkRinkaControl_ruc*>(control), part);
            }
        }
    }

    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        fn_7100a2b7b8(timeline, &g_unk_710164a838);
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(CurrentBinding(self)));
    }

    if (UnkXParasitePlayer_l6e7e8c* player =
            reinterpret_cast<UnkXParasitePlayer_l6e7e8c*>(fn_7100defcd4(g_unk_7101d47310))) {
        if (UnkXParasiteIdList_l6e7e8c* ids = player->ids)
            ids->Remove(reinterpret_cast<UnkXParasiteOwnerId_l6e7e8c*>(self->owner)->id);
    }
    fn_7100de9730(g_unk_7101d47310, &g_unk_717270b8b0, self);
}
