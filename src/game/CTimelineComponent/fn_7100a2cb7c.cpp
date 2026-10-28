// Sets a value for an id (-1 = none). With a delay that is positive or about zero, it is
// queued on a timed list (once per id when `unique`); otherwise it is applied now, or queued
// for addition while the table is locked. With `unique`, an id not yet known is ignored on
// the immediate path.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

// Appends to the engine's array, copy-constructing the elements one by one.
static inline void AppendCopy(base::UnkVector<UnkPendingAdd>& a, const UnkPendingAdd& value) {
    u32 n = a.count;
    u32 old_capacity = a.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && a.step != 0)
        new_capacity = (n / a.step + 1) * a.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkPendingAdd* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkPendingAdd*>(
            a.allocator->vf_0x40((u64)new_capacity * sizeof(UnkPendingAdd), 8, a.flags));
        if (UnkPendingAdd* old = a.data) {
            UnkPendingAdd* src = old;
            UnkPendingAdd* dst = p;
            for (u32 i = 0; i < a.count; ++i, ++src, ++dst)
                new (dst) UnkPendingAdd(*src);
            a.allocator->vf_0x48(a.data);
        }
        a.data = p;
        n = a.count;
        a.capacity = new_capacity;
    } else {
        p = a.data;
    }
    new (&p[n]) UnkPendingAdd(value);
    ++a.count;
}

void fn_7100a2c200(UnkTimelineComponent* self, void* entry, const u32* value);

// NON_MATCHING(minor): the duplicate-id scan's two exit tests are merged into one conditional
// compare; the original branches on each (3 instructions).
void fn_7100a2cb7c(UnkTimelineComponent* self, const u64* id, const u32* value, bool unique, f32 delay) {
    u64 key = *id;
    if (key == (u64)-1)
        return;
    bool zero = delay + -0.001f <= 0.0f && delay + 0.001f >= 0.0f;
    if (delay > 0.0f || zero) {
        if (unique) {
            bool found = false;
            for (UnkTimedEntry* e = self->timed_head; e && !found; e = e->next)
                found = e->item.key == key;
            if (found)
                return;
        }
        u32 v = *value;
        UnkTimedEntry* e = static_cast<UnkTimedEntry*>(
            self->timed_allocator->vf_0x40(sizeof(UnkTimedEntry), 8, self->timed_flags));
        if (!e)
            return;
        e->item.key = key;
        e->item.value = v;
        e->delay = delay;
        e->next = nullptr;
        if (self->timed_count == 0) {
            self->timed_head = e;
            self->timed_tail = e;
            self->timed_count = 1;
        } else {
            self->timed_tail->next = e;
            e->next = nullptr;
            self->timed_tail = e;
            ++self->timed_count;
        }
        return;
    }
    if (unique && !fn_7100a2be94(self, id))
        return;
    if (self->locked) {
        AppendCopy(self->pending_add, UnkPendingAdd(*id, *value));
        return;
    }
    void* entry = fn_7100a2c13c(self, id);
    alignas(8) u32 v = *value;
    fn_7100a2c200(self, entry, &v);
}
