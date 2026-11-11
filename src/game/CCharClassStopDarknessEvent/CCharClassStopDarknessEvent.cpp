// A function in the CCharClassStopDarknessEvent unit: fn_7100be54d4 does nothing for an empty id at +0x10;
// otherwise it passes the id and the value at +0x18 to every entry of the list fn_7100772c98 returns
// (fn_7100772698), each entry's object (+0x10) cast to CCentralUnitComponent (null when it isn't one).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1176.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkCentralUnitEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
};

struct UnkCentralUnitList {
    UnkCentralUnitEntry** data;
    u32 count;
};

void fn_7100772698(UnkReflObject* component, const CStrId* id, f32 value);

struct UnkStopDarknessEvent {
    u8 pad_0x0[0x10];
    CStrId id;      // 0x10
    f32 value;      // 0x18
};

void fn_7100be54d4(UnkStopDarknessEvent* self) {
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkReflObject* component = nullptr;
        if (*it) {
            if (UnkReflObject* o = (*it)->object)
                component = fn_710009d50c(o->vf_0x0(), fn_710076cf58()) ? o : nullptr;
        }
        fn_7100772698(component, &self->id, self->value);
    }
}
