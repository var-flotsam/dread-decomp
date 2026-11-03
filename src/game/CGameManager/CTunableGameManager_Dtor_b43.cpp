// CGameManager::CTunableGameManager's unit (tools/tu_map.py): a destructor of an object holding two
// record tables (at +0x8 and +0x38): each is cleared out of line (fn_7100ae31f8, fn_7100e050b0), then
// its two arrays are freed through its allocator, the second first. Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "types.h"

// As src/game/CActorComponent/fn_71002520fc.cpp declares it.
template <class Value>
struct UnkIdRecordTable;
template <class T, void (*Destroy)(T*)>
struct UnkOwned;
void fn_7100ae31f8(UnkIdRecordTable<UnkOwned<UnkKeyedStrIds, fn_7100ae2df8>>* self);

extern char g_unk_7101aa9aa0[];  // the object's vtable

struct UnkRecordTable_b43a3 {
    void* keys;
    void* values;
    u64 field_0x10;
    u64 field_0x18;
    base::UnkAllocator* allocator;
    u64 field_0x28;

    // Frees the arrays, the values first.
    __attribute__((always_inline)) void Free() {
        if (values)
            allocator->vf_0x48(values);
        if (keys)
            allocator->vf_0x48(keys);
    }
};

void fn_7100e050b0(UnkRecordTable_b43a3* self);

struct UnkTunableTables_b43a3 {
    char* vtable;
    UnkRecordTable_b43a3 first;   // 0x8
    UnkRecordTable_b43a3 second;  // 0x38
};
static_assert(__builtin_offsetof(UnkTunableTables_b43a3, second) == 0x38, "UnkTunableTables_b43a3 layout");

void fn_7100e04e80(UnkTunableTables_b43a3* self) {
    self->vtable = g_unk_7101aa9aa0 + 0x10;
    fn_7100e050b0(&self->second);
    self->second.Free();
    fn_7100ae31f8(reinterpret_cast<UnkIdRecordTable<UnkOwned<UnkKeyedStrIds, fn_7100ae2df8>>*>(&self->first));
    self->first.Free();
}
