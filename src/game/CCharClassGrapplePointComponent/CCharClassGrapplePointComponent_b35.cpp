// A constructor in the CCharClassGrapplePointComponent unit: the same shape as fn_7100816cb8 (the base
// constructor fn_71011acd10, the vtable, the flag at +0x58 set, an empty string id at +0x60) without
// the post-construction virtual call. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkGrapplePointDef {
    void** vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    u8 pad_0x59[0x7];
    UnkStrId field_0x60;
};

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
extern void* g_unk_7101a751e0[];

// Declared as src/game/CCharClassHangableGrappleSurfaceComponent/CCharClassHangableGrappleSurfaceComponent.cpp
// declares it.
void fn_7100816b28(char* raw) {
    UnkGrapplePointDef* self = reinterpret_cast<UnkGrapplePointDef*>(raw);
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = &g_unk_7101a751e0[2];
    self->field_0x58 = true;
    new (&self->field_0x60) UnkStrId("", true);
}
