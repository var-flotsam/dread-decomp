// base::reflection's unit (tools/tu_map.py, unit 2610): builds the lower-case file-path id
// "subarea_<a>_<b>[_<c>]" from three named ids (fn_7100fa6ad0). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CRntString;

void fn_710007f564(void* self, const char* str);  // appends a string

// What a named id points at: its text and a use count.
struct UnkNamedIdData_lfa6ad0 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u32 field_0x40;
    // A union member, so the count's store forces the reload of the id the original has.
    union {
        s32 use_count;  // 0x44
    };
};
static_assert(__builtin_offsetof(UnkNamedIdData_lfa6ad0, use_count) == 0x44, "use_count");

struct UnkNamedId_lfa6ad0 {
    UnkNamedIdData_lfa6ad0* data;

    // Counts a use and gives the text.
    const CRntString* Text() const {
        ++data->use_count;
        return &data->name;
    }
};
inline bool operator!=(const UnkNamedId_lfa6ad0& a, const UnkNamedId_lfa6ad0& b) { return a.data != b.data; }

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
struct UnkInlineString_lfa6ad0 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_lfa6ad0() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    explicit UnkInlineString_lfa6ad0(const char* text) : UnkInlineString_lfa6ad0() {
        fn_710007e77c(Str(), text);
    }
    ~UnkInlineString_lfa6ad0() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// A file-path id built from a string; returned in memory.
struct UnkPathId_lfa6ad0 {
    base::global::UnkFilePathStrIdBase id;
    explicit UnkPathId_lfa6ad0(const CRntString& str) { fn_710001a270(&id, str, true); }
    UnkPathId_lfa6ad0(const UnkPathId_lfa6ad0& other);
};

// Copies `src` into `dst` with A-Z turned into a-z.
static inline void CopyLower_lfa6ad0(char* dst, const char* src) {
    dst[0] = 0;
    u32 length = static_cast<u32>(__builtin_strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'A') < 26)
                c += 0x20;
            dst[i] = c;
        }
        dst[length] = 0;
    }
}

UnkPathId_lfa6ad0 fn_7100fa6ad0(void* self, const UnkNamedId_lfa6ad0* area, const UnkNamedId_lfa6ad0* first,
                                const UnkNamedId_lfa6ad0* second) {
    char lower[0x100];  // declared first: it sits above both strings in the frame
    UnkInlineString_lfa6ad0 text("SubArea_");
    fn_710007f3b4(text.Str(), area->Text());
    fn_710007f564(text.Str(), "_");
    fn_710007f3b4(text.Str(), first->Text());
    if (*first != reinterpret_cast<UnkNamedId_lfa6ad0&>(GameManagerIdTable().field_0x11a8)) {
        fn_710007f564(text.Str(), "_");
        fn_710007f3b4(text.Str(), second->Text());
    }

    CopyLower_lfa6ad0(lower, text.Str()->data);
    UnkInlineString_lfa6ad0 lowered(lower);
    return UnkPathId_lfa6ad0(*lowered.Str());
}
