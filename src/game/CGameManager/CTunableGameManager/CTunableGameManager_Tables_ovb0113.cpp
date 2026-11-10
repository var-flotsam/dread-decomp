// CGameManager::CTunableGameManager's unit (tools/tu_map.py): the second record table's Clear
// (fn_7100e050b0: the string ids in the key array at +0x8 are released, the owned values at +0x0 deleted
// through their vf_0x8, the count at +0x10 zeroed) and the deleting destructor of the object holding the two
// tables (fn_7100e04f2c), as fn_7100e04e80 (CTunableGameManager_Dtor_b43.cpp) with that Clear inlined.
// Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// As src/game/CActorComponent/fn_71002520fc.cpp declares it.
template <class Value>
struct UnkIdRecordTable;
template <class T, void (*Destroy)(T*)>
struct UnkOwned;
void fn_7100ae31f8(UnkIdRecordTable<UnkOwned<UnkKeyedStrIds, fn_7100ae2df8>>* self);

extern char g_unk_7101aa9aa0[];  // the object's vtable

struct UnkRecordTable_b43a3;  // as CTunableGameManager_Dtor_b43.cpp lays it out

struct UnkTunableValue_ovb0113 {
    virtual void vf_0x0();
    virtual void vf_0x8();  // deleting destructor
};

// An owned value's slot, a union member (no type-based alias info: the original reloads the count after
// clearing it).
struct UnkTunableSlot_ovb0113 {
    union {
        UnkTunableValue_ovb0113* ptr;
    };
};

struct UnkTunableId_ovb0113 {
    UnkStringData* data;

    __attribute__((always_inline)) void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkTunableTable_ovb0113 {
    UnkTunableSlot_ovb0113* values;
    UnkTunableId_ovb0113* keys;
    u32 count;
    u8 pad_0x14[0x20 - 0x14];
    base::UnkAllocator* allocator;  // 0x20
    u64 field_0x28;

    __attribute__((always_inline)) void Clear() {
        if (keys && count) {
            for (u64 i = 0; i < count; ++i)
                keys[i].Reset();
        }
        if (values && count) {
            for (u64 i = 0; i < count; ++i) {
                UnkTunableValue_ovb0113* v = values[i].ptr;
                values[i].ptr = nullptr;
                if (v)
                    v->vf_0x8();
            }
        }
        count = 0;
    }
    __attribute__((always_inline)) void Free() {
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

void fn_7100e050b0(UnkRecordTable_b43a3* self) {
    reinterpret_cast<UnkTunableTable_ovb0113*>(self)->Clear();
}

struct UnkTunableTables_ovb0113 {
    char* vtable;
    UnkTunableTable_ovb0113 first;   // 0x8
    UnkTunableTable_ovb0113 second;  // 0x38
};
static_assert(__builtin_offsetof(UnkTunableTables_ovb0113, second) == 0x38, "UnkTunableTables_ovb0113 layout");

void fn_7100e04f2c(UnkTunableTables_ovb0113* self) {
    self->vtable = g_unk_7101aa9aa0 + 0x10;
    self->second.Clear();
    self->second.Free();
    fn_7100ae31f8(reinterpret_cast<UnkIdRecordTable<UnkOwned<UnkKeyedStrIds, fn_7100ae2df8>>*>(&self->first));
    self->first.Free();
    operator delete(self);
}
