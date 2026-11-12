// CCharClassPlayEntityTimelineEvent (unit 1826 of tools/tu_map.py, the name from reflection): when
// both of the event's string ids are set (the entity at +0x10, the timeline at +0x18), finds the
// entity in the game manager by the first id's hash and plays the timeline on it
// (fn_7100d7c828, by the second id's hash). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkTimelineNameData {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;
};

struct UnkPlayEntityTimelineEvent {
    u8 unk_0x0[0x10];
    UnkTimelineNameData* entity;
    UnkTimelineNameData* timeline;
};

void fn_7100bc57c0(UnkPlayEntityTimelineEvent* self) {
    UnkTimelineNameData* entity_name = self->entity;
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    if (&entity_name->base == empty || &self->timeline->base == empty)
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = entity_name->hash;
    kind = kind_value;
    UnkWeakTarget* target = fn_7100debbc8(manager, &key, &kind);
    if (!target)
        return;
    key = self->timeline->hash;
    fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &key, false);
}
