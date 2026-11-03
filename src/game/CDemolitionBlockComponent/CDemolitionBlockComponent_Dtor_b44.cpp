// The destructor of the activatable component (its vtable g_unk_7101a7ac20, as the constructor in
// UnkDemolitionBlockCtor.cpp stores it; this copy is in the CDemolitionBlockComponent unit) and its
// copies in other units with their own vtables (CEmmyWakeUpComponent, CCharClassLavaPoolComponent
// with its own base destructor fn_7100900414, CMagnetSurfaceHuskComponent): the string-id table at
// +0x50 (base/UnkStrIdTable.h; +0xcd8 and +0x68 in two copies) is reset and its arrays freed, the
// string id before it (+0x48) released,
// then the base destructor runs (fn_71010c4194, the component's). Names and layout are
// placeholders.
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a7ac20[];  // the activatable component's vtable
extern void* g_unk_7101a72d70[];  // CEmmyWakeUpComponent unit
extern void* g_unk_7101a80778[];  // CCharClassLavaPoolComponent unit
extern void* g_unk_7101a831e0[];  // CMagnetSurfaceHuskComponent unit

void fn_7100900414(UnkBaseComponent* self);

// The id at `IdOffset` (0x48 in the activatable component), the table right after it.
template <u32 IdOffset>
struct UnkActivatable_b44a2 : UnkBaseComponent {
    u8 pad_0x40[IdOffset - 0x40];
    UnkStringData* field_0x48;  // CStrId
    UnkStrIdTable field_0x50;
};

template <void** Vtable, void (*Base)(UnkBaseComponent*), u32 IdOffset>
__attribute__((always_inline)) inline void UnkActivatableDtor_b44a2(UnkActivatable_b44a2<IdOffset>* self) {
    self->vtable = Vtable + 2;
    fn_71004f11f8(&self->field_0x50);
    if (self->field_0x50.keys)
        self->field_0x50.allocator->vf_0x48(self->field_0x50.keys);
    if (self->field_0x50.values)
        self->field_0x50.allocator->vf_0x48(self->field_0x50.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0x48;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    Base(self);
}

void fn_71007b00cc(UnkActivatable_b44a2<0x48>* self) {
    UnkActivatableDtor_b44a2<g_unk_7101a7ac20, fn_71010c4194>(self);
}
void fn_71007d70d8(UnkActivatable_b44a2<0x48>* self) {
    UnkActivatableDtor_b44a2<g_unk_7101a72d70, fn_71010c4194>(self);
}
void fn_71008fbc64(UnkActivatable_b44a2<0xcd0>* self) {
    UnkActivatableDtor_b44a2<g_unk_7101a80778, fn_7100900414>(self);
}
void fn_7100924e78(UnkActivatable_b44a2<0x60>* self) {
    UnkActivatableDtor_b44a2<g_unk_7101a831e0, fn_71010c4194>(self);
}
