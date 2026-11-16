// Unit 2702 (hint CAIComponent): appends an entry (an object, the game manager's current time and a
// flag) to the AI's queue of recent entries, first dropping the oldest one while the queue holds ten.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "types.h"

extern char g_unk_7101cb2598[];  // the entry's vtable

struct UnkQueueEntry_l105990 {
    void* vtable;
    void* object;  // 0x8
    f32 time;      // 0x10
    bool flag;     // 0x14
    UnkQueueEntry_l105990* prev;  // 0x18
    UnkQueueEntry_l105990* next;  // 0x20

    UnkQueueEntry_l105990(void* object, f32 time, bool flag)
        : vtable(g_unk_7101cb2598 + 0x10), object(object), time(time), flag(flag), prev(nullptr), next(nullptr) {}
};

struct UnkQueueAI_l105990 {
    u8 pad_0x0[0x1170];
    u32 count;                      // 0x1170
    UnkQueueEntry_l105990* last;    // 0x1178
    UnkQueueEntry_l105990* first;   // 0x1180
    u32 flags;                      // 0x1188
    base::UnkAllocator* allocator;  // 0x1190

    void PopFirst() {
        UnkQueueEntry_l105990* entry = first;
        first = entry->next;
        if (first)
            first->prev = nullptr;
        allocator->vf_0x48(entry);
        if (--count == 0)
            first = last;
    }

    void Append(UnkQueueEntry_l105990* entry) {
        u32 n;
        if (count == 0) {
            last = entry;
            n = 1;
            first = entry;
        } else {
            last->next = entry;
            entry->prev = last;
            n = count + 1;
            last = entry;
        }
        count = n;
    }
};

void fn_7101105990(UnkQueueAI_l105990* self, void* object, bool flag) {
    if (self->count >= 10 && self->first)
        self->PopFirst();
    f32 time = g_unk_7101d47310->field_0x2444;
    void* mem = self->allocator->vf_0x40(sizeof(UnkQueueEntry_l105990), 8, self->flags);
    if (!mem)
        return;
    self->Append(new (mem) UnkQueueEntry_l105990(object, time, flag));
}
