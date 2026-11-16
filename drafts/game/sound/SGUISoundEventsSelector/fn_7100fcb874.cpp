// sound::SGUISoundEventsSelector's unit (tools/tu_map.py unit 2625): the copy-assignment of an array of
// (key, string id) events. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A string id held by an event: CStrId with a move constructor. Copies take a reference, moves take
// the pointer and leave the pool's empty string.
struct UnkSoundEventId : base::global::UnkStrIdBase {
    UnkSoundEventId(const UnkSoundEventId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkSoundEventId(UnkSoundEventId&& other) : UnkStrIdBase(other) { other.data = &g_unk_7101d47108->empty; }
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

// What an event is selected by (a 32-bit value, copied whole).
struct UnkSoundEventKey {
    u32 value;
};

struct UnkSoundEvent {
    UnkSoundEventKey key;
    UnkSoundEventId id;
};
static_assert(sizeof(UnkSoundEvent) == 0x10, "UnkSoundEvent layout");

using UnkSoundEventVector = base::global::CRntVector<UnkSoundEvent>;

// The array's copy-assignment: drops this array's events, reserves the source's capacity and appends
// a copy of each of its events.
UnkSoundEventVector* fn_7100fcb874(UnkSoundEventVector* self, const UnkSoundEventVector* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        if (other->count != 0) {
            const UnkSoundEvent* end = other->data + other->count;
            for (const UnkSoundEvent* p = other->data; p != end; ++p)
                self->PushBackCopy(*p);
        }
    }
    return self;
}
