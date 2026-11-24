// Copies of fn_71008ce35c, fn_71008ce190 (src/game_s4/fn_71008ce35c.cpp), which the binary
// repeats at other addresses: fn_710027514c, fn_710027497c.
// Constructor and destructor of five groups of three arrays: each group first unlinks the objects of its first
// array (clearing their back-pointers) and empties all three, then frees them. fn_7100a9dbac and
// fn_7100a9dd78 are identical copies.
#include "base/global/CRntVector.h"
#include "game/CUsableComponent.h"
#include "game/UnkArray.h"
#include "game/UnkLinkGroup.h"
#include "game/UnkLinkGroups.h"
#include "game/UnkLinked.h"

static inline __attribute__((always_inline)) void Destroy(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 5; i-- > 0;)
        self->groups[i].~UnkLinkGroup();
}

void CNavMesh_DestroyLinkGroups(UnkLinkGroups* self) {
    Destroy(self);
}

static inline __attribute__((always_inline)) void Construct(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 0; i < 5; i++)
        new (&self->groups[i]) UnkLinkGroup;
}

void CNavMesh_ConstructLinkGroups(UnkLinkGroups* self) {
    Construct(self);
}
