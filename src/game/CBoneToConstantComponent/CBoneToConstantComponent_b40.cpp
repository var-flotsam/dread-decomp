// CBoneToConstantComponent unit (tools/tu_map.py unit 1264): initialisation (fn_7100803ef4). Calls the
// base method, keeps the object at +0x10 when it is a CCharClassBoneToConstantComponent (null otherwise)
// and copies its settings (+0x58..+0x70) to +0x48..+0x60. Returns bit 0 of the base method's result.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkComponent.h"

using base::reflection::UnkReflObject;


struct UnkCharClassBoneToConstant_bc {
    u8 pad_0x0[0x58];
    u64 field_0x58;
    u64 field_0x60;
    u32 field_0x68;
    u32 field_0x6c;
};

struct UnkBoneToConstant_bc {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
    u8 pad_0x18[0x30];
    u64 field_0x48;
    u64 field_0x50;
    u32 field_0x58;
    u32 field_0x5c;
};

u32 fn_7100803ef4(UnkBoneToConstant_bc* self, void* arg) {
    // Read as a word here (the original keeps bit 0 of it), as a flag by the other callers.
    u32 result = reinterpret_cast<u32 (*)(UnkBaseComponent*, void*)>(fn_71010c41bc)(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* object = self->object;
    UnkReflObject* cast = object && fn_710009d50c(object->vf_0x0(), fn_7100804e24()) ? object : nullptr;
    self->object = cast;
    UnkCharClassBoneToConstant_bc* c = reinterpret_cast<UnkCharClassBoneToConstant_bc*>(cast);
    self->field_0x50 = c->field_0x60;
    self->field_0x48 = c->field_0x58;
    self->field_0x58 = c->field_0x68;
    self->field_0x5c = c->field_0x6c;
    return result & 1;
}
