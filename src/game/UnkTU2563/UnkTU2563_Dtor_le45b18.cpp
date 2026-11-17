// CMinimapData's unit (2563): CMinimapData's destructor. It empties and frees its arrays and tables (from
// +0x5fd8 to the end), then runs the named secondary base's and the asset base's destructors.
// Names and layouts are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/core/CAsset.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CChangeStageNavMeshItemComponent.h"
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

extern void* g_unk_7101aa9cf8[];  // CMinimapData's vtable

// The clears of the tables below, out of line.
struct UnkRec60Clear_le45b18 {
    static void Clear(void* table) { fn_7100e4efa0(reinterpret_cast<UnkRec60IdArray_b0223*>(table)); }
};
struct UnkRecord28Clear_le45b18 {
    static void Clear(void* table) { fn_7100e4ee60(reinterpret_cast<UnkIdRecordTable<UnkIdRecord28>*>(table)); }
};
struct UnkRecordA8Clear_le45b18 {
    static void Clear(void* table) { fn_7100e4ed10(reinterpret_cast<UnkRecordIdArray<UnkRecordA8>*>(table)); }
};
struct UnkRecord80Clear_le45b18 {
    static void Clear(void* table) { fn_7100e4eb8c(reinterpret_cast<UnkRecordIdArray<UnkRecord80>*>(table)); }
};
struct UnkIdClear_le45b18 {
    static void Clear(void* table) { fn_71000d99d0(reinterpret_cast<UnkIdTable<UnkReleasedStrId>*>(table)); }
};
struct UnkNavMeshClear_le45b18 {
    static void Clear(void* table) { fn_71009ba68c(reinterpret_cast<UnkNavMeshItemMember*>(table)); }
};

// A table of values and keys whose clear is out of line; destroying it clears it and frees both arrays.
template <class Ops>
struct UnkTable_le45b18 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkTable_le45b18() {
        Ops::Clear(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// A dictionary that frees its arrays when destroyed.
template <class K, class V>
struct UnkOwnedDictionary_le45b18 : base::global::CRntSmallDictionary<K, V> {
    ~UnkOwnedDictionary_le45b18() {
        this->Clear();
        if (this->keys)
            this->allocator->vf_0x48(this->keys);
        if (this->values)
            this->allocator->vf_0x48(this->values);
    }
};

// A vector that frees its array when destroyed.
template <class T>
struct UnkOwnedVector_le45b18 : base::global::CRntVector<T> {
    ~UnkOwnedVector_le45b18() { this->Free(); }
};

// An array of array pairs, destroyed out of line.
struct UnkPairArrayEntry_le45b18 {
    UnkSecondPtrArray<UnkFreedVectorPair> array;

    ~UnkPairArrayEntry_le45b18() { fn_7100e493f4(&array); }
};

// A 0x60-byte record, destroyed out of line.
struct UnkRecord60Entry_le45b18 {
    u8 bytes[0x60];

    ~UnkRecord60Entry_le45b18() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }
};

// CMinimapData: the asset base (0x60 bytes, its destructor out of line), a secondary base at +0x60 (a
// string id and its own vtable at +0x18), then the tables.
struct UnkMinimapData_le45b18 {
    void** vtable;
    u8 unk_0x8[0x58];
    CStrId name;            // 0x60
    u64 field_0x68;
    u64 field_0x70;
    void** namedVtable;     // 0x78
    u8 unk_0x80[0x5f58];
    UnkOwnedVector_le45b18<UnkFreedVectorPair> field_0x5fd8;
    UnkOwnedDictionary_le45b18<CStrId, UnkPairArrayEntry_le45b18> field_0x6000;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x6028;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x6050;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x6078;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x60a0;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x60c8;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x60f0;
    UnkTable_le45b18<UnkRec60Clear_le45b18> field_0x6118;
    UnkOwnedDictionary_le45b18<CStrId, UnkRecord60Entry_le45b18> mapDoors;  // 0x6140
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x6168;
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x6190;
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x61b8;
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x61e0;
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x6208;
    UnkTable_le45b18<UnkRecord28Clear_le45b18> field_0x6230;
    UnkTable_le45b18<UnkRecordA8Clear_le45b18> field_0x6258;
    UnkOwnedVector_le45b18<CMinimapData::SMovingMagnetSurfaceData> aMovingMagnetSurfaces;  // 0x6280
    UnkTable_le45b18<UnkRecord80Clear_le45b18> field_0x62a8;
    UnkTable_le45b18<UnkIdClear_le45b18> field_0x62d0;
    UnkTable_le45b18<UnkIdClear_le45b18> field_0x62f8;
    UnkTable_le45b18<UnkNavMeshClear_le45b18> field_0x6320;
    u8 unk_0x6348[0x10];
};
static_assert(sizeof(UnkMinimapData_le45b18) == 0x6358, "layout");

// Destroys the members in reverse order, then the secondary base and the asset base.
void fn_7100e45b18(UnkMinimapData_le45b18* self) {
    self->vtable = &g_unk_7101aa9cf8[2];
    self->field_0x6320.~UnkTable_le45b18();
    self->field_0x62f8.~UnkTable_le45b18();
    self->field_0x62d0.~UnkTable_le45b18();
    self->field_0x62a8.~UnkTable_le45b18();
    self->aMovingMagnetSurfaces.~UnkOwnedVector_le45b18();
    self->field_0x6258.~UnkTable_le45b18();
    self->field_0x6230.~UnkTable_le45b18();
    self->field_0x6208.~UnkTable_le45b18();
    self->field_0x61e0.~UnkTable_le45b18();
    self->field_0x61b8.~UnkTable_le45b18();
    self->field_0x6190.~UnkTable_le45b18();
    self->field_0x6168.~UnkTable_le45b18();
    self->mapDoors.~UnkOwnedDictionary_le45b18();
    self->field_0x6118.~UnkTable_le45b18();
    self->field_0x60f0.~UnkTable_le45b18();
    self->field_0x60c8.~UnkTable_le45b18();
    self->field_0x60a0.~UnkTable_le45b18();
    self->field_0x6078.~UnkTable_le45b18();
    self->field_0x6050.~UnkTable_le45b18();
    self->field_0x6028.~UnkTable_le45b18();
    self->field_0x6000.~UnkOwnedDictionary_le45b18();
    self->field_0x5fd8.~UnkOwnedVector_le45b18();

    self->namedVtable = &g_unk_7101aa9d38[2];
    self->name.~CStrId();
    fn_7100016f88(self);
}
