// Unit 2594 of tools/tu_map.py (no name hint): closes the slot whose key matches (counted at +0x30), clears
// the key's link and tells each of the slot's parts (vtable slot 0x98). Names and layouts are placeholders.
#include "types.h"

struct UnkSlotObj_b256 {
    virtual void vf_0x0();
    virtual void vf_0x8();  // deletes
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
};

struct UnkSlotKey_b256 {
    u8 pad_0x0[8];
    void* link;  // 0x8
};

struct UnkSlot_b256 {
    union {
        UnkSlotKey_b256* key;  // 0x0
        UnkSlotObj_b256* first;
    };
    UnkSlotObj_b256* parts[8];  // 0x8
    u32 field_0x48;
    u8 closed;  // 0x4c
};

struct UnkSlotTable_b256 {
    u8 pad_0x0[8];
    UnkSlot_b256** slots;  // 0x8
    u32 count;             // 0x10
    u8 pad_0x14[0x1c];
    u32 closed_count;  // 0x30
};

bool fn_7100f12a08(UnkSlotTable_b256* self, void* key) {
    UnkSlot_b256* slot = nullptr;
    UnkSlot_b256** p = self->slots;
    for (UnkSlot_b256** end = p + self->count; p != end; ++p) {
        if ((*p)->key == key) {
            slot = *p;
            break;
        }
    }
    if (!slot || slot->closed)
        return false;
    ++self->closed_count;
    UnkSlotKey_b256* k = slot->key;
    slot->closed = 1;
    slot->field_0x48 = 0;
    __builtin_memset(&k->link, 0, 8);
    slot->key->link = nullptr;
    for (int i = 0; i < 8; ++i)
        slot->parts[i]->vf_0x98();
    return true;
}
