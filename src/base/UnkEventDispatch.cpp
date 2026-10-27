// An event dispatch of unit 42 (tools/tu_map.py; no class hint, the unit's initialiser registers the
// enums EEventProps and TEventPropsFlags): sends an event's data to the listeners of a set whose flags
// the event's mask allows (fn_7100068bec). Names and layouts are placeholders inferred from the code.
#include "types.h"

// The event's data, handed to each listener by value (its copy is not trivial).
struct UnkEventData {
    u64 field_0x0;
    u64 field_0x8;

    UnkEventData(const UnkEventData& other) : field_0x0(other.field_0x0), field_0x8(other.field_0x8) {}
};

struct UnkEventListener {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkEventData data);  // receives the event
    union {
        u32 flags;  // 0x100, 0x200: excluded from the passes below
        u8 kinds;   // the low byte: the kinds of event it listens to
    };
};

// A run of listeners: the first and the number of them.
struct UnkEventRange {
    UnkEventListener** first;
    s64 count;

    UnkEventRange(const UnkEventRange& other) : first(other.first), count(other.count) {}
};

struct UnkEventSet {
    UnkEventListener** listeners;  // the listeners an event can start part-way through
    u8 pad_0x8[0x8];
    u32 count;
    u8 pad_0x14[0x64];
    UnkEventListener** global;  // +0x78: listeners every event reaches
    u32 global_count;
};

// The listeners of `set` from `first` to the end (fn_7100069a04).
UnkEventRange fn_7100069a04(UnkEventSet* set, UnkEventListener** first);

struct UnkEventDispatch {
    UnkEventSet* set;
    UnkEventData data;
    UnkEventListener** first;  // where the set's own listeners start
    u8 flags;  // 1: reach the set's listeners; 8: the global ones
    u8 pad_0x21[0x3];
    u8 mask;  // the kinds the event is for
    u8 pad_0x25[0x3];
    bool enabled;
};

static inline void Send(UnkEventDispatch* self, UnkEventListener* listener, u8 pass) {
    if (self->enabled && (self->flags & pass) && (listener->kinds & (u8)~self->mask) == 0)
        listener->vf_0x18(self->data);
}

// Sends the event to the set's listeners from `first` on (those without 0x100) and to the global ones;
// with `exclusive`, listeners with 0x200 are skipped in both passes.
void fn_7100068bec(UnkEventDispatch* self, bool exclusive) {
    UnkEventSet* set = self->set;
    if (!set)
        return;
    if (set->count) {
        UnkEventRange range = fn_7100069a04(set, self->first);
        for (UnkEventListener **it = range.first, **end = range.first + range.count; it != end; ++it) {
            UnkEventListener* listener = *it;
            if (listener->flags & 0x100)
                continue;
            if (exclusive && (listener->flags & 0x200))
                continue;
            Send(self, listener, 1);
        }
        set = self->set;
        if (!set)
            return;
    }
    for (UnkEventListener **it = set->global, **end = set->global + set->global_count; it != end; ++it) {
        if (exclusive && ((*it)->flags & 0x200))
            continue;
        Send(self, *it, 8);
    }
}
