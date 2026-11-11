// A function in the CCharClassStartDarknessEvent unit (the shape of fn_7100be54d4 in
// game/CCharClassStopDarknessEvent): fn_7100be27a0 does nothing for an empty id at +0x10; otherwise it
// passes the id and the two values at +0x18 to every entry of the list fn_7100772c98 returns
// (fn_7100772610), each entry's object (+0x10) cast to CCentralUnitComponent (null when it isn't one).
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

void fn_7100772610(UnkReflObject* component, const CStrId* id, f32 a, f32 b);

struct UnkStartDarknessEvent {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
    f32 a;      // 0x18
    f32 b;      // 0x1c
};

void fn_7100be27a0(UnkStartDarknessEvent* self) {
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkReflObject* component = nullptr;
        if (*it) {
            if (UnkReflObject* o = (*it)->object)
                component = fn_710009d50c(o->vf_0x0(), fn_710076cf58()) ? o : nullptr;
        }
        fn_7100772610(component, &self->id, self->a, self->b);
    }
}
