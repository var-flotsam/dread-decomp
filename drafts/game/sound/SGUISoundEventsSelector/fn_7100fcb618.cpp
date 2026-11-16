// sound::SGUISoundEventsSelector's unit (tools/tu_map.py unit 2625): the copy constructor of an array of
// (key, string id) events. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A string id held by an event: copies take a reference, moves take the pointer and leave the
// pool's empty string (CStrId has no move constructor).
struct UnkSoundEventId {
    UnkStringData* data;

    UnkSoundEventId(const UnkSoundEventId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkSoundEventId(UnkSoundEventId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkSoundEventId() {
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

struct UnkSoundEvent {
    u32 key;
    UnkSoundEventId id;
};
static_assert(sizeof(UnkSoundEvent) == 0x10, "UnkSoundEvent layout");

using UnkSoundEventVector = base::global::CRntVector<UnkSoundEvent>;

// The array's copy constructor: the source's settings and allocator, its capacity, then a copy of each
// event appended.
void fn_7100fcb618(UnkSoundEventVector* self, const UnkSoundEventVector* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkSoundEvent* end = other->data + other->count;
        for (const UnkSoundEvent* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}
