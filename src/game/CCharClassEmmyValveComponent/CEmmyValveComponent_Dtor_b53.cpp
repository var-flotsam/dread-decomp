// CCharClassEmmyValveComponent's unit (tools/tu_map.py; also CEmmyValveComponent): the valve
// component's complete destructor, the shape of fn_71007de318
// (src/game/CQuarentineDoorComponent/CQuarentineDoorComponent_Dtor_b53.cpp): the vtable, the member at
// +0x258 (fn_71007b4578), three value holders (+0xf0, +0x160, +0x1d0, as base::refl_ValueHolderDtor),
// the zeroed member at +0x40, then the component base's destructor (fn_71010c4194). The deleting
// destructor (fn_71007b4028) inlines the member's destructor (as fn_71007b4578 in
// CCharClassEmmyValveComponent_b44.cpp) and frees the component. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

extern char g_unk_7101a70560[];
extern char g_unk_7101a70788[];
extern char g_unk_7101a707e0[];

void fn_71007b4578(void* member);

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkValveRecord_b53 {
    u64 field_0x0;
    UnkStringData* field_0x8;  // CStrId
    UnkStrIdTable field_0x10;
};

// fn_71007b4578's body, inlined into the deleting destructor.
static inline __attribute__((always_inline)) void DestroyValveRecord_b53(UnkValveRecord_b53* self) {
    fn_71004f11f8(&self->field_0x10);
    if (self->field_0x10.keys)
        self->field_0x10.allocator->vf_0x48(self->field_0x10.keys);
    if (self->field_0x10.values)
        self->field_0x10.allocator->vf_0x48(self->field_0x10.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0x8;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
}

struct UnkEmmyValveHolderTag_b53;
using UnkEmmyValveHolder_b53 = base::UnkValueHolder<UnkEmmyValveHolderTag_b53>;

// As base::refl_ValueHolderDtor.
static inline __attribute__((always_inline)) void DestroyHolder_b53(UnkEmmyValveHolder_b53* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = g_unk_7101a70788 + 0x10;
    fn_71000a1c60(&self->value);
}

struct UnkEmmyValveZeroed_b53 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkEmmyValveComponent_b53 {
    char* vtable;
    u8 pad_0x8[0x38];
    UnkEmmyValveZeroed_b53 zeroed;  // 0x40
    u8 pad_0x68[0x88];
    UnkEmmyValveHolder_b53 holders[3];  // 0xf0
    u8 pad_0x240[0x18];
    u8 field_0x258[0x40];
};

template <bool Inline>
static inline __attribute__((always_inline)) void DestroyEmmyValve_b53(UnkEmmyValveComponent_b53* self) {
    self->vtable = g_unk_7101a70560 + 0x10;
    if (Inline)
        DestroyValveRecord_b53(reinterpret_cast<UnkValveRecord_b53*>(self->field_0x258));
    else
        fn_71007b4578(self->field_0x258);
    DestroyHolder_b53(&self->holders[2]);
    DestroyHolder_b53(&self->holders[1]);
    DestroyHolder_b53(&self->holders[0]);
    self->zeroed.vtable = g_unk_7101a707e0 + 0x10;
    self->zeroed.field_0x10 = 0;
    self->zeroed.field_0x18 = 0;
    self->zeroed.field_0x20 = 0;
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_71007b3f68(UnkEmmyValveComponent_b53* self) { DestroyEmmyValve_b53<false>(self); }

void fn_71007b4028(UnkEmmyValveComponent_b53* self) {
    DestroyEmmyValve_b53<true>(self);
    fn_7100083c00(self);
}
