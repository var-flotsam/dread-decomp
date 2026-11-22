// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): sets the 0x100-byte record of a key in the
// global keyed array g_unk_71729ce018: assigns it when the key is there, else appends the key and a copy of the
// record (growing by the array's step through fn_7100ea9934). Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "game/UnkTU2567.h"
#include "types.h"

extern void* g_unk_7101a1b5a0[];

// A string with 0x20 bytes of storage, assigned through fn_710007eaa0.
struct UnkString20_e8bc : UnkBufferString<0x20, g_unk_7101a1b5a0> {
    UnkString20_e8bc(const UnkString20_e8bc& other) = default;
    UnkString20_e8bc& operator=(const UnkString20_e8bc& other) {
        CRntString_Assign(Str(), other.Str());
        return *this;
    }
};

// A word copied on its own (a value type with its own copy and assignment).
struct UnkWord_e8bc {
    u32 value;

    UnkWord_e8bc(const UnkWord_e8bc& other) : value(other.value) {}
    UnkWord_e8bc& operator=(const UnkWord_e8bc& other) {
        value = other.value;
        return *this;
    }
};

struct UnkRecord100_e8bc {
    u64 field_0x0;
    u32 field_0x8;
    UnkString20_e8bc field_0x10;
    UnkWord_e8bc field_0x78;
    u64 field_0x80;
    u32 field_0x88;
    UnkString20_e8bc field_0x90;
    UnkWord_e8bc field_0xf8;
};
static_assert(sizeof(UnkRecord100_e8bc) == 0x100, "UnkRecord100_e8bc layout");

// As src/game/UnkTU2566/UnkTU2566_Minimap_ovb0201.cpp defines it.
struct UnkKeyed100Array_d;
void fn_7100ea9934(UnkKeyed100Array_d* self, u32 capacity);

struct UnkKeyed100Array_e8bc {
    UnkRecord100_e8bc* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;

    // The key's index, or the count when it isn't there.
    u32 IndexOf(u32 key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return i;
        return count;
    }
    // The key's record, or one past the last when it isn't there.
    UnkRecord100_e8bc* Find(u32 key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};

void fn_7100e8e8bc(u32 key, const UnkRecord100_e8bc* value) {
    UnkKeyed100Array_e8bc* array = reinterpret_cast<UnkKeyed100Array_e8bc*>(&g_unk_71729ce018);
    if (array->Find(key) == &array->values[array->count]) {
        if (array->count == array->capacity)
            fn_7100ea9934(reinterpret_cast<UnkKeyed100Array_d*>(array), array->step + array->count);
        array->keys[array->count] = key;
        new (&array->values[array->count]) UnkRecord100_e8bc(*value);
        array->count++;
    } else {
        array->values[array->IndexOf(key)] = *value;
    }
}
