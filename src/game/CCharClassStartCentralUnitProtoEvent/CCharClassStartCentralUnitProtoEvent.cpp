// A function in the CCharClassStartCentralUnitProtoEvent unit: fn_7100be1e0c, given a context that holds an
// object, starts (fn_710077561c) every central unit (the list fn_7100772c98 returns, each entry's object at
// +0x10) whose definition (+0x18) is named "PRP_CV_CentralUnitProto" (its id at +0x30). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1176.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkCentralUnitProtoDef {
    u8 pad_0x0[0x30];
    UnkStringData* name;
};

struct UnkCentralUnitProto {
    u8 pad_0x0[0x18];
    UnkCentralUnitProtoDef* def;
};

struct UnkCentralUnitProtoEntry {
    u8 pad_0x0[0x10];
    UnkCentralUnitProto* object;
};

struct UnkCentralUnitList {
    UnkCentralUnitProtoEntry** data;
    u32 count;
};

void fn_7100be1e0c(void* self, void* const* context) {
    if (!*context)
        return;
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitProtoEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkCentralUnitProto* unit = *it ? (*it)->object : nullptr;
        CStrId id("PRP_CV_CentralUnitProto", true);
        if (unit && unit->def->name == id.data)
            fn_710077561c(unit);
    }
}
