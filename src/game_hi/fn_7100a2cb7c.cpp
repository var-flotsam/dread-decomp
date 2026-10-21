// Sets a value for an id (-1 = none). With a delay that is positive or about zero, it is
// queued on a timed list (once per id when `unique`); otherwise it is applied now, or queued
// for addition while the table is locked. With `unique`, an id not yet known is ignored on
// the immediate path.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPendingAdd {
    u64 key;
    u32 value;

    UnkPendingAdd(u64 key, u32 value) : key(key), value(value) {}
    // The key is copied bitwise (the original reloads the array's count and data pointer
    // after the copies).
    UnkPendingAdd(const UnkPendingAdd& other) {
        __builtin_memcpy(&key, &other.key, sizeof(key));
        value = other.value;
    }
};

// The engine's growable array (same layout as base::UnkVector).
struct UnkPendingAddArray {
    UnkPendingAdd* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void PushBack(const UnkPendingAdd& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkPendingAdd* p;
        if (old_capacity < new_capacity) {
            p = static_cast<UnkPendingAdd*>(
                allocator->vf_0x40((u64)new_capacity * sizeof(UnkPendingAdd), 8, flags));
            if (UnkPendingAdd* old = data) {
                UnkPendingAdd* src = old;
                UnkPendingAdd* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst)
                    new (dst) UnkPendingAdd(*src);
                allocator->vf_0x48(data);
            }
            data = p;
            n = count;
            capacity = new_capacity;
        } else {
            p = data;
        }
        new (&p[n]) UnkPendingAdd(value);
        ++count;
    }
};

struct UnkTimedEntry {
    UnkPendingAdd item;
    f32 delay;
    UnkTimedEntry* next;
};

struct UnkTableOwner {
    u8 unk_0x0[0x90];
    bool locked;
    UnkPendingAddArray pending_add;
    u8 unk_0xc0[0xc0];
    // A union member: no type-based alias info, so the count is reloaded after the link
    // stores.
    union {
        u32 timed_count;
    };
    UnkTimedEntry* timed_head;
    UnkTimedEntry* timed_tail;
    base::UnkAllocator* timed_allocator;
    u32 timed_flags;
};

bool fn_7100a2be94(UnkTableOwner* self, const u64* id);
void* fn_7100a2c13c(UnkTableOwner* self, const u64* id);
void fn_7100a2c200(UnkTableOwner* self, void* entry, const u32* value);

// NON_MATCHING(minor): the duplicate-id scan's two exit tests are merged into one conditional
// compare; the original branches on each (3 instructions).
void fn_7100a2cb7c(UnkTableOwner* self, const u64* id, const u32* value, bool unique, f32 delay) {
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
        self->pending_add.PushBack(UnkPendingAdd(*id, *value));
        return;
    }
    void* entry = fn_7100a2c13c(self, id);
    alignas(8) u32 v = *value;
    fn_7100a2c200(self, entry, &v);
}
