#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// CStrId's copy constructor, with the pointer copied untyped (the count is reloaded after it).
__attribute__((always_inline)) static inline void CopyId(CStrId* dst, const CStrId* src) {
    base::global::UnkStringData* data = src->data;
    __builtin_memcpy(&dst->data, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

struct UnkRawId_b0234 {
    base::global::UnkStringData* data;
    __attribute__((always_inline)) UnkRawId_b0234(const UnkRawId_b0234& other) {
        base::global::UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
    }
};
struct UnkWord_b0234 {
    u32 value;
    __attribute__((always_inline)) UnkWord_b0234(const UnkWord_b0234& other) { __builtin_memcpy(this, &other, 4); }
};
struct UnkIdWords4_b0234 {
    UnkRawId_b0234 id;
    UnkWord_b0234 field_0x8;
    UnkWord_b0234 field_0xc;
    UnkWord_b0234 field_0x10;
    UnkWord_b0234 field_0x14;
};

struct UnkIdWordsArray_b0234 {
    UnkIdWords4_b0234* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100e71984(UnkIdWordsArray_b0234* self, u32 capacity);

void fn_7100e711dc(UnkIdWordsArray_b0234* self, const CStrId* key, const UnkIdWords4_b0234* value) {
    if (self->count == self->capacity)
        fn_7100e71984(self, self->step + self->count);
    CopyId(&self->keys[self->count], key);
    new (&self->values[self->count]) UnkIdWords4_b0234(*value);
    ++self->count;
}
