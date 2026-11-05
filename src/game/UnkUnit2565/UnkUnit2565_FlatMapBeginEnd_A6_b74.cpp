// Unit 2565 (tools/tu_map.py): the reflected begin (+0x160) and end (+0x168) slots of the flat map whose
// "set by key" slots are in UnkUnit2565_FlatMapSet_A6_b74.cpp (u32 keys, 0x248-byte run-time string values).
// The iterator is the cached (key, value) holder itself (base/UnkCollectionIter.h's shape): empty, owning a
// copy (destroyed inline through fn_710007ed40 and fn_710007ecd4), or referring to a key and a value. The
// slot builds a referring temporary and moves it into the output. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct alignas(8) UnkLongStringValue_A6_b74 {
    u8 data[0x248];

    __attribute__((always_inline)) void Destroy() {
        fn_710007ed40(reinterpret_cast<base::global::CRntString*>(this));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};

struct UnkLongStringIterValue_A6_b74 {
    union {
        struct {
            u32 key;
            UnkLongStringValue_A6_b74 element;
        } value;
        struct {
            u32* key;
            UnkLongStringValue_A6_b74* element;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) UnkLongStringIterValue_A6_b74(u32* k, UnkLongStringValue_A6_b74* e) {
        state = 2;
        ref.key = k;
        ref.element = e;
    }
    __attribute__((always_inline)) ~UnkLongStringIterValue_A6_b74() { Reset(); }

    __attribute__((always_inline)) void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            value.element.Destroy();
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) UnkLongStringIterValue_A6_b74& operator=(UnkLongStringIterValue_A6_b74&& o) {
        Reset();
        state = 2;
        ref = o.ref;
        return *this;
    }
};

struct UnkLongStringMap_A6_b74 {
    UnkLongStringValue_A6_b74* values;
    u32* keys;
    u32 count;
};

void fn_7100e7c214(UnkLongStringMap_A6_b74* self, UnkLongStringIterValue_A6_b74* out) {
    *out = UnkLongStringIterValue_A6_b74(self->keys, self->values);
}

void fn_7100e7c28c(UnkLongStringMap_A6_b74* self, UnkLongStringIterValue_A6_b74* out) {
    *out = UnkLongStringIterValue_A6_b74(self->keys + self->count, self->values + self->count);
}
