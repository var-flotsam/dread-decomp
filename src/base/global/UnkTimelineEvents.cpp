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

// The arrays' slots are written as raw words (the original's stores may alias the arrays' own
// fields: their counts are read again after each one).
template <class T>
static inline void StoreSlot(T* slot, T value) {
    __builtin_memcpy(slot, &value, sizeof(value));
}

// An array of owning event pointers (base::UnkVector's layout), moved when it grows.
struct UnkEventArray : base::UnkVector<UnkTimelineEvent*> {
    __attribute__((always_inline)) void Grow(u32 n) {
        if (capacity < n) {
            auto** p = static_cast<UnkTimelineEvent**>(allocator->vf_0x40((u64)n * 8, 8, flags));
            if (data) {
                UnkTimelineEvent** src = data;
                u64 k = 0;
                for (u32 i = 0; i < count; ++i, ++k, ++src) {
                    UnkTimelineEvent* t = *src;
                    StoreSlot<UnkTimelineEvent*>(src, nullptr);
                    StoreSlot(&p[k], t);
                    UnkTimelineEvent* old = *src;
                    StoreSlot<UnkTimelineEvent*>(src, nullptr);
                    if (old)
                        old->vf_0x10();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    __attribute__((always_inline)) void Append(UnkTimelineEvent* e) {
        u32 c = count;
        u32 cap = capacity;
        if (cap <= c && step != 0)
            cap = (c / step + 1) * step;
        if (!((s32)c < (s32)cap))
            for (;;) {
            }
        Grow(cap);
        StoreSlot(&data[count], e);
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
void fn_7100068f5c(UnkTimelineEvents* self, base::global::CRntFile* file) {
    u16 timed_count, count0, count1, count2, count3;
    fn_710007c714(file, &timed_count, 2, 1);
    fn_710007c714(file, &count0, 2, 1);
    fn_710007c714(file, &count1, 2, 1);
    fn_710007c714(file, &count2, 2, 1);
    fn_710007c714(file, &count3, 2, 1);
    fn_710006981c(&self->timed, timed_count);
    self->lists[0].Grow(count0);
    self->lists[1].Grow(count1);
    self->lists[2].Grow(count2);
    self->lists[3].Grow(count3);
    for (u32 i = 0; i < timed_count; ++i) {
        u32 tenths;
        fn_710007c714(file, &tenths, 4, 1);
        f32 time = (f32)tenths * 0.1f;
        UnkTimelineEvent* e = fn_7100067848(file);
        UnkTimedEvents& t = self->timed;
        if (t.count == t.capacity)
            fn_710006981c(&t, t.step + t.count);
        StoreSlot(&t.times[t.count], time);
        StoreSlot(&t.events[t.count], e);
        ++t.count;
    }
    for (u32 i = 0; i < count0; ++i)
        self->lists[0].Append(fn_7100067848(file));
    for (u32 i = 0; i < count1; ++i)
        self->lists[1].Append(fn_7100067848(file));
    for (u32 i = 0; i < count2; ++i)
        self->lists[2].Append(fn_7100067848(file));
    for (u32 i = 0; i < count3; ++i)
        self->lists[3].Append(fn_7100067848(file));
}
