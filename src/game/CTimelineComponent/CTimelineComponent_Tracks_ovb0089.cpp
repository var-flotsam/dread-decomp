// CTimelineComponent unit (tools/tu_map.py): the component's table of tracks keyed by 8-byte ids at
// +0x68 (each value a unique_ptr with a std::function deleter, 0x40 bytes; the keys in a parallel
// array), its timed list of delayed additions at +0x178 and its destructor. Names and layouts are
// placeholders.
#include <functional>
#include <memory>
#include <new>
#include <utility>

#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU1134.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineTrack_ovb0089;
using UnkTimelineTrackPtr_ovb0089 =
    std::unique_ptr<UnkTimelineTrack_ovb0089, std::function<void(UnkTimelineTrack_ovb0089*)>>;

struct UnkTimelineTrackTable_ovb0089 {
    UnkTimelineTrackPtr_ovb0089* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    void DestroyAll() {
        if (values && count) {
            u64 i = 0;
            do {
                values[i].~UnkTimelineTrackPtr_ovb0089();
            } while (++i < count);
        }
    }
};

extern char g_unk_7101a8dcc0[];
extern char g_unk_7101a8de98[];
extern char g_unk_7101a8de78[];

struct UnkTimelineComponentFull_ovb0089 {
    char* vtable;  // 0x0
    u8 pad_0x8[0x38];
    char* vtable_0x40;  // 0x40
    u8 pad_0x48[0x20];
    UnkTimelineTrackTable_ovb0089 tracks;  // 0x68
    u8 pad_0x90[0x8];
    base::UnkVector<UnkPendingAdd> pending_add;  // 0x98
    base::UnkVector<u64> pending_remove;         // 0xc0
    u8 pad_0xe8[0x8];
    char* listeners_vtable;               // 0xf0
    base::UnkVector<void*> listeners[3];  // 0xf8
    s32 listeners_lock;                   // 0x170
    char* timed_vtable;                   // 0x178
    u32 timed_count;                      // 0x180
    UnkTimedEntry* timed_head;
    UnkTimedEntry* timed_tail;
    base::UnkAllocator* timed_allocator;  // 0x198

    void FreeTimed() {
        for (UnkTimedEntry* e = timed_head; e;) {
            UnkTimedEntry* next = e->next;
            timed_allocator->vf_0x48(e);
            e = next;
        }
        timed_count = 0;
        timed_head = nullptr;
        timed_tail = nullptr;
    }
};
static_assert(__builtin_offsetof(UnkTimelineComponentFull_ovb0089, tracks) == 0x68, "layout");
static_assert(__builtin_offsetof(UnkTimelineComponentFull_ovb0089, timed_allocator) == 0x198, "layout");

struct UnkParent;

// Drops every track, then calls the base method.
void fn_7100a2bb4c(UnkTimelineComponentFull_ovb0089* self) {
    self->tracks.DestroyAll();
    self->tracks.count = 0;
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
}

// Notifies fn_710070d5dc of every track, drops them all, and frees the timed list.
void fn_7100a2bc3c(UnkTimelineComponentFull_ovb0089* self) {
    for (UnkTimelineTrackPtr_ovb0089 *p = self->tracks.values, *end = p + self->tracks.count; p != end; ++p)
        fn_710070d5dc(reinterpret_cast<u64>(p->get()));
    self->tracks.DestroyAll();
    self->tracks.count = 0;
    self->FreeTimed();
}

// Grows the table to room for `n` tracks, moving the tracks and keys over.
void fn_7100a2d678(UnkTimelineTrackTable_ovb0089* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* values = static_cast<UnkTimelineTrackPtr_ovb0089*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkTimelineTrackPtr_ovb0089), 8, self->flags));
    u64* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)n * sizeof(u64), 8, self->flags));
    if (UnkTimelineTrackPtr_ovb0089* old = self->values) {
        UnkTimelineTrackPtr_ovb0089* dst = values;
        UnkTimelineTrackPtr_ovb0089* src = old;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkTimelineTrackPtr_ovb0089(std::move(*src));
            src->~UnkTimelineTrackPtr_ovb0089();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (u64* old = self->keys) {
        // Copied bitwise: the original reloads the count after each key.
        u64* d = keys;
        u64* s = old;
        for (u32 i = 0; i < self->count; ++i, ++d, ++s)
            __builtin_memcpy(d, s, sizeof(u64));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = n;
}

// The destructor: the timed list, the listener lists, the queues and the table, then the base.
void fn_7100a2ade0(UnkTimelineComponentFull_ovb0089* self) {
    self->vtable = g_unk_7101a8dcc0 + 0x10;
    self->vtable_0x40 = g_unk_7101a8dcc0 + 0x1a0;
    self->timed_vtable = g_unk_7101a8de98 + 0x10;
    self->FreeTimed();
    self->listeners_vtable = g_unk_7101a8de78 + 0x10;
    self->listeners[2].Free();
    self->listeners[1].Free();
    self->listeners[0].Free();
    self->pending_remove.Free();
    self->pending_add.Free();
    self->tracks.DestroyAll();
    self->tracks.count = 0;
    if (self->tracks.keys)
        self->tracks.allocator->vf_0x48(self->tracks.keys);
    if (self->tracks.values)
        self->tracks.allocator->vf_0x48(self->tracks.values);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
