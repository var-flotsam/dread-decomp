// Adds an object to a string-id hash map held at +8, keyed by the id the object reports through
// vtable slot 3, as the generated fn_7100121008 (base::refl_StrIdHashInsert) does for an id and an
// 8-byte value: fn_71000875d0.
#include "base/UnkCloneHelpers.h"

struct UnkNamedObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const base::global::CStrId* vf_0x18();  // the object's name
};

struct UnkObjectsByName;

struct UnkNamedObjectMap {
    void* field_0x0;
    base::UnkStrIdHashMap<UnkObjectsByName> map;
};

__attribute__((flatten)) void fn_71000875d0(UnkNamedObjectMap* self, UnkNamedObject* object) {
    u64 value = reinterpret_cast<u64>(object);
    base::refl_StrIdHashInsert<UnkObjectsByName>(&self->map, object->vf_0x18(), &value);
}
