// behaviortree::CSelector's unit (unit 546 of tools/tu_map.py, the name from reflection): the step after a child
// finished. State 5 is kept; any state but 6 gives 6. In state 6 the selector moves to the next child (+0x58);
// while one exists (children array at +0x30, fetched through its GrowAt, fn_710039657c) it is started on the tree
// (+0x10, fn_71003aab64), the state becomes 2 and the result is 4; otherwise 6. Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkClone_fn_710039657c;
using UnkSelectorChild_A2_b68 = UnkCloneElem<UnkClone_fn_710039657c, 8>;
extern template UnkSelectorChild_A2_b68& base::UnkVector<UnkSelectorChild_A2_b68>::GrowAt(u32);

struct UnkSelectorTree_A2_b68;
struct UnkSelectorNode_A2_b68;

void fn_71003aab64(UnkSelectorTree_A2_b68* tree, UnkSelectorNode_A2_b68* child);

struct UnkSelector_A2_b68 {
    u8 pad_0x0[0x10];
    UnkSelectorTree_A2_b68* tree;
    u8 pad_0x18[0x18];
    base::UnkVector<UnkSelectorChild_A2_b68> children;
    s32 current;
    s32 state;
};

s32 fn_71003a8f08(UnkSelector_A2_b68* self) {
    s32 state = self->state;
    if (state == 5)
        return state;
    if (state == 6) {
        s32 index = ++self->current;
        if (index >= 0 && index < (s32)self->children.count) {
            UnkSelectorNode_A2_b68* child =
                *reinterpret_cast<UnkSelectorNode_A2_b68**>(self->children.GrowAt(index).data);
            if (child) {
                fn_71003aab64(self->tree, child);
                self->state = 2;
                return 4;
            }
        }
    }
    return 6;
}
