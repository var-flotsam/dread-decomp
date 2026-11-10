// CTimelineComponent unit (tools/tu_map.py): plays a random timeline event of the owner's
// CCharClass (its events list at +0xf8, held by reference while in use), never the one played last
// (+0x64) when there is a choice. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern "C" int rand();
void fn_7100a2c200(UnkTimelineComponent* self, void* entry, __attribute__((noescape)) const u32* value);

struct UnkTimelineEventNode_ovb0089 {
    u8 pad_0x0[0x10];
    u8 entry[0x1c0];
    UnkTimelineEventNode_ovb0089* next;  // 0x1d0
};

struct UnkTimelineEvents_ovb0089 : UnkCountedResource {
    u8 pad_0x58[0x74 - 0x58];
    s32 count;  // 0x74
    u8 pad_0x78[0xa8 - 0x78];
    UnkTimelineEventNode_ovb0089* head;  // 0xa8
};

struct UnkTimelineCharClass_ovb0089 : UnkReflObject {
    u8 pad_0x8[0xf8 - 0x8];
    UnkTimelineEvents_ovb0089* events;  // 0xf8
};

struct UnkTimelineOwner_ovb0089 {
    u8 pad_0x0[0x28];
    UnkReflObject* def;
};

struct UnkTimelineRandom_ovb0089 {
    u8 pad_0x0[0x30];
    UnkTimelineOwner_ovb0089* owner;
    u8 pad_0x38[0x64 - 0x38];
    s32 last;  // 0x64
};

void fn_7100a2b9a8(UnkTimelineRandom_ovb0089* self, const u32* value) {
    UnkReflObject* def = self->owner->def;
    UnkTimelineCharClass_ovb0089* cc = fn_710009d50c(def->vf_0x0(), fn_71011abbd4())
                                           ? static_cast<UnkTimelineCharClass_ovb0089*>(def)
                                           : nullptr;
    UnkTimelineEvents_ovb0089* events = cc->events;
    if (events)
        ++events->ref_count;
    s32 count = events->count;
    s32 pick = count - 1;
    if (pick != 0)
        pick = (rand() & 0x7fff) % pick;
    if (self->last == pick)
        pick = (pick + 1) % count;
    UnkTimelineEventNode_ovb0089* node = events->head;
    if (pick && node) {
        u32 left = pick - 1;
        for (;;) {
            node = node->next;
            if (left-- == 0 || !node)
                break;
        }
    }
    {
        u8* entry = node->entry;
        alignas(8) u32 v = *value;
        fn_7100a2c200(reinterpret_cast<UnkTimelineComponent*>(self), entry, &v);
    }
    self->last = pick;
    if (events && --events->ref_count == 0)
        fn_710001737c(events);
}
