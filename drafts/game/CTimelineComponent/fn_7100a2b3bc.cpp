// CTimelineComponent's unit: fn_7100a2b3bc ticks the timed list. Each entry's delay drops by dt;
// once it is spent and the id is known, its value is applied now, and the entry is unlinked and
// freed. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimedStaticZero_la2b3bc {
    u32 value;
    UnkTimedStaticZero_la2b3bc() : value(0) {}
};

inline const UnkTimedStaticZero_la2b3bc& UnkTimedStaticZero() {
    static UnkTimedStaticZero_la2b3bc s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkTimedDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkTimedStaticZero().value & 0xff;
    return s_mask;
}

void fn_7100a2b3bc(UnkTimelineComponent* self, f32 dt) {
    UnkTimedEntry* entry = self->timed_head;
    if (!entry)
        return;
    do {
        entry->delay -= dt;
        UnkTimelineResourceEntry* found = fn_7100a2c13c(self, &entry->item.key);
        if ((entry->delay > 0.0f) & (found != nullptr)) {
            entry = entry->next;
            continue;
        }
        asm("" : "+r"(found));
        {
            {
                if (found) {
                u32 value = UnkTimedDefaultMask();
                fn_7100a2cb7c(self, &entry->item.key, &value, false, -1.0f);
                }
            }
            UnkTimedEntry* prev = nullptr;
            UnkTimedEntry* cur = self->timed_head;
            UnkTimedEntry* next;
            for (;;) {
                if (!cur)
                    return;
                next = cur->next;
                if (cur == entry)
                    break;
                prev = cur;
                cur = next;
            }
            if (prev) {
                prev->next = next;
                if (self->timed_tail == entry)
                    self->timed_tail = prev;
            } else {
                self->timed_head = next;
            }
            if (--self->timed_count == 0) {
                self->timed_head = nullptr;
                self->timed_tail = nullptr;
            }
            self->timed_allocator->vf_0x48(entry);
            entry = next;
        }
    } while (entry);
}
