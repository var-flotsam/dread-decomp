// Unit 2666 (lua_bind): empties a container of two parallel arrays sharing a count: string ids, and
// arrays of 0x160-byte records. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/tunable/CTunable.h"
#include "types.h"

struct UnkIdsString_133be4;

struct UnkRecord_l097ab8 {
    u8 pad_0x0[0x160];
    ~UnkRecord_l097ab8() { fn_7100133be4(reinterpret_cast<UnkIdsString_133be4*>(this)); }
};

struct UnkRecordArrays_l097ab8 {
    base::global::CRntVector<UnkRecord_l097ab8>* lists;  // 0x0
    base::UnkStrIdMember* ids;                           // 0x8
    u32 count;                                           // 0x10
};

void fn_7101097ab8(UnkRecordArrays_l097ab8* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMemberOf();
    }
    if (self->lists) {
        for (u64 i = 0; i < self->count; ++i)
            self->lists[i].Free();
    }
    self->count = 0;
}
