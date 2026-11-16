// Unit 1648 (CBlackboard's sections): copies a section's data, a fresh set of property arrays
// sized like the other's, then every (id, value) property of the other appended in turn.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

// A property's value: a reflected value that owns what it holds.
struct UnkPropertyValue_af0030 : UnkReflValue {
    UnkPropertyValue_af0030(const UnkPropertyValue_af0030& other) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = other.kind;
        const void* value;
        if (!other.type)
            value = nullptr;
        else if (other.field_0x14 == 1 || *reinterpret_cast<const u64*>(other.type + 0x18) > 8)
            value = other.field_0x0;
        else
            value = &other;
        fn_71000a18d4(this, reinterpret_cast<const CType*>(other.type), value, false);
    }
    ~UnkPropertyValue_af0030() { fn_71000a1c60(this); }
};

// Appends a property, growing the arrays by their step when they are full.
inline void Append_af0030(UnkPropertyArrays* props, const CStrId& id, const UnkPropertyValue_af0030& value) {
    if (props->count == props->capacity)
        fn_7100093dd0(props, props->step + props->count);
    new (&props->ids[props->count]) CStrId(id);
    new (&static_cast<UnkPropertyValue_af0030*>(props->values)[props->count]) UnkPropertyValue_af0030(value);
    props->count++;
}

void fn_7100af0030(UnkSectionData* self, const UnkSectionData* other) {
    self->reserve = 200;
    u32 flags = other->flags;
    u32 capacity = other->properties.capacity;
    UnkPropertyArrays& props = self->properties;
    props.values = nullptr;
    props.ids = nullptr;
    props.count = 0;
    props.capacity = 0;
    props.step = 10;
    props.flags = flags;
    props.allocator = fn_7100083990();
    fn_7100093dd0(&props, capacity);
    self->flags = other->flags;

    base::UnkIterValue<UnkPropertyValue_af0030, CStrId> it;
    it.SetRef(other->properties.ids, static_cast<UnkPropertyValue_af0030*>(other->properties.values));
    while (it.ref.index != other->properties.ids + other->properties.count) {
        const CStrId& id = it.Index();
        const UnkPropertyValue_af0030& value = it.Element();
        Append_af0030(&self->properties, id, value);
        ++it.ref.index;
        ++it.ref.element;
    }
}
