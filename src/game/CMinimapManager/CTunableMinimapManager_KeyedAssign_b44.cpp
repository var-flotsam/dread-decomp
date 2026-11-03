// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): two functions that assign a value to
// the entry of a keyed array (0x118-byte values, u32 keys) whose key matches, if any (each with its own value type). Shaped like fn_7100e545b4 in
// src/clones/ with integer keys. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100eac64c(void* dst, const void* src);
void fn_7100eac82c(void* dst, const void* src);

template <void (*Assign)(void*, const void*)>
struct UnkMinimapKeyedValue_b44a3 {
    u64 words[0x118 / 8];

    UnkMinimapKeyedValue_b44a3& operator=(const UnkMinimapKeyedValue_b44a3& other) {
        Assign(this, &other);
        return *this;
    }
};

template <class V>
struct UnkMinimapKeyedArray_b44a3 {
    V* values;
    u32* keys;
    u32 count;

    V* End() { return &values[count]; }
    V* Find(const u32& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

// As in refl_KeyedFindAssign, the assignment is not a tail call; disable_tail_calls stands in.
template <class V>
__attribute__((disable_tail_calls, always_inline)) inline void KeyedAssign_b44a3(UnkMinimapKeyedArray_b44a3<V>* self,
                                                                                 const u32* key, const V* value) {
    V* v = self->Find(*key);
    if (v != self->End())
        *v = *value;
}

using UnkMinimapValueA_b44a3 = UnkMinimapKeyedValue_b44a3<fn_7100eac64c>;
using UnkMinimapValueB_b44a3 = UnkMinimapKeyedValue_b44a3<fn_7100eac82c>;

__attribute__((disable_tail_calls)) void fn_7100eaddf4(UnkMinimapKeyedArray_b44a3<UnkMinimapValueA_b44a3>* self,
                                                       const u32* key, const UnkMinimapValueA_b44a3* value) {
    KeyedAssign_b44a3(self, key, value);
}

__attribute__((disable_tail_calls)) void fn_7100eade84(UnkMinimapKeyedArray_b44a3<UnkMinimapValueB_b44a3>* self,
                                                       const u32* key, const UnkMinimapValueB_b44a3* value) {
    KeyedAssign_b44a3(self, key, value);
}
