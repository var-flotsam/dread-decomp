// From unit 374 of tools/tu_map.py (0x71002b525c-0x71002b563c, no reflected class): an object
// with a list of 4-byte keys (an id and a kind, base/UnkIdKindKey.h: built by fn_71002874d0 and
// destroyed by the empty fn_71002874dc) after two header words, read back from a file in memory:
// the header, a count, then each key's id and kind as 32-bit words. Names and layouts are
// placeholders.
// Holds fn_71002b5338.
#include "base/UnkIdKindKey.h"
#include "base/global/CRntVector.h"
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkKeyList {
    u8 pad_0x0[0x8];
    base::global::CRntVector<UnkIdKindKey> keys;
    u16 field_0x30;
    u8 field_0x32;
    u8 pad_0x33;
    u32 field_0x34[2];  // read as one 8-byte element
};
static_assert(__builtin_offsetof(UnkKeyList, field_0x30) == 0x30, "UnkKeyList placeholder layout");

// Reads the header and the keys from `file`, appending them.
__attribute__((flatten)) void fn_71002b5338(UnkKeyList* self, base::global::CRntFile* file) {
    u32 word0;
    CRntFile_Read(file, &word0, 4, 1);
    self->field_0x30 = word0;
    u32 word1;
    CRntFile_Read(file, &word1, 4, 1);
    self->field_0x32 = word1;
    CRntFile_Read(file, &self->field_0x34, 8, 1);
    s32 count = 0;
    CRntFile_Read(file, &count, 4, 1);
    self->keys.Reserve(count);
    for (s32 i = 0; i < count; ++i) {
        u32 id;
        CRntFile_Read(file, &id, 4, 1);
        u32 kind;
        CRntFile_Read(file, &kind, 4, 1);
        UnkIdKindKey key(id, kind);
        self->keys.EmplaceBack(static_cast<UnkIdKindKey&&>(key));
    }
}
