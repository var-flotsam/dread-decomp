// Copy assignment of a byte array (base::UnkVector<u8>; base::reflection's unit): the array grows
// to the other's capacity when it is smaller (the old bytes, none after the count is cleared, are
// copied over and the old array freed), then the other's bytes are copied in (fn_71000e1980).
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

base::UnkVector<u8>* fn_71000e1980(base::UnkVector<u8>* self, const base::UnkVector<u8>* other) {
    if (self != other) {
        self->count = 0;
        u32 capacity = other->capacity;
        u8* data;
        if (self->capacity < capacity) {
            data = static_cast<u8*>(self->allocator->vf_0x40(capacity, 8, self->flags));
            if (self->data) {
                memcpy(data, self->data, self->count);
                self->allocator->vf_0x48(self->data);
            }
            self->data = data;
            self->capacity = capacity;
        } else {
            data = self->data;
        }
        memcpy(data, other->data, other->count);
        self->count = other->count;
    }
    return self;
}
