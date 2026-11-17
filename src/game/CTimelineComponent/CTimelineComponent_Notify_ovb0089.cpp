// CTimelineComponent unit (tools/tu_map.py): two message handlers that tell the component's listeners
// (the lists at +0xf8, walked under their counter; fn_7100a2e4a0 ends the walk) about an event: a kind
// (3 or 2), a value and a name id copied from the message's timeline. A listener without a callback, or
// whose callback returns false, is removed (fn_7100a2e30c). Names and layouts are placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CActionInstance.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU1134.h"
#include "types.h"

using base::global::CStrId;

// A string id whose release decrements and tests in two steps (the original reloads the id and
// its count after the decrement).
struct UnkEventName_ovb0089 {
    base::global::UnkStringData* data;
    UnkEventName_ovb0089(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkEventName_ovb0089() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkTimelineEvent_ovb0089 {
    u32 kind;
    u64 value;
    UnkEventName_ovb0089 name;
};

struct UnkTimelineListener_ovb0089 {
    u8 pad_0x0[8];
    void* context;
    bool (*callback)(void* context, UnkTimelineEvent_ovb0089* event);
};

struct UnkTimelineComp_ovb0089 {
    u8 pad_0x0[0xf8];
    base::UnkCountedDeferredSet<void> listeners;  // 0xf8
};

struct UnkTrack;
struct UnkCutsceneTimeline;
struct UnkPlayer_b42;

struct UnkTimelineNamed_ovb0089 {
    u8 pad_0x0[0x38];
    CStrId name;
};

struct UnkTimelineSource_ovb0089 {
    u8 pad_0x0[0x10];
    UnkTimelineNamed_ovb0089* named;
};

struct UnkTimelineMessage_ovb0089 {
    u8 pad_0x0[8];
    UnkTimelineSource_ovb0089* source;
    u64 value;
};

static inline void Notify(UnkTimelineComp_ovb0089* self, UnkTimelineEvent_ovb0089* event) {
    base::UnkCountedDeferredWalk<void> walk(&self->listeners);
    u32 count = self->listeners.items.count;
    if (count) {
        void** p = self->listeners.items.data;
        void** end = p + count;
        for (; p != end; ++p) {
            UnkTimelineListener_ovb0089* l = static_cast<UnkTimelineListener_ovb0089*>(*p);
            if (!l || !l->callback || !l->callback(l->context, event))
                fn_7100a2e30c(&self->listeners, l);
        }
    }
    fn_7100a2e4a0(&walk);
}

void fn_7100a2c520(UnkTimelineComp_ovb0089* self, UnkTimelineMessage_ovb0089* msg) {
    UnkTimelineNamed_ovb0089* named =
        reinterpret_cast<UnkTimelineNamed_ovb0089*>(fn_710070d4e8(reinterpret_cast<UnkTrack*>(msg->source)));
    u64 value = msg->value;
    UnkTimelineEvent_ovb0089 event{3, value, named->name};
    Notify(self, &event);
}

void fn_7100a2c658(UnkTimelineComp_ovb0089* self, UnkTimelineMessage_ovb0089* msg) {
    if (fn_7100c95ed8(reinterpret_cast<UnkPlayer_b42*>(msg->source)))
        return;
    UnkTimelineSource_ovb0089* source = msg->source;
    UnkTimelineNamed_ovb0089* named = source->named;
    UnkTimelineEvent_ovb0089 event{2, (u64)source, named->name};
    Notify(self, &event);
}
