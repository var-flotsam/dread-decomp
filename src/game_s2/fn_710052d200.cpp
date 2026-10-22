// Constructor of an object whose tail holds eight identical blocks, each a vtable and the same
// seven ids (the animation state names).
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
extern void* g_unk_7101a38c40[];
extern void* g_unk_7101a481e8[];

struct UnkStateIds {
    void** vtable;
    CStrId relax;
    CStrId walk;
    CStrId back;
    CStrId walkinit;
    CStrId backinit;
    CStrId walkend;
    CStrId backend;

    __attribute__((always_inline)) UnkStateIds()
        : vtable(g_unk_7101a38c40 + 2),
          relax("relax", true),
          walk("walk", true),
          back("back", true),
          walkinit("walkinit", true),
          backinit("backinit", true),
          walkend("walkend", true),
          backend("backend", true) {}
};

struct UnkObj_710052d200 {
    void** vtable;
    u8 pad_0x8[0x1c8];
    UnkStateIds states[8];
};

void fn_7100964670(UnkObj_710052d200* self);  // the base constructor

void fn_710052d200(UnkObj_710052d200* self) {
    fn_7100964670(self);
    self->vtable = g_unk_7101a481e8 + 2;
    for (UnkStateIds& s : self->states)
        new (&s) UnkStateIds();
}
