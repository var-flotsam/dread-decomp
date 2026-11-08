// Functions in the CNavMeshItemComponent unit (tools/tu_map.py). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkComponent.h"

using base::reflection::UnkReflObject;

struct UnkSourceDesc;
struct UnkNavMeshItemOwner {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x1b0 - 0x18];
    UnkReflObject* item;  // 0x1b0, the object as a CCharClassNavMeshItemComponent
};

void fn_71009bf9a8(UnkNavMeshItemOwner* self, UnkSourceDesc** desc);
extern __attribute__((visibility("hidden"))) UnkSourceDesc* g_unk_717273ee10;

// Calls the base method, caches the object at +0x10 when it is a CCharClassNavMeshItemComponent,
// then passes the global descriptor to fn_71009bf9a8.
bool fn_71009bf928(UnkNavMeshItemOwner* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* object = self->object;
    self->item = object && fn_710009d50c(object->vf_0x0(), fn_71009bb690()) ? object : nullptr;
    fn_71009bf9a8(self, &g_unk_717273ee10);
    return result;
}
