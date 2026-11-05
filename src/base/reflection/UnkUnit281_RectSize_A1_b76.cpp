// Unit 281 (base::reflection): two getters of an entry's rectangle size as floats. The entries
// (0x28 bytes) are in a bounds-checked vector at +8 (data, u32 size at +0x10, the RntVector
// operator[] assertion); an entry whose rectangle at +8 is all zero falls back to the rectangle at
// +0x30. fn_7100202a14 answers the width, fn_7100202ac0 the height. Names are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkRectA1 {
    u32 x;
    u32 y;
    u32 w;
    u32 h;

    bool IsSet() const { return w || h || x || y; }
};

struct UnkRectEntryA1 {
    u8 pad_0x0[8];
    UnkRectA1 rect;  // +0x8
    u8 pad_0x18[0x10];
};
static_assert(sizeof(UnkRectEntryA1) == 0x28, "UnkRectEntryA1 layout");

struct UnkRectEntriesA1 {
    UnkRectEntryA1* data;
    u32 size;

    UnkRectEntryA1& operator[](u32 i) {
        if (!(i < size))
            __assert_fail("i < m_iSize", "..\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

struct UnkRectOwnerA1 {
    void* vtable;
    UnkRectEntriesA1 entries;  // +0x8
    u8 pad_0x18[0x18];
    UnkRectA1 fallback;  // +0x30
};

static inline const UnkRectA1& RectA1(UnkRectOwnerA1* self, u32 i) {
    if (self->entries[i].rect.IsSet())
        return self->entries.data[i].rect;
    return self->fallback;
}

f32 fn_7100202a14(UnkRectOwnerA1* self, u32 i) {
    return RectA1(self, i).w;
}

f32 fn_7100202ac0(UnkRectOwnerA1* self, u32 i) {
    return RectA1(self, i).h;
}
