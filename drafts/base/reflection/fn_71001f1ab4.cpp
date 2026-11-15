// Unit base::reflection: looks a key up in a sorted entry list with a cursor, else hands it to the fallback.
// Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkKeyEntry {
    u64 field_0x0;
    u64 field_0x8;
    s32 field_0x10;
    u8 pad_0x14[4];
};

struct UnkKeyEntries {
    UnkKeyEntry* data;
    u32 size;
};

struct UnkKeyCursor {
    u64 field_0x0;
    u8 pad_0x8[8];
    u8 field_0x10;
    u8 pad_0x11[7];
    UnkKeyEntries* entries;
    u32 count;
    u32 cursor;
};

void fn_71001f9e34(const UnkKeyEntry* key, u64 a, u8 b);

template <class I>
static UnkKeyEntry* At(UnkKeyEntries* v, I i) {
    if (v->size <= i)
        __assert_fail("i < m_iSize", "..\Base/global/RntVector.h", 0x15b, "operator[]");
    return &v->data[i];
}

void fn_71001f1ab4(UnkKeyCursor* self, const UnkKeyEntry* key) {
    s32 keyHi = key->field_0x10;
    u64 keyLo = key->field_0x8;
    self->cursor = 0;
    u64 i = 0;
    UnkKeyEntries* entries = self->entries;
    while (i < self->count) {
        const UnkKeyEntry* e = At(entries, i);
        if (e->field_0x10 < keyHi || (e->field_0x10 == keyHi && e->field_0x8 < keyLo)) {
            ++i;
            self->cursor = i;
            continue;
        }
        e = At(entries, self->cursor);
        if (e && e->field_0x10 == key->field_0x10 && e->field_0x0 == key->field_0x0)
            return;
        break;
    }
    fn_71001f9e34(key, self->field_0x0, self->field_0x10);
}
