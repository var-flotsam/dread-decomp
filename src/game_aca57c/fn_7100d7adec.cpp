// Calls one virtual function (vf_0x138 to vf_0x158) on each of an entity's components that is a
// CComponent (null entries skipped). Four slots.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

struct UnkComponent {
    virtual const CClass* vf_0x0() const;
};

struct UnkComponentOwner {
    u8 unk_0x0[0xa0];
    UnkComponent** components;
    void* field_0xa8;
    u32 count;
};

template <u32 Slot>
__attribute__((always_inline)) static inline void CallEach(UnkComponentOwner* self) {
    for (UnkComponent** it = self->components; it != self->components + self->count; ++it) {
        UnkComponent* c = *it;
        if (c && fn_710009d50c(c->vf_0x0(), fn_71010c23d0())) {
            void** vtable = *reinterpret_cast<void***>(c);
            reinterpret_cast<void (*)(UnkComponent*)>(vtable[Slot / 8])(c);
        }
    }
}

void fn_7100d7adec(UnkComponentOwner* self) { CallEach<0x148>(self); }
void fn_7100d7ae80(UnkComponentOwner* self) { CallEach<0x150>(self); }
void fn_7100d7af14(UnkComponentOwner* self) { CallEach<0x158>(self); }
void fn_7100d7b300(UnkComponentOwner* self) { CallEach<0x138>(self); }
