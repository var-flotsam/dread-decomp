// CTimelineComponent's unit (1542): the component's constructor. After the component base it sets the
// defaults, creates the track table with room for 16 tracks, the two queues with room for 100 entries
// each, the listener lists and the timed list. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntVector;

// The table's type as fn_7100a2d678's definition (src/game/CTimelineComponent/CTimelineComponent_Tracks_ovb0089.cpp)
// declares it.
struct UnkTimelineTrackTable_ovb0089;
void fn_7100a2d678(UnkTimelineTrackTable_ovb0089* self, u32 n);

// A queued 64-bit id, as its hash's two words. The copy constructor is defaulted out of line, so it
// copies both words as one block (the original reloads the queue's count after each copy).
struct UnkQueuedId_la2ab7c {
    u32 low;
    u32 high;

    UnkQueuedId_la2ab7c(const UnkQueuedId_la2ab7c& other);
};
inline UnkQueuedId_la2ab7c::UnkQueuedId_la2ab7c(const UnkQueuedId_la2ab7c& other) = default;

using UnkTrackTable_la2ab7c = base::global::CRntSmallDictionary<u64, UnkTableEntry>;

// Three lists behind their own vtable.
struct UnkTimelineListeners_la2ab7c {
    char* vtable;
    CRntVector<void*> lists[3];
    s32 lock;

    UnkTimelineListeners_la2ab7c()
        : vtable(g_unk_7101a8de78 + 0x10),
          lists{CRntVector<void*>(0xf), CRntVector<void*>(0xf), CRntVector<void*>(0xf)}, lock(0) {}
};

// The timed list of delayed additions.
struct UnkTimedList_la2ab7c {
    char* vtable;
    u32 count = 0;
    UnkTimedEntry* head = nullptr;
    UnkTimedEntry* tail = nullptr;
    base::UnkAllocator* allocator = fn_7100083990();
    u32 flags = 0x13;

    UnkTimedList_la2ab7c() : vtable(g_unk_7101a8de98 + 0x10) {}
};

struct UnkTimelineComponentFull_la2ab7c {
    char* vtable;  // 0x0
    u8 pad_0x8[0x38];
    char* vtable_0x40;  // 0x40
    u32 field_0x48;
    s64 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    s32 field_0x64;
    UnkTrackTable_la2ab7c tracks;  // 0x68
    bool locked;                   // 0x90
    CRntVector<UnkPendingAdd> pending_add;  // 0x98
    CRntVector<UnkQueuedId_la2ab7c> pending_remove;         // 0xc0
    u64 field_0xe8;
    UnkTimelineListeners_la2ab7c listeners;  // 0xf0
    UnkTimedList_la2ab7c timed;              // 0x178
};
static_assert(__builtin_offsetof(UnkTimelineComponentFull_la2ab7c, tracks) == 0x68, "layout");
static_assert(__builtin_offsetof(UnkTimelineComponentFull_la2ab7c, listeners) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkTimelineComponentFull_la2ab7c, timed) == 0x178, "layout");

void fn_7100a2ab7c(UnkTimelineComponentFull_la2ab7c* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a8dcc0 + 0x10;
    self->vtable_0x40 = g_unk_7101a8dcc0 + 0x1a0;
    self->field_0x48 = 0;
    self->field_0x50 = -1;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->field_0x64 = -1;
    new (&self->tracks) UnkTrackTable_la2ab7c(10);
    fn_7100a2d678(reinterpret_cast<UnkTimelineTrackTable_ovb0089*>(&self->tracks), 16);
    self->locked = false;
    new (&self->pending_add) CRntVector<UnkPendingAdd>(10);
    self->pending_add.Reserve(100);
    new (&self->pending_remove) CRntVector<UnkQueuedId_la2ab7c>(10);
    self->pending_remove.Reserve(100);
    self->field_0xe8 = 0;
    new (&self->listeners) UnkTimelineListeners_la2ab7c();
    new (&self->timed) UnkTimedList_la2ab7c();
}
