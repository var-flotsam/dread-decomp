// Unit 351 (navmesh::CDynamicSmartLinkGroup): the reflected placement constructor of a 0x90-byte
// object (fn_7100267254, slot 0x28 of a type in src/reflection/navmesh_CDynamicSmartLinkGroup.cpp).
// The object is value-initialised: zeroed, then the base's constructor (fn_7100016ad8) runs, the
// vtable is stored, a file-path id is copied from the global empty one (+0x58), a growable array
// is default-constructed (+0x60, flags 0xf) and a flag cleared (+0x88); the object's virtual
// function in slot 3 is called at the end. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "game/navmesh/CDynamicSmartLinkGroup.h"
#include "types.h"

struct UnkSmartLinkRules_b58 {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::CFilePathStrId field_0x58;
    base::UnkVector<void*> field_0x60;
    bool field_0x88;
};
static_assert(sizeof(UnkSmartLinkRules_b58) == 0x90, "UnkSmartLinkRules_b58 layout");

void fn_7100267254(UnkSmartLinkRules_b58* self) {
    __builtin_memset(self, 0, sizeof(*self));
    fn_7100016ad8(self);
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a20cc0) + 0x10);
    new (&self->field_0x58) base::global::CFilePathStrId(g_unk_7101d470e8);
    new (&self->field_0x60) base::UnkVector<void*>(0xf);
    self->field_0x88 = false;
    reinterpret_cast<void (*)(UnkSmartLinkRules_b58*)>(self->vtable[3])(self);
}
