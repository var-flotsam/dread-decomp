// Unit 141 of tools/tu_map.py: the entry at an index of a pointer vector at +0x10 (base::global::
// CRntVector's checked operator[]), or null when the index is not below the virtual count (vf_0x18).
// Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

template <class T>
struct UnkRntVector141 {
    T* data;
    u32 size;

    T& operator[](u32 i) {
        if (size <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

struct UnkEntry141;

struct UnkEntryList141 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 vf_0x18();

    u8 pad_0x8[0x8];
    UnkRntVector141<UnkEntry141*> entries;  // 0x10
};

UnkEntry141* fn_7100149ed4(UnkEntryList141* self, u32 index) {
    if (self->vf_0x18() <= index)
        return nullptr;
    return self->entries[index];
}
