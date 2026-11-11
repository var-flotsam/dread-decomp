// behaviortree::CIf's unit (tools/tu_map.py, the name from reflection): a child of the node finished. The
// condition (+0x58) hands the node back to the tree (+0x10, fn_71003aab64); the branch picked by the
// condition's result (+0x70: 5 the one at +0x60, 6 the one at +0x68) clears two flags and reports 2 when
// +0x7e is clear, else 3 unless +0x7c is set (fn_71003aac0c). Names and layouts are placeholders.
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkSelectorTree_A2_b68;
struct UnkSelectorNode_A2_b68;
struct UnkInverterTarget_b66a2;
struct UnkInverter_b66a2;

struct UnkIfNode_A2_b70 {
    u8 pad_0x0[0x10];
    void* tree;
    u8 pad_0x18[0x40];
    void* condition;
    void* then_branch;
    void* else_branch;
    s32 result;
    u8 pad_0x74[8];
    bool field_0x7c;
    bool field_0x7d;
    bool field_0x7e;
    bool field_0x7f;
};

void fn_71003a7364(UnkIfNode_A2_b70* self, void* child) {
    if (self->condition == child) {
        fn_71003aab64(static_cast<UnkSelectorTree_A2_b68*>(self->tree),
                      reinterpret_cast<UnkSelectorNode_A2_b68*>(self));
        return;
    }
    void* branch;
    if (self->result == 5)
        branch = self->then_branch;
    else if (self->result == 6)
        branch = self->else_branch;
    else
        branch = nullptr;
    if (branch != child)
        return;
    bool repeat = self->field_0x7e;
    self->field_0x7f = false;
    self->field_0x7d = false;
    if (!repeat)
        fn_71003aac0c(static_cast<UnkInverterTarget_b66a2*>(self->tree), reinterpret_cast<UnkInverter_b66a2*>(self), 2);
    else if (!self->field_0x7c)
        fn_71003aac0c(static_cast<UnkInverterTarget_b66a2*>(self->tree), reinterpret_cast<UnkInverter_b66a2*>(self), 3);
}
