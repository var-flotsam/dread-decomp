// CGoliathAttack (reflection: size 0x70): its copy constructor (fn_710058f3d4) and move
// constructor (fn_710058f4cc), both the implicit copy.
#include "game/CAttack.h"
#include "base/global/CStrId.h"
#include "game/CGoliathAttack.h"

#include <new>

void fn_710058f3d4(CGoliathAttack* self, const CGoliathAttack* other) {
    new (self) CGoliathAttack(*other);
}

void fn_710058f4cc(CGoliathAttack* self, const CGoliathAttack* other) {
    new (self) CGoliathAttack(*other);
}
