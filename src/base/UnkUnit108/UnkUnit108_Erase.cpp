// Unit 108 of tools/tu_map.py (0x7100110c54-0x71001239b0): erasing an entry (12 bytes) from an
// array by index, the later entries moved down one, directly (fn_7100119f3c) and as two reflected
// setters that read the index out of a value holder (in place, or behind a pointer when the holder
// is in mode 1 or its type is wider than 8 bytes) and answer a 0x18-byte status {0, 0, 2, 1}.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry108 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkEntryArray108 {
    UnkEntry108* data;
    u32 count;  // 0x8
};

struct UnkValueType108 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

struct UnkValue108 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType108* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};

struct UnkArrayOwner108 {
    u8 pad_0x0[0x8];
    UnkEntryArray108* array;  // 0x8
};

struct UnkStatus108 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

static inline void Erase(UnkEntryArray108* self, u32 index) {
    if (index < self->count) {
        for (u64 i = index; i < self->count - 1; ++i)
            self->data[i] = self->data[i + 1];
        --self->count;
    }
}

void fn_7100119f3c(UnkEntryArray108* self, u32 index) {
    Erase(self, index);
}

static inline UnkStatus108 EraseByValue(UnkArrayOwner108* owner, UnkValue108* const* value) {
    UnkValue108* v = *value;
    UnkEntryArray108* array = owner->array;
    const u8* data = (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
    Erase(array, *reinterpret_cast<const u32*>(data));
    UnkStatus108 status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

UnkStatus108 fn_710011e290(void* context, UnkArrayOwner108* owner, UnkValue108* const* value) {
    return EraseByValue(owner, value);
}

UnkStatus108 fn_710011e324(void* context, UnkArrayOwner108* owner, UnkValue108* const* value) {
    return EraseByValue(owner, value);
}

