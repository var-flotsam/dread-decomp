// CMinimapData's unit (2563): empties the arrays and tables at the end of a CMinimapData (from +0x5fd8),
// keeping their storage. Names and layouts are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CMinimapData.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkRec60IdArray_b0223;
template <class Value>
struct UnkIdRecordTable;
struct UnkIdRecord28;
void fn_7100e4ee60(UnkIdRecordTable<UnkIdRecord28>* self);
template <class Record>
struct UnkRecordIdArray;
struct UnkRecordA8;
struct UnkRecord80;
void fn_7100e4ed10(UnkRecordIdArray<UnkRecordA8>* self);
void fn_7100e4eb8c(UnkRecordIdArray<UnkRecord80>* self);
template <base::global::UnkStringPool** Pool, bool Reread>
struct UnkReleasedStrIdT;
template <class Value>
struct UnkIdTable;
using UnkReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47108, true>;
void fn_71000d99d0(UnkIdTable<UnkReleasedStrId>* self);

// A table whose clear is out of line: values, string-id keys, count, capacity, step, flags, allocator.
struct UnkOpaqueTable_le3e7b8 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// An array of array pairs, destroyed out of line.
struct UnkPairArrayEntry_le3e7b8 {
    UnkSecondPtrArray<UnkFreedVectorPair> array;

    ~UnkPairArrayEntry_le3e7b8() { fn_7100e493f4(&array); }
};

// A 0x60-byte record, destroyed out of line.
struct UnkRecord60Entry_le3e7b8 {
    u8 bytes[0x60];

    ~UnkRecord60Entry_le3e7b8() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }
};

struct UnkMinimapTables_le3e7b8 {
    u8 pad_0x0[0x5fd8];
    base::global::CRntVector<UnkFreedVectorPair> field_0x5fd8;
    base::global::CRntSmallDictionary<CStrId, UnkPairArrayEntry_le3e7b8> field_0x6000;
    UnkOpaqueTable_le3e7b8 field_0x6028;
    UnkOpaqueTable_le3e7b8 field_0x6050;
    UnkOpaqueTable_le3e7b8 field_0x6078;
    UnkOpaqueTable_le3e7b8 field_0x60a0;
    UnkOpaqueTable_le3e7b8 field_0x60c8;
    UnkOpaqueTable_le3e7b8 field_0x60f0;
    UnkOpaqueTable_le3e7b8 field_0x6118;
    base::global::CRntSmallDictionary<CStrId, UnkRecord60Entry_le3e7b8> field_0x6140;
    UnkOpaqueTable_le3e7b8 field_0x6168;
    UnkOpaqueTable_le3e7b8 field_0x6190;
    UnkOpaqueTable_le3e7b8 field_0x61b8;
    UnkOpaqueTable_le3e7b8 field_0x61e0;
    UnkOpaqueTable_le3e7b8 field_0x6208;
    UnkOpaqueTable_le3e7b8 field_0x6230;
    UnkOpaqueTable_le3e7b8 field_0x6258;
    base::global::CRntVector<CMinimapData::SMovingMagnetSurfaceData> aMovingMagnetSurfaces;  // 0x6280
    UnkOpaqueTable_le3e7b8 field_0x62a8;
    UnkOpaqueTable_le3e7b8 field_0x62d0;
    UnkOpaqueTable_le3e7b8 field_0x62f8;
};
static_assert(__builtin_offsetof(UnkMinimapTables_le3e7b8, field_0x6140) == 0x6140, "layout");
static_assert(__builtin_offsetof(UnkMinimapTables_le3e7b8, aMovingMagnetSurfaces) == 0x6280, "layout");
static_assert(__builtin_offsetof(UnkMinimapTables_le3e7b8, field_0x62f8) == 0x62f8, "layout");

inline void ClearRec60Table_le3e7b8(UnkOpaqueTable_le3e7b8* table) {
    fn_7100e4efa0(reinterpret_cast<UnkRec60IdArray_b0223*>(table));
}
inline void ClearRecord28Table_le3e7b8(UnkOpaqueTable_le3e7b8* table) {
    fn_7100e4ee60(reinterpret_cast<UnkIdRecordTable<UnkIdRecord28>*>(table));
}
inline void ClearIdTable_le3e7b8(UnkOpaqueTable_le3e7b8* table) {
    fn_71000d99d0(reinterpret_cast<UnkIdTable<UnkReleasedStrId>*>(table));
}

void fn_7100e3e7b8(UnkMinimapTables_le3e7b8* self) {
    self->field_0x5fd8.Clear();
    self->field_0x6000.Clear();
    ClearRec60Table_le3e7b8(&self->field_0x6078);
    ClearRec60Table_le3e7b8(&self->field_0x60a0);
    ClearRec60Table_le3e7b8(&self->field_0x60c8);
    ClearRec60Table_le3e7b8(&self->field_0x60f0);
    ClearRec60Table_le3e7b8(&self->field_0x6050);
    self->field_0x6140.Clear();
    ClearIdTable_le3e7b8(&self->field_0x62d0);
    ClearIdTable_le3e7b8(&self->field_0x62f8);
    ClearRecord28Table_le3e7b8(&self->field_0x6168);
    ClearRecord28Table_le3e7b8(&self->field_0x6190);
    ClearRecord28Table_le3e7b8(&self->field_0x61b8);
    ClearRecord28Table_le3e7b8(&self->field_0x61e0);
    ClearRecord28Table_le3e7b8(&self->field_0x6208);
    ClearRecord28Table_le3e7b8(&self->field_0x6230);
    ClearRec60Table_le3e7b8(&self->field_0x6028);
    ClearRec60Table_le3e7b8(&self->field_0x6118);
    fn_7100e4ed10(reinterpret_cast<UnkRecordIdArray<UnkRecordA8>*>(&self->field_0x6258));
    self->aMovingMagnetSurfaces.Clear();
    fn_7100e4eb8c(reinterpret_cast<UnkRecordIdArray<UnkRecord80>*>(&self->field_0x62a8));
}
