// behaviortree::CIf's unit (tools/tu_map.py, the name from reflection): the node's update, its abort and
// a child's status report. The condition (+0x58) runs first; its result (+0x70: 5 the branch at +0x60,
// 6 the one at +0x68) picks the branch to run. The tree (+0x10) starts a child (fn_71003aab64), drops a
// pending one (fn_71003aad48) and is told when the node changes state (fn_71003aac0c). Names and layouts
// are placeholders.
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkSelectorTree_A2_b68;
struct UnkSelectorNode_A2_b68;
struct UnkInverterTarget_b66a2;
struct UnkInverter_b66a2;
struct UnkStruct_71003aad48;
bool fn_71003aad48(UnkStruct_71003aad48* self, void* key);

struct UnkIfNode_ovb0088 {
    u8 pad_0x0[0x10];
    void* tree;
    u8 pad_0x18[0x40];
    void* condition;    // 0x58
    void* then_branch;  // 0x60
    void* else_branch;  // 0x68
    s32 result;         // 0x70
    s32 last_result;    // 0x74
    s32 branch_status;  // 0x78
    bool condition_running;  // 0x7c
    bool branch_running;     // 0x7d
    bool aborting;           // 0x7e
    bool branch_dropped;     // 0x7f
    bool condition_dropped;  // 0x80
    bool repeat;             // 0x81

    void* Branch() {
        if (result == 5)
            return then_branch;
        if (result == 6)
            return else_branch;
        return nullptr;
    }
    void Start(void* child) {
        fn_71003aab64(static_cast<UnkSelectorTree_A2_b68*>(tree), static_cast<UnkSelectorNode_A2_b68*>(child));
    }
    void Notify(s32 status) {
        fn_71003aac0c(static_cast<UnkInverterTarget_b66a2*>(tree), reinterpret_cast<UnkInverter_b66a2*>(this),
                      status);
    }
};

s32 fn_71003a71b4(UnkIfNode_ovb0088* self) {
    self->aborting = true;
    if (!self->branch_dropped && !self->condition_dropped) {
        if (self->condition_running) {
            UnkStruct_71003aad48* tree = static_cast<UnkStruct_71003aad48*>(self->tree);
            self->condition_running = !fn_71003aad48(tree, self->condition);
        }
        if (self->branch_running) {
            UnkStruct_71003aad48* tree = static_cast<UnkStruct_71003aad48*>(self->tree);
            self->branch_running = !fn_71003aad48(tree, self->Branch());
        }
    }
    if (self->branch_running || self->condition_running)
        return 4;
    return 7;
}

void fn_71003a727c(UnkIfNode_ovb0088* self, void* child, s32 status) {
    if (self->condition == child) {
        self->last_result = status;
        self->condition_running = false;
        if (!self->aborting) {
            if (self->result == 1)
                self->result = status;
            else if (self->result == status)
                self->repeat = true;
            self->Notify(2);
        } else if (!self->branch_running) {
            self->Notify(3);
        }
        return;
    }
    if (self->Branch() != child)
        return;
    self->branch_status = status;
    if (status == 2)
        return;
    bool aborting = self->aborting;
    self->branch_dropped = false;
    self->branch_running = false;
    if (!aborting)
        self->Notify(2);
    else if (!self->condition_running)
        self->Notify(3);
}

s32 fn_71003a6ff0(UnkIfNode_ovb0088* self) {
    if (self->repeat) {
        self->repeat = false;
        return 2;
    }
    if (self->branch_running || self->branch_status == 1) {
        s32 result = self->result;
        if (result != 1 && self->last_result != result && !self->condition_running) {
            if (self->branch_running) {
                UnkStruct_71003aad48* tree = static_cast<UnkStruct_71003aad48*>(self->tree);
                if (!fn_71003aad48(tree, self->Branch())) {
                    self->branch_dropped = true;
                    return 4;
                }
            }
            return 6;
        }
        if (!self->branch_running) {
            void* branch = self->Branch();
            if (branch) {
                if (self->branch_status == 1) {
                    self->Start(branch);
                    self->branch_running = true;
                    return 2;
                }
                if (!self->condition_running)
                    return self->branch_status;
                UnkStruct_71003aad48* tree = static_cast<UnkStruct_71003aad48*>(self->tree);
                if (fn_71003aad48(tree, self->condition))
                    return self->branch_status;
                return 4;
            }
        }
        if (self->condition_running)
            return 6;
        UnkSelectorTree_A2_b68* tree = static_cast<UnkSelectorTree_A2_b68*>(self->tree);
        fn_71003aab64(tree, static_cast<UnkSelectorNode_A2_b68*>(self->condition));
        self->condition_running = true;
        return 4;
    }
    if (!self->condition_running)
        return self->branch_status;
    UnkStruct_71003aad48* tree = static_cast<UnkStruct_71003aad48*>(self->tree);
    if (fn_71003aad48(tree, self->condition))
        return self->branch_status;
    self->condition_dropped = true;
    return 4;
}
