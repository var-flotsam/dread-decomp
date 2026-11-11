// CHeatableShieldComponent's unit (tools/tu_map.py unit 1322). The component keeps its 0xd0-byte entries
// (+0x40) and the string ids that name them (+0x48) as a map with one count (+0x50) and an allocator
// (+0x60). Here: its destructor (fn_710086ad24: the ids released, each entry destroyed by fn_710086e0ac,
// both arrays freed), the constructor of the four groups of three arrays and a flag (fn_710086abb4), an
// entry's copy constructor (fn_710086dbb0), and three reflected functions' invokers: fn_710086cc04
// passes its nine arguments to fn_710086c218; fn_710086d380 adds a name to the named entry's list
// (+0x20) and fn_710086d530 sets its id at +0x88. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkParallelFind.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CHeatableShieldComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU1323.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkHeatableEntryD_b0080;
struct UnkHeatableExtra_b0080;
void fn_710086ae60(UnkHeatableExtra_b0080* self);
struct UnkHeatableValue_0xd0;

struct UnkHeatableEntryD_b0080 {
    u8 unk_0x0[0xd0];
};

struct UnkHeatableShieldD_b0080 {
    void** vtable;
    u8 unk_0x8[0x38];
    UnkHeatableEntryD_b0080* entries;  // 0x40
    base::UnkStrIdMember* ids;        // 0x48
    u32 count;                        // 0x50
    u8 unk_0x54[0xc];
    base::UnkAllocator* allocator;    // 0x60
    UnkHeatableExtra_b0080* extra[1];  // 0x68
};
static_assert(__builtin_offsetof(UnkHeatableShieldD_b0080, extra) == 0x68, "layout");

void fn_710086ad24(UnkHeatableShieldD_b0080* self) {
    self->vtable = g_unk_7101a7a260 + 2;
    fn_710086ae60(reinterpret_cast<UnkHeatableExtra_b0080*>(self->extra));
    if (self->ids) {
        for (u32 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMemberOf();
    }
    if (self->entries) {
        for (u32 i = 0; i < self->count; ++i)
            fn_710086e0ac(reinterpret_cast<UnkHeatableValue_0xd0*>(&self->entries[i]));
    }
    self->count = 0;
    if (self->ids)
        self->allocator->vf_0x48(self->ids);
    if (self->entries)
        self->allocator->vf_0x48(self->entries);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

// Three arrays and a flag.
struct UnkHeatableGroup_b0080 {
    base::UnkVector<void*> a{0xf};
    base::UnkVector<void*> b{0xf};
    base::UnkVector<void*> c{0xf};
    bool flag = false;
};
static_assert(sizeof(UnkHeatableGroup_b0080) == 0x80, "layout");

struct UnkHeatableGroups_b0080 {
    UnkHeatableGroup_b0080 groups[4];
};

__attribute__((flatten)) void fn_710086abb4(UnkHeatableGroups_b0080* self) {
    new (self) UnkHeatableGroups_b0080;
}

struct UnkHeatableBlock20_b0080 {
    u64 words[4];
};
struct UnkHeatableBlock30_b0080 {
    u64 words[6];
};

struct UnkHeatableEntry_b0080 {
    CStrId id;                         // 0x0
    u32 field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    base::UnkStrIdVec names;           // 0x20
    CStrId field_0x48;
    u64 field_0x50;
    UnkHeatableBlock20_b0080 field_0x58;
    CStrId field_0x78;
    CStrId field_0x80;
    CStrId field_0x88;
    CStrId field_0x90;
    CStrId field_0x98;
    UnkHeatableBlock30_b0080 field_0xa0;
};
static_assert(sizeof(UnkHeatableEntry_b0080) == 0xd0, "layout");

__attribute__((flatten)) void fn_710086dbb0(UnkHeatableEntry_b0080* self, const UnkHeatableEntry_b0080* other) {
    new (self) UnkHeatableEntry_b0080(*other);
}

struct UnkHeatableShield_b0080;
void fn_710086c218(UnkHeatableShield_b0080* self, const void* a0, bool a1, const void* a2, const void* a3,
                   const void* a4, const void* a5, const void* a6, const void* a7, const void* a8);

static inline __attribute__((always_inline)) void* ArgPtr_b0080(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

// A pointer argument: null without a value.
static inline __attribute__((always_inline)) void* OptArgPtr_b0080(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    return ArgPtr_b0080(arg);
}

void fn_710086cc04(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkHeatableShield_b0080**>(obj + 8);
    fn_710086c218(self, OptArgPtr_b0080(&a[0]), *static_cast<const bool*>(ArgPtr_b0080(&a[1])),
                  OptArgPtr_b0080(&a[2]), OptArgPtr_b0080(&a[3]), OptArgPtr_b0080(&a[4]), OptArgPtr_b0080(&a[5]),
                  OptArgPtr_b0080(&a[6]), OptArgPtr_b0080(&a[7]), OptArgPtr_b0080(&a[8]));
}

struct UnkHeatableEntryR_b0080 {
    u8 unk_0x0[0x20];
    base::UnkStrIdVec names;  // 0x20
    u8 unk_0x48[0x40];
    base::UnkStrIdMember field_0x88;
    u8 unk_0x90[0x40];
};
static_assert(sizeof(UnkHeatableEntryR_b0080) == 0xd0, "layout");

struct UnkHeatableEntries_b0080 {
    u8 unk_0x0[0x40];
    UnkHeatableEntryR_b0080* values;  // 0x40
    CStrId* ids;                     // 0x48
    u32 count;                       // 0x50
};

void fn_710086d380(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkHeatableEntries_b0080**>(obj + 8);
    const CStrId* key = static_cast<const CStrId*>(OptArgPtr_b0080(&a[0]));
    const CStrId* name = static_cast<const CStrId*>(OptArgPtr_b0080(&a[1]));
    UnkHeatableEntryR_b0080* entry = UnkParallelFindSlot(self, *key);
    if (entry != &self->values[self->count])
        entry->names.PushBack(*name);
}

UnkReflValue fn_710086d530(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkHeatableEntries_b0080**>(obj + 8);
    const CStrId* key = static_cast<const CStrId*>(OptArgPtr_b0080(&a[0]));
    const auto* id = static_cast<const base::UnkStrIdMember*>(OptArgPtr_b0080(&a[1]));
    UnkHeatableEntryR_b0080* entry = UnkParallelFindSlot(self, *key);
    if (entry != &self->values[self->count])
        entry->field_0x88 = *id;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
