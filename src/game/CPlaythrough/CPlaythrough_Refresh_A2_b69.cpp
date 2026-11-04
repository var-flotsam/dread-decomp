// CPlaythrough's unit (the name from reflection): refreshes the flag at +0xa0 of the object the argument holds
// at +0x8: set when its two string ids (+0x58, +0x60) and its file path (+0x68) are all non-empty, then replaced
// by fn_71002ebe84's answer. The result is a fixed 0x18-byte record {0, 0, 2, 1}. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkPlaythroughEntry_A2_b69 {
    u8 pad_0x0[0x58];
    CStrId id_a;           // +0x58
    CStrId id_b;           // +0x60
    CFilePathStrId path;   // +0x68
    u8 pad_0x70[0x30];
    bool valid;  // +0xa0
};

struct UnkPlaythroughArg_A2_b69 {
    u8 pad_0x0[0x8];
    UnkPlaythroughEntry_A2_b69* entry;  // +0x8
};

struct UnkPlaythroughResult_A2_b69 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

bool fn_71002ebe84(UnkPlaythroughEntry_A2_b69* entry);

UnkPlaythroughResult_A2_b69 fn_71002ee14c(void* self, UnkPlaythroughArg_A2_b69* arg) {
    UnkPlaythroughEntry_A2_b69* entry = arg->entry;
    entry->valid = entry->id_a.data != &g_unk_7101d47108->empty && entry->id_b.data != &g_unk_7101d47108->empty &&
                   entry->path.data != &g_unk_7101d47200->empty;
    entry->valid = fn_71002ebe84(entry);
    UnkPlaythroughResult_A2_b69 result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    result.field_0x14 = 1;
    return result;
}
