#pragma once

#include "base/UnkKeyedStrIds.h"

// The keyed string-id table as this unit's collection holds it, with its move assignment.
// The keyed string-id table at +0x28, with its move assignment.
struct UnkKeyedIdTable_laed78c : UnkKeyedStrIds {
    // Releases every id; the arrays are kept.
    void Clear() {
        if (ids) {
            for (u32 i = 0; i < count; ++i)
                ids[i].~UnkStrIdMemberOf();
        }
        count = 0;
    }

    // Takes over other's arrays, leaving it empty.
    UnkKeyedIdTable_laed78c& operator=(UnkKeyedIdTable_laed78c&& other) {
        Clear();
        allocator = other.allocator;
        ids = other.ids;
        keys = other.keys;
        capacity = other.capacity;
        count = other.count;
        field_0x1c = other.field_0x1c;
        field_0x18 = other.field_0x18;
        other.count = 0;
        other.keys = nullptr;
        other.ids = nullptr;
        return *this;
    }
};
