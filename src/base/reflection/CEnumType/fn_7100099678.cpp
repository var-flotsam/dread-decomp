// base::reflection::CEnumType (tools/tu_map.py unit 76): hashes an enum type: opens a stream (kind 0x16),
// appends its id and the value of each of its entries, and returns the digest of the buffer.
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/reflection/CType.h"
#include "types.h"

struct UnkEnumEntry_l099678 {
    u8 pad_0x0[0x8];
    s32 value;  // 0x8
    u8 pad_0xc[0x4];
};

struct UnkEnumType_l099678 {
    u8 pad_0x0[0xf0];
    UnkEnumEntry_l099678* entries;  // 0xf0
    u32 count;                      // 0xf8

    UnkEnumEntry_l099678* begin() { return entries; }
    UnkEnumEntry_l099678* end() { return entries + count; }
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
s32 fn_7100099678(UnkEnumType_l099678* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s32 id = CType_ComputeBaseHash(reinterpret_cast<UnkHashed*>(self));
    s.PutBytes(&id, 4);
    for (const UnkEnumEntry_l099678& entry : *self)
        s.PutBytes(&entry.value, 4);
    return s.Finish();
}
