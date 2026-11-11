// behaviortree::CSequence's unit (the name from reflection): the step after a child finished, the mirror of
// CSelector's (src/game/behaviortree/CSelector/CSelector_NextChild_A2_b68.cpp). Any state but 5 gives 6. In
// state 5 the sequence moves to the next child (+0x58); past the last one the result is 5; a null child gives 6;
// otherwise it is started on the tree (+0x10, fn_71003aab64), the state becomes 2 and the result is 4.
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkClone_fn_710039657c;
using UnkSequenceChild_A2_b69 = UnkCloneElem<UnkClone_fn_710039657c, 8>;
extern template UnkSequenceChild_A2_b69& base::UnkVector<UnkSequenceChild_A2_b69>::GrowAt(u32);

struct UnkSelectorTree_A2_b68;
struct UnkSelectorNode_A2_b68;

struct UnkSequence_A2_b69 {
    u8 pad_0x0[0x10];
    UnkSelectorTree_A2_b68* tree;
    u8 pad_0x18[0x18];
    base::UnkVector<UnkSequenceChild_A2_b69> children;
    s32 current;
    s32 state;
};

s32 fn_71003a9484(UnkSequence_A2_b69* self) {
    if (self->state == 5) {
        s32 index = ++self->current;
        if (index >= 0 && index < (s32)self->children.count) {
            UnkSelectorNode_A2_b68* child =
                *reinterpret_cast<UnkSelectorNode_A2_b68**>(self->children.GrowAt(index).data);
            if (child) {
                fn_71003aab64(self->tree, child);
                self->state = 2;
                return 4;
            }
        } else {
            return 5;
        }
    }
    return 6;
}
