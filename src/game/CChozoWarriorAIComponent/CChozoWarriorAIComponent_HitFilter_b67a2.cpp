// CChozoWarriorAIComponent unit (tools/tu_map.py, the name from reflection): a hit filter. When the hit target's
// +0x10 object has a kind (+0x20) equal to the one at 0x71726d1260 and the hit info's flags (+0xb5) have 0x50
// set, it writes 0 to *out and returns 1; for the kind at 0x71726d1268 it writes 2 and returns 1; otherwise it
// defers to fn_71011061ac. Names and layouts are placeholders.
#include "types.h"

// As src/game/CGoliathAIComponent/CGoliathAIComponent_HitFilter_b55.cpp declares it.
struct UnkGoliathAI_b55;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;
u32 fn_71011061ac(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkGoliathHitInfo_b55* info,
                  UnkGoliathHitTarget_b55** hit);

struct UnkChozoWarriorKind_b67a2;
extern UnkChozoWarriorKind_b67a2* g_unk_71726d1260 __attribute__((visibility("hidden")));
extern UnkChozoWarriorKind_b67a2* g_unk_71726d1268 __attribute__((visibility("hidden")));

struct UnkChozoWarriorHitObject_b67a2 {
    u8 pad_0x0[0x20];
    UnkChozoWarriorKind_b67a2* kind;
};

struct UnkChozoWarriorHitTarget_b67a2 {
    u8 pad_0x0[0x10];
    UnkChozoWarriorHitObject_b67a2* object;
};

struct UnkChozoWarriorHitInfo_b67a2 {
    u8 pad_0x0[0xb5];
    u8 flags;
};

u32 fn_710049abf8(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkChozoWarriorHitInfo_b67a2* info,
                  UnkChozoWarriorHitTarget_b67a2** hit) {
    if (UnkChozoWarriorHitTarget_b67a2* target = *hit) {
        if (UnkChozoWarriorHitObject_b67a2* object = target->object) {
            UnkChozoWarriorKind_b67a2* kind = object->kind;
            if (kind == g_unk_71726d1260) {
                if (info->flags & 0x50) {
                    *out = 0;
                    return 1;
                }
            } else if (kind == g_unk_71726d1268) {
                *out = 2;
                return 1;
            }
        }
    }
    return fn_71011061ac(self, out, arg2, reinterpret_cast<UnkGoliathHitInfo_b55*>(info),
                         reinterpret_cast<UnkGoliathHitTarget_b55**>(hit));
}
