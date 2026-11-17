// CEmmyGrabSamusAttack unit (tools/tu_map.py): builds a fixed array of four strings with 0x20 bytes of
// inline storage and copies up to four given strings into it, in order. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// The vtable of the allocator object of a string with 0x20 bytes of inline storage.
extern void* g_unk_7101a479c8[];

// A string with 0x20 bytes of inline storage (0x50 bytes in all).
struct UnkInlineStr50_l505000 {
    u8 str[0x30];
    void** vtable;  // +0x30: the allocator object that owns the inline storage
    u8 pad_0x38[8];
    u64 field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[4];

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    const CRntString* Str() const { return reinterpret_cast<const CRntString*>(str); }

    UnkInlineStr50_l505000() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&vtable));
        vtable = g_unk_7101a479c8 + 2;
        field_0x40 = 0;
        field_0x48 = 0;
    }
    UnkInlineStr50_l505000(const UnkInlineStr50_l505000& other) : UnkInlineStr50_l505000() {
        fn_710007eaa0(Str(), other.Str());
    }
    UnkInlineStr50_l505000& operator=(const UnkInlineStr50_l505000& other) {
        fn_710007eaa0(Str(), other.Str());
        return *this;
    }
    ~UnkInlineStr50_l505000() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkInlineStrArray_l505000 {
    UnkInlineStr50_l505000 strs[4];
};

// The count is a 64-bit value in the original (it is tested and walked whole), though the capacity
// check reads only its low half.
void fn_7100505000(void* storage, const UnkInlineStr50_l505000* strs, u64 count) {
    auto* self = new (storage) UnkInlineStrArray_l505000;
    const u32 n = static_cast<u32>(count);
    if (n > sizeof(self->strs) / sizeof(self->strs[0]))
        return;
    u32 i = 0;
    for (const UnkInlineStr50_l505000* s = strs; s != strs + count; ++s)
        self->strs[i++] = UnkInlineStr50_l505000(*s);
}
