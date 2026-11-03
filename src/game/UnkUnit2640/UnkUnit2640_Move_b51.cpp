// Unit 2640 (tools/tu_map.py, low confidence; no reflected class): builds a record from `other`, taking
// over its array (pointer +0x8, size and capacity +0x10/+0x14, left empty in `other`) and copying the
// rest. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2640Record_b51 {
    u32 field_0x0;
    void* data;     // 0x8
    u32 size;       // 0x10
    u32 capacity;   // 0x14
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    u64 field_0x28;
    u8 tail[0x1d];  // 0x30
};

// The signature src/reflection/class_descriptors_02.cpp declares (a type's move slot); `src` is emptied.
void fn_7100ffc538(void* dst, const void* src) {
    UnkUnit2640Record_b51* self = static_cast<UnkUnit2640Record_b51*>(dst);
    UnkUnit2640Record_b51* other = static_cast<UnkUnit2640Record_b51*>(const_cast<void*>(src));
    self->field_0x0 = other->field_0x0;
    self->data = other->data;
    self->size = other->size;
    self->capacity = other->capacity;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    other->data = nullptr;
    other->size = 0;
    other->capacity = 0;
    __builtin_memcpy(self->tail, other->tail, sizeof(self->tail));
}
