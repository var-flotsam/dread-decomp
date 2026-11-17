// UnkTU811 (CEmmyAIComponent's unit): fn_71005551c0 copies one fixed-capacity list of string ids over
// another: the ids both hold are assigned, the other's extra ids are appended, and the count taken.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A count followed by inline storage for the ids (packed: the ids start at +4).
struct __attribute__((packed)) UnkStrIdFixedList_l5551c0 {
    u32 count;
    u8 storage[16 * sizeof(CStrId)];

    CStrId* items() { return reinterpret_cast<CStrId*>(storage); }
    const CStrId* items() const { return reinterpret_cast<const CStrId*>(storage); }
};

UnkStrIdFixedList_l5551c0* fn_71005551c0(UnkStrIdFixedList_l5551c0* self, const UnkStrIdFixedList_l5551c0* other) {
    if (self == other)
        return self;
    if (self->count > other->count) {
        for (u32 i = 0; i < other->count; ++i)
            self->items()[i] = other->items()[i];
    } else {
        for (u32 i = 0; i < self->count; ++i)
            self->items()[i] = other->items()[i];
    }
    for (u32 i = self->count; i < other->count; ++i) {
        new (&self->items()[self->count]) CStrId(other->items()[i]);
        ++self->count;
    }
    self->count = other->count;
    return self;
}
