// From unit 42 of tools/tu_map.py (0x71000684e4-0x710006a428, no class hint): a timeline object's
// events read from a file: one list of timed events (a time in tenths and the event, in two parallel
// arrays) and four lists of untimed ones, each event read by fn_7100067848. Names and layouts are
// placeholders.
// Holds fn_7100068f5c.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "types.h"

// An event: polymorphic, destroyed (and freed) through its vtable's slot 2.
struct UnkTimelineEvent {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deleting destructor
};

// Reads one event (created by its class id) from the file.
UnkTimelineEvent* fn_7100067848(base::global::CRntFile* file);

// An array of owning event pointers (base::UnkVector's layout), moved when it grows.
struct UnkEventArray : base::UnkVector<UnkTimelineEvent*> {
    void Grow(u32 n) {
        if (capacity < n) {
            auto** p = static_cast<UnkTimelineEvent**>(allocator->vf_0x40((u64)n * 8, 8, flags));
            if (data) {
                for (u32 i = 0; i < count; ++i) {
                    p[i] = data[i];
                    data[i] = nullptr;
                    if (UnkTimelineEvent* old = data[i]) {
                        data[i] = nullptr;
                        old->vf_0x10();
                    }
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    void Append(UnkTimelineEvent* e) {
        u32 c = count;
        u32 cap = capacity;
        if (cap <= c && step != 0)
            cap = (c / step + 1) * step;
        if ((s32)cap <= (s32)c)
            for (;;) {
            }
        Grow(cap);
        data[count] = e;
        ++count;
    }
};

// The timed events: the events and their times in two arrays sharing a count.
struct UnkTimedEvents {
    UnkTimelineEvent** events;
    f32* times;
    u32 count;
    u32 capacity;
    u32 step;
};
void fn_710006981c(UnkTimedEvents* self, u32 capacity);  // reserve

struct UnkTimelineEvents {
    UnkTimedEvents timed;
    u8 pad_0x20[0x8];
    UnkEventArray lists[4];  // 0x28, 0x50, 0x78, 0xa0
};
static_assert(__builtin_offsetof(UnkTimelineEvents, lists) == 0x28, "UnkTimelineEvents placeholder layout");

// Reads the five counts, reserves each list, then the timed events and the four untimed lists.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100068f5c(UnkTimelineEvents* self, base::global::CRntFile* file) {
    u16 counts[5];
    for (u32 i = 0; i < 5; ++i)
        fn_710007c714(file, &counts[i], 2, 1);
    fn_710006981c(&self->timed, counts[0]);
    for (u32 i = 0; i < 4; ++i)
        self->lists[i].Grow(counts[i + 1]);
    for (u32 i = 0; i < counts[0]; ++i) {
        u32 tenths;
        fn_710007c714(file, &tenths, 4, 1);
        f32 time = (f32)tenths;
        UnkTimelineEvent* e = fn_7100067848(file);
        UnkTimedEvents& t = self->timed;
        if (t.count == t.capacity)
            fn_710006981c(&t, t.step + t.count);
        t.times[t.count] = time * 0.1f;
        t.events[t.count] = e;
        ++t.count;
    }
    for (u32 l = 0; l < 4; ++l)
        for (u32 i = 0; i < counts[l + 1]; ++i)
            self->lists[l].Append(fn_7100067848(file));
}
