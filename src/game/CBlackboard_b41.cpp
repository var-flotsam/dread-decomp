// CBlackboard unit (tools/tu_map.py, the name from reflection): the reflected CreateSection(sKey)
// (game/CBlackboard.h): builds a section under the string-id argument as fn_71010472e0 does, without
// looking for an existing one first, adds it through vf_0x28 and returns it as a reflected
// CBlackboard::CSection pointer (fn_7101045d7c). Names and layouts are placeholders.
#include <new>

#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

const base::reflection::CType* fn_7101045d7c();

UnkReflValue fn_7101049fe0(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    CBlackboard* self = *reinterpret_cast<CBlackboard**>(obj + 8);
    const CStrId* key;
    if (!a->type)
        key = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        key = static_cast<const CStrId*>(a->field_0x0);
    else
        key = reinterpret_cast<const CStrId*>(a);
    CBlackboard::CSection* section = static_cast<CBlackboard::CSection*>(fn_71000839a0(sizeof(CBlackboard::CSection), 0x15));
    new (&section->key) CStrId(*key);
    section->data.reserve = 200;
    section->blackboard = self;
    section->field_0x10 = nullptr;
    section->data.properties.values = nullptr;
    section->data.properties.ids = nullptr;
    section->data.properties.count = 0;
    section->data.properties.capacity = 0;
    section->data.properties.step = 10;
    section->data.properties.flags = 0x15;
    section->data.properties.allocator = fn_7100083990();
    fn_7100093dd0(&section->data.properties, 200);
    section->data.flags = 0x15;
    section->data.field_0x38 = false;
    self->vf_0x28(*key, section);
    CBlackboard::CSection* result = section;
    const base::reflection::CType* type = fn_7101045d7c();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &result, false);
    return r;
}
