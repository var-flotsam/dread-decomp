// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): the move helper (CType+0x38) of
// the iterator value over the keyed array of 0x118-byte records by u32 key. A record's move: a word, three
// string ids taken over, plain words and blocks, a string moved (fn_710007eab4) and a string with 0x20 bytes
// of storage copied. Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;

namespace {

// A string id that is moved from: the source gets the empty string.
struct UnkMovedId_lead260 {
    UnkStringData* data;

    UnkMovedId_lead260(UnkMovedId_lead260&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

// A string moved by its move constructor (fn_710007eab4).
struct UnkMovedString_lead260 {
    u8 data[0x30];

    UnkMovedString_lead260(UnkMovedString_lead260&& other) {
        fn_710007eab4(reinterpret_cast<CRntString*>(this), reinterpret_cast<CRntString*>(&other));
    }
};

struct UnkWords5_lead260 {
    u32 field_0x0[5];
};
struct UnkWords3_lead260 {
    u32 field_0x0[3];
};
struct UnkHalves3_lead260 {
    u16 field_0x0[3];
};

// The record's second half, from +0x40.
struct UnkRecordTail_lead260 {
    UnkWords5_lead260 field_0x0;
    u32 unk_0x14;
    UnkMovedString_lead260 field_0x18;
    u32 field_0x48;
    u32 unk_0x4c;
    UnkWords3_lead260 field_0x50;
    UnkBufferString<0x20, g_unk_7101a1b5a0> field_0x60;
    u32 field_0xc8;
    u32 unk_0xcc;
    UnkHalves3_lead260 field_0xd0;

    UnkRecordTail_lead260(UnkRecordTail_lead260&& other)
        : field_0x0(other.field_0x0), field_0x18(static_cast<UnkMovedString_lead260&&>(other.field_0x18)),
          field_0x48(other.field_0x48), field_0x50(other.field_0x50), field_0x60(other.field_0x60),
          field_0xc8(other.field_0xc8), field_0xd0(other.field_0xd0) {}
};

struct UnkRecord118_lead260 {
    u64 field_0x0;
    UnkMovedId_lead260 field_0x8;
    UnkMovedId_lead260 field_0x10;
    UnkMovedId_lead260 field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 unk_0x3c;
    UnkRecordTail_lead260 field_0x40;

    UnkRecord118_lead260(UnkRecord118_lead260&& other)
        : field_0x0(other.field_0x0), field_0x8(static_cast<UnkMovedId_lead260&&>(other.field_0x8)),
          field_0x10(static_cast<UnkMovedId_lead260&&>(other.field_0x10)),
          field_0x18(static_cast<UnkMovedId_lead260&&>(other.field_0x18)), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), field_0x28(other.field_0x28), field_0x2c(other.field_0x2c),
          field_0x30(other.field_0x30), field_0x34(other.field_0x34), field_0x38(other.field_0x38),
          field_0x40(static_cast<UnkRecordTail_lead260&&>(other.field_0x40)) {}
};
static_assert(sizeof(UnkRecord118_lead260) == 0x118, "UnkRecord118_lead260 layout");

}  // namespace

void fn_7100ead260(void* dst, const void* src) {
    base::refl_IterValueMove<UnkRecord118_lead260>(dst, src);
}
