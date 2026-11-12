// Unit 252 (0x71001bdeac-0x71001c071c, no reflected class): readies slot `index` of the object for a pass of the
// given size: creates the slot's holder (a 0x128-byte object with the identity matrix) and records it in the holder
// list (+0xf0) when it has none, makes the slot's pass object (fn_71001bef34) when it has none or checks that the
// existing one is big enough, flags the slot (and its entry in the list through fn_71001c04bc) and hands the pass's
// source and word to the holder. Always answers false. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR
#include "base/UnkVector.h"
#include "game/UnkTU221.h"
#include "game/UnkTU252.h"
#include "types.h"
#include "base/UnkMatrix44.h"

struct UnkPassOwner252;
struct UnkSlotArray252;
struct UnkSlot252;
struct UnkBillboardDrawable;
struct UnkStruct_710019ab0c;
struct UnkPass213Activate;
void fn_71001bef34(UnkPassOwner252* self, u32 index, u32 width, u32 height);
extern UnkMatrix44 g_unk_7101a1a680;

struct UnkSlotRef252 {
    void* holder;
    u8 flag;

    UnkSlotRef252() {}
    UnkSlotRef252(const UnkSlotRef252& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
    UnkSlotRef252(UnkSlotRef252&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};
static_assert(sizeof(UnkSlotRef252) == 16, "UnkSlotRef252 layout");

struct UnkPassSize252 {
    u8 pad_0x0[0x20];
    u32 width;   // 0x20
    u32 height;  // 0x24
    void* field_0x28;
    void* field_0x30;
};

struct UnkHolder252 {
    void* field_0x0;
    void* field_0x8;
};

struct UnkSlot252x {
    UnkPassSize252* pass;
    UnkHolder252* holder;
    u8 flag;
    u8 pad_0x11[7];
};

struct UnkSetupOwner252 {
    u8 pad_0x0[0xf0];
    base::UnkVector<UnkSlotRef252> refs;  // 0xf0
    u8 pad_0x118[0x28];
    UnkSlot252x slots[5];  // 0x140
};
static_assert(__builtin_offsetof(UnkSetupOwner252, slots) == 0x140, "UnkSetupOwner252 layout");

bool fn_71001bffc8(UnkSetupOwner252* self, u32 index, u32 a, u32 b, u32 c) {
    if (index > 4)
        return false;
    UnkSlot252x* slot = &self->slots[index];
    UnkPassSize252* pass = slot->pass;
    UnkHolder252* holder = slot->holder;
    if (!holder) {
        holder = static_cast<UnkHolder252*>(fn_71000839a0(0x128, 0x1e));
        fn_710019a4e8(reinterpret_cast<UnkBillboardDrawable*>(holder), 1, 1);
        slot->holder = holder;
        fn_710019b184(reinterpret_cast<UnkBillboardDrawable*>(holder), 0, 0, &g_unk_7101a1a680);
        UnkSlotRef252 ref;
        ref.holder = holder;
        ref.flag = 1;
        self->refs.PushBackCopy(ref);
    }
    u32 size = c * a;
    u32 height = b * 2;
    if (pass) {
        if (pass->width < size)
            return false;
        if (pass->height < height)
            return false;
    } else {
        fn_71001bef34(reinterpret_cast<UnkPassOwner252*>(self), index, size, height);
        pass = slot->pass;
    }
    self->slots[index].flag = 1;
    // keeps the slot test from folding into the range check at the top
    asm("" : "+r"(index));
    if (index <= 4 && self->refs.count > index) {
        UnkSlot252* element = fn_71001c04bc(reinterpret_cast<UnkSlotArray252*>(&self->refs), index);
        *reinterpret_cast<u8*>(reinterpret_cast<u8*>(element) + 8) = 1;
    }
    void* source = pass->field_0x28;
    void* word = pass->field_0x30;
    holder->field_0x8 = source;
    fn_710019ab0c(reinterpret_cast<UnkStruct_710019ab0c*>(holder));
    holder->field_0x0 = word;
    return false;
}
