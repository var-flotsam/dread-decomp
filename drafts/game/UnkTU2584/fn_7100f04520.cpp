// Unit 2584: updates every entry of a reference-counted list with the time step and removes the
// entries that have finished (inactive and out of time), moving the last entry into the gap.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(f32 dt);
    u8 pad_0x8[0x18];
    f32 timeLeft;
    u8 active;
    u8 pad_0x25[0xb];
    u32 refCount;
};

inline void ReleaseEntry(UnkEntry* e) {
    if (e && --e->refCount == 0)
        e->vf_0x8();
}

struct UnkEntryOwner {
    u8 pad_0x0[0x30];
    UnkEntry** data;
    u32 count;
};

void fn_7100f04520(UnkEntryOwner* self, f32 dt) {
    if (self->count == 0)
        return;
    UnkEntry** it = self->data;
    do {
        (*it)->vf_0x18(dt);
        UnkEntry* e = *it;
        if (e->active || e->timeLeft > 0.0f) {
            ++it;
            continue;
        }
        u32 index = (u32)(it - self->data);
        u32 count = self->count;
        if (index < count) {
            count -= 1;
            u64 slot = index;
            if (index < count) {
                UnkEntry* old = self->data[index];
                UnkEntry** d;
                UnkEntry* moved;
                if (__builtin_expect(old == nullptr || --old->refCount != 0, 1)) {
                    d = self->data;
                    d[slot] = nullptr;
                    moved = d[count];
                } else {
                    old->vf_0x8();
                    d = self->data;
                    d[slot] = nullptr;
                    moved = d[count];
                }
                if (moved) {
                    moved->refCount += 1;
                    ReleaseEntry(d[slot]);
                    d[slot] = moved;
                }
            }
            if (count < self->count) {
                u64 i = count;
                do {
                    ReleaseEntry(self->data[i]);
                    ++i;
                } while (i < self->count);
            }
            self->count = count;
        }
        it = self->data + index;
    } while (it != self->data + self->count);
}
