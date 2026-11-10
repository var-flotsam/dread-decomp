// Unit 252 (0x71001bdeac-0x71001c071c, no reflected class): the slot `index` of the slot array (16-byte elements:
// a word and a flag), growing it with empty slots when it is past the end (base::UnkVector::GrowAt; the original
// shape of src/engine/UnkUnit252/UnkUnit252_SetSlotFlag.cpp's array). Names and layouts are placeholders.
#include "types.h"

#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"

struct UnkSlotArray252;
struct UnkSlot252;

struct UnkSlotElem252 {
    void* field_0x0;
    u8 flag;  // 0x8

    UnkSlotElem252() {
        __builtin_memset(&field_0x0, 0, sizeof(field_0x0));
        flag = 0;
    }
    UnkSlotElem252(UnkSlotElem252&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};

UnkSlot252* fn_71001c04bc(UnkSlotArray252* array, u32 index) {
    return reinterpret_cast<UnkSlot252*>(&reinterpret_cast<base::UnkVector<UnkSlotElem252>*>(array)->GrowAt(index));
}
