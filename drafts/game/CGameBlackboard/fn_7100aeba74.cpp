// CGameBlackboard's unit (tools/tu_map.py): adding a section. CBlackboard's own add runs first
// (fn_71010489c4), then each property the section already holds is set in the section through
// fn_7100aeb78c. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// As src/game/CBlackboard/CBlackboard_Sections_ovb0026.cpp defines it.
class UnkBlackboardView_b0026;
void fn_71010489c4(UnkBlackboardView_b0026* self, const CStrId& key, CBlackboard::CSection* section);
// As src/game/CGameBlackboard/CGameBlackboard_Maps_ovb0064.cpp declares it.
struct UnkGameBlackboard_ovb0064;
void fn_7100aeb78c(UnkGameBlackboard_ovb0064* self, const CStrId& key, const CStrId& name);

// A section's property value, released by its destructor (fn_71000a1c60).
struct UnkPropertyValue_laeba74 : UnkReflValue {
    ~UnkPropertyValue_laeba74() { fn_71000a1c60(this); }
};

// An iterator over a section's properties: it walks the ids and the values together and hands out
// the current pair through an iterator value that refers to them.
struct UnkPropertyIter_laeba74 {
    UnkSectionData* data;
    base::UnkIterValue<UnkPropertyValue_laeba74, CStrId> current;

    explicit UnkPropertyIter_laeba74(UnkSectionData* data) : data(data) {
        current.SetRef(data->properties.ids, static_cast<UnkPropertyValue_laeba74*>(data->properties.values));
    }

    bool AtEnd() const { return current.ref.index == &data->properties.ids[data->properties.count]; }
    void operator++() {
        ++current.ref.index;
        ++current.ref.element;
    }

    // The value stored under `name`, or the end of the values.
    UnkReflValue* Find(const CStrId& name) const {
        UnkPropertyArrays& properties = data->properties;
        for (u32 i = 0; i < properties.count; ++i)
            if (properties.ids[i] == name)
                return &Values()[i];
        return End();
    }
    UnkReflValue* Values() const { return static_cast<UnkReflValue*>(data->properties.values); }
    UnkReflValue* End() const { return &Values()[data->properties.count]; }
};

void fn_7100aeba74(UnkGameBlackboard_ovb0064* self, const CStrId& key, CBlackboard::CSection* section) {
    fn_71010489c4(reinterpret_cast<UnkBlackboardView_b0026*>(self), key, section);
    UnkPropertyIter_laeba74 it(&section->data);
    while (!it.AtEnd()) {
        const CStrId& name = it.current.Index();
        UnkReflValue* value = it.Find(name);
        const char* type = nullptr;
        void* object = nullptr;
        if (value != it.End()) {
            type = value->type;
            if (type)
                object = value->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8
                             ? value->field_0x0
                             : value;
        }
        UnkTypedRef property;
        fn_71000a16e0(&property, type, object);
        fn_7100aeb78c(self, key, name);
        ++it;
    }
}
