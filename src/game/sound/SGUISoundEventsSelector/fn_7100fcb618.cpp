// build-flags: no-strict-aliasing
// sound::SGUISoundEventsSelector's unit (tools/tu_map.py unit 2625): the copy constructor of an array of
// (key, string id) events. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A string id held by an event: copies take a reference, moves take the pointer and leave the
// pool's empty string (CStrId has no move constructor).
struct UnkSoundEventId_fcb618 {
    UnkStringData* data;

    UnkSoundEventId_fcb618(const UnkSoundEventId_fcb618& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkSoundEventId_fcb618(UnkSoundEventId_fcb618&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkSoundEventId_fcb618() {
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

struct UnkSoundEvent_fcb618 {
    u32 key;
    UnkSoundEventId_fcb618 id;
};
static_assert(sizeof(UnkSoundEvent_fcb618) == 0x10, "UnkSoundEvent_fcb618 layout");

using UnkSoundEventVector = base::global::CRntVector<UnkSoundEvent_fcb618>;

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
        const UnkSoundEvent_fcb618* end = other->data + other->count;
        for (const UnkSoundEvent_fcb618* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}
