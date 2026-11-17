// CCharClassBaseTriggerComponent's unit (tools/tu_map.py unit 1562): refreshes the list of actors inside
// the trigger (+0x3ac) against the ones inside now (virtual 0x1e0): disabled actors are dropped, ones still
// inside are updated (virtual 0x1b0), new ones are added (virtual 0x1a8) and ones gone are removed
// (virtual 0x1b8). Names and layouts are placeholders.
#include "types.h"

struct UnkTriggerActor_la47950 {
    u8 pad_0x0[0x14a];
    bool enabled;  // 0x14a
};

struct UnkTriggerInfo_la47950 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkTriggerInfo_la47950(const UnkTriggerInfo_la47950& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8) {}
    UnkTriggerInfo_la47950& operator=(const UnkTriggerInfo_la47950& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        return *this;
    }
};

struct UnkTriggerEntry_la47950 {
    UnkTriggerActor_la47950* actor;
    UnkTriggerInfo_la47950 info;  // 0x8
};

// A count and room for N entries.
template <u32 N>
struct UnkTriggerEntryList_la47950 {
    u32 count;
    u8 storage[N * sizeof(UnkTriggerEntry_la47950)];

    UnkTriggerEntry_la47950* items() { return reinterpret_cast<UnkTriggerEntry_la47950*>(storage); }
    UnkTriggerEntry_la47950* end() { return &items()[count]; }
    bool Full() const { return count >= N; }

    void Add(const UnkTriggerEntry_la47950& entry) {
        items()[count] = entry;
        ++count;
    }

    void Resize(u32 n) {
        if (n > count) {
            UnkTriggerEntry_la47950* first = end();
            UnkTriggerEntry_la47950* last = &items()[n];
            __builtin_memset(first, 0, (last - first) * sizeof(UnkTriggerEntry_la47950));
        }
        count = n;
    }

    UnkTriggerEntry_la47950* Find(const UnkTriggerActor_la47950* actor) {
        UnkTriggerEntry_la47950* it = items();
        UnkTriggerEntry_la47950* last = end();
        while (it != last && it->actor != actor)
            ++it;
        return it;
    }

    // Moves the last entry into `pos` and drops the last.
    void Erase(UnkTriggerEntry_la47950* pos) {
        UnkTriggerEntry_la47950* last = end() - 1;
        if (pos != last)
            *pos = *last;
        Resize(count - 1);
    }
};

using UnkTriggerActorList_la47950 = UnkTriggerEntryList_la47950<64>;

struct UnkTriggerComponent_la47950;

struct UnkTriggerComponentVtbl_la47950 {
    void* pad_0x0[0x1a8 / 8];
    void (*vf_0x1a8)(UnkTriggerComponent_la47950* self, UnkTriggerActor_la47950* actor,
                     UnkTriggerInfo_la47950* info);  // entered
    void (*vf_0x1b0)(UnkTriggerComponent_la47950* self, UnkTriggerActor_la47950* actor);  // still inside
    void (*vf_0x1b8)(UnkTriggerComponent_la47950* self, UnkTriggerActor_la47950* actor,
                     UnkTriggerInfo_la47950* info);  // left
    void* pad_0x1c0[(0x1e0 - 0x1c0) / 8];
    void (*vf_0x1e0)(UnkTriggerComponent_la47950* self, UnkTriggerActorList_la47950* out);  // inside now
};

struct UnkTriggerComponent_la47950 {
    UnkTriggerComponentVtbl_la47950* vtable;
    u8 pad_0x8[0x3a4];
    UnkTriggerActorList_la47950 actors;  // 0x3ac

    void vf_0x1a8(UnkTriggerActor_la47950* actor, UnkTriggerInfo_la47950* info) {
        vtable->vf_0x1a8(this, actor, info);
    }
    void vf_0x1b0(UnkTriggerActor_la47950* actor) { vtable->vf_0x1b0(this, actor); }
    void vf_0x1b8(UnkTriggerActor_la47950* actor, UnkTriggerInfo_la47950* info) {
        vtable->vf_0x1b8(this, actor, info);
    }
    void vf_0x1e0(UnkTriggerActorList_la47950* out) { vtable->vf_0x1e0(this, out); }

    // Takes the actor's entry out of the list and reports it gone.
    void Remove(UnkTriggerActor_la47950* actor) {
        UnkTriggerEntry_la47950* pos = actors.Find(actor);
        UnkTriggerEntry_la47950 removed = *pos;
        actors.Erase(pos);
        vf_0x1b8(removed.actor, &removed.info);
    }
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la47950, actors) == 0x3ac, "layout");

void fn_7100a47950(UnkTriggerComponent_la47950* self) {
    UnkTriggerActorList_la47950 inside;
    inside.count = 0;
    self->vf_0x1e0(&inside);
    UnkTriggerActorList_la47950 left;
    left.count = 0;

    UnkTriggerActorList_la47950& actors = self->actors;
    for (UnkTriggerEntry_la47950* it = actors.items(); it != actors.end();) {
        UnkTriggerActor_la47950* actor = it->actor;
        if (actor && !actor->enabled) {
            actors.Erase(it);
        } else {
            UnkTriggerEntry_la47950* found = inside.Find(actor);
            if (found != inside.end()) {
                it->info = found->info;
                inside.Erase(found);
                self->vf_0x1b0(actor);
            } else {
                left.Add(*it);
            }
            ++it;
        }
    }

    for (UnkTriggerEntry_la47950* entry = inside.items(); entry != inside.end(); ++entry) {
        if (!actors.Full())
            actors.Add(*entry);
        self->vf_0x1a8(entry->actor, &entry->info);
    }

    for (UnkTriggerEntry_la47950* entry = left.items(); entry != left.end(); ++entry)
        self->Remove(entry->actor);
}
