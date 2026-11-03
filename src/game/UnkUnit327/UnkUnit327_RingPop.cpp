// tools/tu_map.py unit 327 (no class hint): a ring of 64 two-word entries stored inline at +0x20
// (head +0x8, tail +0x10, count +0x18); pops an entry from the front or from the back and answers
// it. Names are placeholders.
#include "types.h"

struct UnkRingEntry327 {
    u64 first;
    u64 second;
};

struct UnkRing327 {
    void* field_0x0;
    UnkRingEntry327* head;  // 0x8
    UnkRingEntry327* tail;  // 0x10
    u64 count;              // 0x18
    UnkRingEntry327 entries[64];  // 0x20
};

UnkRingEntry327 fn_710022314c(UnkRing327* self) {
    UnkRingEntry327* e = self->head;
    UnkRingEntry327 value = *e;
    ++e;
    self->head = e == self->entries + 64 ? self->entries : e;
    --self->count;
    return value;
}

UnkRingEntry327 fn_710022317c(UnkRing327* self) {
    UnkRingEntry327* e = self->tail;
    e = e == self->entries ? self->entries + 63 : e - 1;
    UnkRingEntry327 value = *e;
    self->tail = e;
    --self->count;
    return value;
}
