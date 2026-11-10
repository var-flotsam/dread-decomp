// CTimelineComponent unit (tools/tu_map.py): two message handlers that tell the component's listeners
// (the lists at +0xf8, walked under their counter; fn_7100a2e4a0 ends the walk) about an event: a kind
// (3 or 2), a value and a name id copied from the message's timeline. A listener without a callback, or
// whose callback returns false, is removed (fn_7100a2e30c). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTimelineListenerLists_ovb0089 {
    base::UnkVector<void*> field_0x0;
    base::UnkVector<void*> field_0x28;
    base::UnkVector<void*> field_0x50;
    union {
        s32 lock;  // 0x78
    };
};

struct UnkTimelineListenerLock_ovb0089 {
    UnkTimelineListenerLists_ovb0089* lists;
};

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
    UnkTimelineListenerLists_ovb0089 listeners;
};

struct UnkTimelineListeners_b53;
void fn_7100a2e30c(UnkTimelineListeners_b53* self, void* value);
void fn_7100a2e4a0(UnkTimelineListenerLock_ovb0089* self);

struct UnkTrack;
struct UnkCutsceneTimeline;
UnkCutsceneTimeline* fn_710070d4e8(UnkTrack* track);
struct UnkPlayer_b42;
bool fn_7100c95ed8(UnkPlayer_b42*);

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
    UnkTimelineListenerLock_ovb0089 lock;
    lock.lists = &self->listeners;
    ++self->listeners.lock;
    u32 count = self->listeners.field_0x50.count;
    if (count) {
        void** p = self->listeners.field_0x50.data;
        void** end = p + count;
        for (; p != end; ++p) {
            UnkTimelineListener_ovb0089* l = static_cast<UnkTimelineListener_ovb0089*>(*p);
            if (!l || !l->callback || !l->callback(l->context, event))
                fn_7100a2e30c(reinterpret_cast<UnkTimelineListeners_b53*>(&self->listeners), l);
        }
    }
    fn_7100a2e4a0(&lock);
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
