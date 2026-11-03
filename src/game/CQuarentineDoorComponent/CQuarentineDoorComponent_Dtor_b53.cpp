// CQuarentineDoorComponent's unit (tools/tu_map.py, the name from reflection): the component's
// complete (fn_71007de318) and deleting (fn_71007de424) destructors. The vtable, three value holders
// (+0x150, +0x1c0, +0x230; each clears its pairs, restores its vtable and destroys its reflected value,
// as base::refl_ValueHolderDtor), the zeroed member at +0xa0 (as base::refl_ZeroedCtor builds it),
// then the activatable component's destructor (fn_71007d70d8). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern char g_unk_7101a73188[];
extern char g_unk_7101a73370[];
extern char g_unk_7101a733c8[];

template <u32 IdOffset>
struct UnkActivatable_b44a2;
// Signature as src/game/CEvacuationCountDown/CEvacuationCountDown_Dtor_b49a3.cpp declares it.
void fn_71007d70d8(UnkActivatable_b44a2<0x48>* self);

struct UnkQuarentineHolderTag_b53;
using UnkQuarentineHolder_b53 = base::UnkValueHolder<UnkQuarentineHolderTag_b53>;

// As base::refl_ValueHolderDtor.
static inline __attribute__((always_inline)) void DestroyHolder_b53(UnkQuarentineHolder_b53* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = g_unk_7101a73370 + 0x10;
    fn_71000a1c60(&self->value);
}

struct UnkQuarentineZeroed_b53 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkQuarentineZeroed_b53() {
        vtable = g_unk_7101a733c8 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkQuarentineDoorComponent_b53 {
    char* vtable;
    u8 pad_0x8[0x98];
    UnkQuarentineZeroed_b53 zeroed;  // 0xa0
    u8 pad_0xc8[0x88];
    UnkQuarentineHolder_b53 holders[3];  // 0x150
};

static inline __attribute__((always_inline)) void DestroyQuarentineDoor_b53(UnkQuarentineDoorComponent_b53* self) {
    self->vtable = g_unk_7101a73188 + 0x10;
    DestroyHolder_b53(&self->holders[2]);
    DestroyHolder_b53(&self->holders[1]);
    DestroyHolder_b53(&self->holders[0]);
    self->zeroed.~UnkQuarentineZeroed_b53();
    fn_71007d70d8(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}

void fn_71007de318(UnkQuarentineDoorComponent_b53* self) { DestroyQuarentineDoor_b53(self); }

void fn_71007de424(UnkQuarentineDoorComponent_b53* self) {
    DestroyQuarentineDoor_b53(self);
    fn_7100083c00(self);
}
