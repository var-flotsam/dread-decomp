// tools/tu_map.py unit 42 (no class hint; its initialiser registers EEventProps and
// TEventPropsFlags): a table of 8-byte keys with a parallel float array, the value of a key by its
// position, and a cursor's value of the key before its position (0 at the start or with none).
// Names and layouts are placeholders.
#include "types.h"

struct UnkKeyTable42 {
    u64* keys;    // 0x0
    f32* values;  // 0x8
};

struct UnkKeyCursor42 {
    UnkKeyTable42* table;  // 0x0
    u8 pad_0x8[0x10];
    u64* pos;  // 0x18
};

__attribute__((noinline)) f32 fn_7100069a20(UnkKeyTable42* self, u64* key) {
    u32 index = static_cast<u32>(key - self->keys);
    return self->values[index];
}

f32 fn_71000686b8(UnkKeyCursor42* self) {
    u64* pos = self->pos;
    if (pos == nullptr || pos == self->table->keys) {
        return 0.0f;
    }
    return fn_7100069a20(self->table, pos - 1);
}
