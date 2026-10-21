// Whether an id is known to a keyed table: queued for addition, or already in the table.
// It first initialises two function-local static masks it doesn't use.
#include "types.h"

struct UnkMaskA {
    u32 value;
    UnkMaskA() : value(0) {}
};

struct UnkMaskB {
    u32 value;
    UnkMaskB(u8 v) : value(v) {}
};

inline const UnkMaskA& UnkMaskNone_7101d57cc8() {
    static UnkMaskA s_none;
    return s_none;
}

inline const UnkMaskB& UnkMaskAll_7101d57cb8() {
    static UnkMaskB s_all((u8)~*reinterpret_cast<const u8*>(&UnkMaskNone_7101d57cc8().value));
    return s_all;
}

struct UnkTableEntry {
    u64 field_0x0;
    u8 unk_0x8[0x38];
};

// Values and keys in parallel arrays.
struct UnkKeyedTable {
    UnkTableEntry* values;
    u64* keys;
    u32 count;

    UnkTableEntry* Find(const u64& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
    UnkTableEntry* End() { return &values[count]; }
};

struct UnkPendingAdd {
    u64 key;
    u32 value;
};

struct UnkPendingList {
    UnkPendingAdd* data;
    u32 count;

    UnkPendingAdd* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (data[i].key == key)
                return &data[i];
        return &data[count];
    }
    UnkPendingAdd* End() { return &data[count]; }
};

struct UnkTableOwner {
    u8 unk_0x0[0x68];
    UnkKeyedTable table;
    u8 unk_0x80[0x10];
    bool locked;
    UnkPendingList pending_add;
};

bool fn_7100a2be94(UnkTableOwner* self, const u64* id) {
    u64 key = *id;
    UnkMaskAll_7101d57cb8();
    UnkPendingAdd* p = self->pending_add.Find(key);
    if (p == self->pending_add.End()) {
        UnkTableEntry* it = self->table.Find(*id);
        if (it == self->table.End())
            return false;
    }
    return true;
}
