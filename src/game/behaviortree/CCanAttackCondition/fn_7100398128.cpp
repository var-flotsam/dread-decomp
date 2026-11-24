// Constructor of a node (as in fn_7100374984, src/game/behaviortree/CSetClockwiseDirBlcValueAction/fn_7100374984.cpp) whose name is a string:
// runs its base constructor (fn_71003944e8), sets the vtable, initialises its fields, then runs
// the post-construction virtual call.
#include <new>

#include "base/global/CRntString.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CRntString;

struct UnkNode_7100398128 : UnkBehaviorTreeNode {
    CRntString sAttack;
    bool bCheckRotation;
    bool bIgnoreDistance;
    bool bUseSuitableAttack;
};

extern void* g_unk_7101a27298[];

void fn_7100398128(UnkNode_7100398128* self) {
    fn_71003944e8(self);
    self->vtable = &g_unk_7101a27298[2];
    new (&self->sAttack) CRntString("");
    self->bCheckRotation = true;
    self->bIgnoreDistance = false;
    self->bUseSuitableAttack = false;
    reinterpret_cast<void (*)(UnkNode_7100398128*)>(self->vtable[3])(self);
}
