// Unit 90 of tools/tu_map.py (no class hint): starts a writer over a base::global::CRntFile: when the
// version entry has a version (fn_71000e6ce0), the file gets its id (the 64-bit word at +0x30 of the
// entry's descriptor, +0x8), major and minor first (the header fn_710000ccd8 reads back), then the
// rest is written by fn_71000ed2b8. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "game/UnkTU85.h"
#include "types.h"

struct UnkVersionEntry;

struct UnkVersionDesc_A1b71 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkVersionEntryView_A1b71 {
    u8 pad_0x0[0x8];
    UnkVersionDesc_A1b71* desc;  // 0x8
};

struct UnkVersionWriter_A1b71 {
    u32 field_0x0;
    u8 pad_0x4[0xc84];
    base::global::CRntFile* file;  // 0xc88
};

void fn_71000ed2b8(UnkVersionWriter_A1b71* self, UnkVersionEntry* entry);

void fn_71000ed224(UnkVersionWriter_A1b71* self, base::global::CRntFile* file, UnkVersionEntry* entry) {
    self->field_0x0 = 0;
    self->file = file;
    u16 major = 0;
    u16 minor = 0;
    if (fn_71000e6ce0(entry, &major, &minor)) {
        UnkVersionDesc_A1b71* desc = reinterpret_cast<UnkVersionEntryView_A1b71*>(entry)->desc;
        base::global::CRntFile* out = self->file;
        u64 id = desc->id;
        fn_710007c3f0(out, &id, 8);
        fn_710007c3f0(self->file, &major, 2);
        fn_710007c3f0(self->file, &minor, 2);
    }
    fn_71000ed2b8(self, entry);
}
