// CHeatableShieldComponent unit (unit 1322): adds a named entry to the shield's entries (+0x40) when it is
// not there yet, then sets its ids and flag from the arguments and looks up the slot its first id's hash
// names in the owner's (+0x30) table. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkTU1323.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CHeatableShieldComponent/CHeatableShieldComponent_Reserve_b52.cpp has it.
struct UnkHeatableValue_0xd0 {
    u8 raw[0xd0];
};
void fn_710086df48(UnkHeatableValue_0xd0* self);  // default constructor
void fn_710086da20(UnkPropertyArrays* self, u32 capacity);
struct UnkHeatableEntry_b0080;
void fn_710086dbb0(UnkHeatableEntry_b0080* self, const UnkHeatableEntry_b0080* other);  // copy constructor

// One entry's view of the 0xd0-byte value.
struct UnkHeatableShieldValue_l86c218 {
    CStrId id;          // 0x0
    u32 slot;           // 0x8
    CStrId field_0x10;
    CStrId field_0x18;
    u8 pad_0x20[0x28];
    CStrId field_0x48;
    u8 pad_0x50[0x28];
    CStrId field_0x78;
    CStrId field_0x80;
    u8 pad_0x88[0x8];
    CStrId field_0x90;
    CStrId field_0x98;
    u8 pad_0xa0[0x2];
    bool field_0xa2;
    u8 pad_0xa3[0x2d];
};
static_assert(sizeof(UnkHeatableShieldValue_l86c218) == 0xd0, "layout");

struct UnkHeatableTable3_l86c218 {
    u8 pad_0x0[0x78];
    void* table;  // 0x78
};
struct UnkHeatableTable2_l86c218 {
    u8 pad_0x0[0x138];
    UnkHeatableTable3_l86c218* field_0x138;
};
struct UnkHeatableTable1_l86c218 {
    u8 pad_0x0[0x50];
    UnkHeatableTable2_l86c218* field_0x50;
};
struct UnkHeatableTable0_l86c218 {
    u8 pad_0x0[0x228];
    UnkHeatableTable1_l86c218* field_0x228;
};

struct UnkHeatableShield_b0080 {
    u8 pad_0x0[0x30];
    UnkHeatableTable0_l86c218* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkPropertyArrays entries;  // 0x40

};

// The entries' values in this unit's view.
static inline UnkHeatableShieldValue_l86c218* Values(UnkPropertyArrays* entries) {
    return static_cast<UnkHeatableShieldValue_l86c218*>(entries->values);
}
static inline UnkHeatableShieldValue_l86c218* End(UnkPropertyArrays* entries) {
    return &Values(entries)[entries->count];
}
// The entry named `id`, or the end of the entries.
static inline UnkHeatableShieldValue_l86c218* Find(UnkPropertyArrays* entries, const CStrId& id) {
    for (u32 i = 0; i < entries->count; ++i) {
        if (entries->ids[i] == id)
            return &Values(entries)[i];
    }
    return End(entries);
}
// The index of the entry named `id`, or the count.
static inline u32 IndexOf(UnkPropertyArrays* entries, const CStrId& id) {
    u32 i = 0;
    for (; i < entries->count; ++i) {
        if (entries->ids[i] == id)
            break;
    }
    return i;
}

void fn_710086c218(UnkHeatableShield_b0080* self, const void* a0, bool a1, const void* a2, const void* a3,
                   const void* a4, const void* a5, const void* a6, const void* a7, const void* a8) {
    const CStrId& name = *static_cast<const CStrId*>(a0);
    UnkPropertyArrays* entries = &self->entries;
    if (Find(entries, name) != End(entries))
        return;

    {
        UnkHeatableValue_0xd0 value;
        fn_710086df48(&value);
        if (entries->count == entries->capacity)
            fn_710086da20(entries, entries->step + entries->count);
        new (&entries->ids[entries->count]) CStrId(name);
        fn_710086dbb0(reinterpret_cast<UnkHeatableEntry_b0080*>(&Values(entries)[entries->count]),
                      reinterpret_cast<const UnkHeatableEntry_b0080*>(&value));
        ++entries->count;
        fn_710086e0ac(&value);
    }

    UnkHeatableShieldValue_l86c218& entry = Values(entries)[IndexOf(entries, name)];
    entry.id = name;
    entry.field_0xa2 = a1;
    entry.field_0x10 = *static_cast<const CStrId*>(a2);
    entry.field_0x18 = *static_cast<const CStrId*>(a3);
    entry.field_0x48 = *static_cast<const CStrId*>(a8);
    entry.field_0x78 = *static_cast<const CStrId*>(a4);
    entry.field_0x80 = *static_cast<const CStrId*>(a5);
    entry.field_0x90 = *static_cast<const CStrId*>(a6);
    entry.field_0x98 = *static_cast<const CStrId*>(a7);

    if (UnkHeatableTable1_l86c218* tables = self->owner->field_0x228) {
        u64 key = entry.field_0x10.data->Key();
        fn_7100199d88(tables->field_0x50->field_0x138->table, &key, &entry.slot);
    }
}
