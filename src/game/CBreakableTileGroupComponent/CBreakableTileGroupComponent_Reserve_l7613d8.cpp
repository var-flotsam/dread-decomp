// build-flags: no-strict-aliasing
// CBreakableTileGroupComponent unit (tools/tu_map.py unit 1167): Reserve of the array of 0x48-byte records
// (base::global::CRntVector's layout): the records are moved to the new storage one by one and the old ones
// destroyed (their entry arrays freed, their string ids released). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkTileEntry_bt {
    virtual ~UnkTileEntry_bt();
    u8 pad_0x8[0x38];
};

// The record of CBreakableTileGroupComponent_b40.cpp, with the move and destruction it has here.
struct UnkTileRecordOwned_l7613d8 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    base::global::CStrId field_0x10;
    base::global::CRntVector<UnkTileEntry_bt> field_0x18;
    u8 field_0x40;  // a byte: the moved-from array is read again after it is stored

    UnkTileRecordOwned_l7613d8(UnkTileRecordOwned_l7613d8&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0x10(other.field_0x10),
          field_0x18(static_cast<base::global::CRntVector<UnkTileEntry_bt>&&>(other.field_0x18)),
          field_0x40(other.field_0x40) {}
    ~UnkTileRecordOwned_l7613d8() { field_0x18.Free(); }
};
static_assert(sizeof(UnkTileRecordOwned_l7613d8) == 0x48, "record layout");

struct UnkTileRecordElem_b43a2 {
    u8 unk_0x0[0x48];
};

using UnkTileRecordVector_l7613d8 = base::global::CRntVector<UnkTileRecordOwned_l7613d8>;

// CRntVector<record>::Reserve.
void fn_71007613d8(base::global::CRntVector<UnkTileRecordElem_b43a2>* array, u32 capacity) {
    UnkTileRecordVector_l7613d8* self = reinterpret_cast<UnkTileRecordVector_l7613d8*>(array);
    if (self->capacity < capacity) {
        UnkTileRecordOwned_l7613d8* p = static_cast<UnkTileRecordOwned_l7613d8*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkTileRecordOwned_l7613d8), 8, self->flags));
        if (self->data) {
            UnkTileRecordOwned_l7613d8* src = self->data;
            UnkTileRecordOwned_l7613d8* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkTileRecordOwned_l7613d8(static_cast<UnkTileRecordOwned_l7613d8&&>(*src));
                src->~UnkTileRecordOwned_l7613d8();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}
