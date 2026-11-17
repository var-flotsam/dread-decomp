// CTimelineComponent unit (tools/tu_map.py): a message handler that tells the component's listeners
// (+0xf8) about event kind 1 with the message's value and its timeline's name, removes the entry under
// that name's key (fn_7100a2b7b8), then by the component's mode (+0x48) adds the timeline's id, plays a
// random event (fn_7100a2b9a8) or draws a random time between +0x5c and +0x60. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1134.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

void fn_7100a2e30c(base::UnkCountedDeferredSet<void>* self, void* value);
void fn_7100a2e4a0(base::UnkCountedDeferredWalk<void>* walk);

struct UnkTimelineRandom_ovb0089;
void fn_7100a2b9a8(UnkTimelineRandom_ovb0089* self, const u32* value);

// A string id whose release decrements and tests in two steps.
struct UnkEventName_la2c798 {
    base::global::UnkStringData* data;
    UnkEventName_la2c798(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkEventName_la2c798() {
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

struct UnkTimelineEvent_la2c798 {
    u32 kind;
    u64 value;
    UnkEventName_la2c798 name;
};

struct UnkTimelineListener_la2c798 {
    u8 pad_0x0[8];
    void* context;
    bool (*callback)(void* context, UnkTimelineEvent_la2c798* event);
};

struct UnkTimelineNamed_la2c798 {
    u8 pad_0x0[0x38];
    CStrId name;  // 0x38
    u8 pad_0x40[0x8];
    u64 id;  // 0x48
};

struct UnkTimelineMessage_la2c798 {
    UnkTrack* track;
    u64 value;
};

struct UnkTimelineComp_la2c798 {
    u8 pad_0x0[0x48];
    s32 mode;  // 0x48
    u8 pad_0x4c[0xc];
    f32 time;      // 0x58
    f32 time_min;  // 0x5c
    f32 time_max;  // 0x60
    u8 pad_0x64[0x94];
    base::UnkCountedDeferredSet<void> listeners;  // 0xf8
};
static_assert(__builtin_offsetof(UnkTimelineComp_la2c798, listeners) == 0xf8, "layout");

static inline void Notify(UnkTimelineComp_la2c798* self, UnkTimelineEvent_la2c798* event) {
    base::UnkCountedDeferredWalk<void> walk(&self->listeners);
    u32 count = self->listeners.items.count;
    if (count) {
        void** p = self->listeners.items.data;
        void** end = p + count;
        for (; p != end; ++p) {
            UnkTimelineListener_la2c798* l = static_cast<UnkTimelineListener_la2c798*>(*p);
            if (!l || !l->callback || !l->callback(l->context, event))
                fn_7100a2e30c(&self->listeners, l);
        }
    }
    fn_7100a2e4a0(&walk);
}

static inline f32 RandomBetween(f32 lo, f32 hi) {
    f32 t = (rand() & 0x7fff) * (1.0 / 32767.0);
    return lo + (hi - lo) * t;
}

void fn_7100a2c798(UnkTimelineComp_la2c798* self, UnkTimelineMessage_la2c798* msg) {
    UnkTimelineNamed_la2c798* named = reinterpret_cast<UnkTimelineNamed_la2c798*>(fn_710070d4e8(msg->track));
    {
        u64 value = msg->value;
        UnkTimelineEvent_la2c798 event{1, value, named->name};
        Notify(self, &event);
    }
    UnkTimelineComponent* timeline = reinterpret_cast<UnkTimelineComponent*>(self);
    u64 id = named->name.data->Key();
    fn_7100a2b7b8(timeline, &id);
    switch (self->mode) {
    case 0:
        id = named->id;
        if (id != (u64)-1) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
        }
        break;
    case 1:
        base::reflection::UnkReflTypeIdArg value;
        value.id = UnkAll_SpitSend().value;
        fn_7100a2b9a8(reinterpret_cast<UnkTimelineRandom_ovb0089*>(self), &value.id);
        break;
    case 2:
        self->time = RandomBetween(self->time_min, self->time_max);
        break;
    }
}
