// Constructor and destructor of five groups of three arrays: each group first unlinks the objects of its first
// array (clearing their back-pointers) and empties all three, then frees them. fn_7100a9dbac and
// fn_7100a9dd78 are identical copies.
#include "base/global/CRntVector.h"
#include "game/UnkArray.h"
#include "game/UnkLinkGroup.h"
#include "game/UnkLinkGroups.h"
#include "game/UnkLinked.h"

static inline __attribute__((always_inline)) void Destroy(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 5; i-- > 0;)
        self->groups[i].~UnkLinkGroup();
}

void fn_71008ce35c(UnkLinkGroups* self) {
    Destroy(self);
}

void fn_7100a9dd78(UnkLinkGroups* self) {
    Destroy(self);
}

static inline __attribute__((always_inline)) void Construct(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 0; i < 5; i++)
        new (&self->groups[i]) UnkLinkGroup;
}

void fn_71008ce190(UnkLinkGroups* self) {
    Construct(self);
}

void fn_7100a9dbac(UnkLinkGroups* self) {
    Construct(self);
}
