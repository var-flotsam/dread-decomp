// CMinimapData's unit (2563): erases one element of an array of 0x80-byte records (an id and an array
// pair each) with a string id per record alongside: the element is destroyed and the last one moved
// into its place. Returns the element's slot. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneCalls.h"
#include "base/UnkDestructors.h"
#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdMember;

struct UnkIdPairRecord_le6164c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    UnkStrIdMember id;
    UnkFreedVectorPair pair;
    u32 field_0x78;

    // Takes over the other's id and arrays.
    UnkIdPairRecord_le6164c(UnkIdPairRecord_le6164c&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), id(static_cast<UnkStrIdMember&&>(other.id)),
          pair(static_cast<UnkFreedVectorPair&&>(other.pair)), field_0x78(other.field_0x78) {}
};
static_assert(sizeof(UnkIdPairRecord_le6164c) == 0x80, "");

struct UnkIdPairRecordArray_le6164c {
    UnkIdPairRecord_le6164c* records;
    UnkStrIdMember* ids;
    u32 count;
};

// The array's types as its caller (src/game/CActorComponent/fn_71002519a8.cpp) declares them.
struct UnkKeyedValues_7100e6095c;
struct UnkValue_0x80;
using UnkValues80ByKey = base::UnkKeyedArray<UnkKeyedValues_7100e6095c, u64, UnkValue_0x80>;

UnkValue_0x80* fn_7100e6164c(UnkValues80ByKey* array, UnkValue_0x80* value) {
    UnkIdPairRecordArray_le6164c* self = reinterpret_cast<UnkIdPairRecordArray_le6164c*>(array);
    u32 index = reinterpret_cast<UnkIdPairRecord_le6164c*>(value) - self->records;
    self->records[index].~UnkIdPairRecord_le6164c();
    self->ids[index].~UnkStrIdMember();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkIdPairRecord_le6164c(static_cast<UnkIdPairRecord_le6164c&&>(self->records[self->count - 1]));
        self->records[self->count - 1].~UnkIdPairRecord_le6164c();
        new (&self->ids[index]) UnkStrIdMember(static_cast<UnkStrIdMember&&>(self->ids[self->count - 1]));
        self->ids[self->count - 1].~UnkStrIdMember();
    }
    --self->count;
    return reinterpret_cast<UnkValue_0x80*>(&self->records[index]);
}
