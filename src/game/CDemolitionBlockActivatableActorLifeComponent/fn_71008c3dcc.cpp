// CDemolitionBlockActivatableActorLifeComponent unit: on the first pass (state 1) takes the handle built from the
// controller at +0x470 into the slot at +0x4a0, then hands the slot's target and two string ids of the
// object at +0x4a8 to fn_710082e5ec. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "types.h"
#include <new>
#include "game/UnkTU437.h"

using base::global::CStrId;

using UnkRef = base::UnkRefPtrMember;
void* fn_71008c3f34(void* object);
void fn_710082e5ec(void* object, CStrId* a, CStrId* b);

struct UnkInfo_l8c3dcc {
    u8 pad_0x0[0x150];
    CStrId first;
    CStrId second;
};

struct UnkActivatable_l8c3dcc {
    u8 pad_0x0[0x470];
    u8 field_0x470[0x28];
    u32 state;
    u8 pad_0x49c[4];
    base::UnkRefHandle target;
    UnkInfo_l8c3dcc* info;
};

void fn_71008c3dcc(UnkActivatable_l8c3dcc* self) {
    alignas(8) u8 sh[8];
    alignas(8) u8 sa[8];
    alignas(8) u8 sb[8];
    if (self->state == 1) {
        base::UnkRefPtrMember* handle = new (sh) base::UnkRefPtrMember(fn_7100353be8(self->field_0x470));
        fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(handle));
        handle->~UnkRefPtrMember();
        self->state = 2;
    }
    base::UnkRefCounted* target = self->target.ptr;
    if (target && *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)) {
        // reads the target's object pointer again, as the original does
        asm("" : "+r"(target));
        void* object = fn_71008c3f34(*reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10));
        if (object) {
            CStrId* a = new (sa) CStrId(self->info->first);
            CStrId* b = new (sb) CStrId(self->info->second);
            fn_710082e5ec(object, a, b);
            b->~CStrId();
            a->~CStrId();
        }
    }
}
