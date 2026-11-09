// animtree::CAnimTreeNode's unit (tools/tu_map.py unit 404): the node's global weight (+0x64), its own weight
// (+0x60) times the parent's global weight, updated with the activation callbacks (vtable slots 0x58 and 0x60
// as it becomes positive or zero, 0x68 on any change); fn_71002df844 first runs slot 0x50 and always
// recurses into the children (+0x38, count +0x40), fn_71002df91c only when asked. Names and layouts are
// placeholders.
#include "types.h"

struct UnkAnimTreeChild_b66a2;

struct UnkAnimTreeNode_ovb0097 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void OnActivate();    // 0x58
    virtual void OnDeactivate();  // 0x60
    virtual void OnWeightChanged();  // 0x68

    u8 pad_0x8[0x18];
    bool active;  // 0x20
    u8 pad_0x21[0xf];
    UnkAnimTreeNode_ovb0097* parent;     // 0x30
    UnkAnimTreeNode_ovb0097** children;  // 0x38
    u32 child_count;                     // 0x40
    u8 pad_0x44[0x1c];
    f32 weight;            // 0x60
    f32 global_weight;     // 0x64

    void UpdateGlobalWeight() {
        f32 w = weight * (parent ? parent->global_weight : 1.0f);
        if (w != global_weight) {
            bool was_active = active;
            global_weight = w;
            if (w > 0.0f && !was_active) {
                active = true;
                OnActivate();
            } else if (w == 0.0f && was_active) {
                active = false;
                OnDeactivate();
            }
            OnWeightChanged();
        }
    }
};

void fn_71002df844(UnkAnimTreeNode_ovb0097* self) {
    self->vf_0x50();
    self->UpdateGlobalWeight();
    if (u32 n = self->child_count) {
        UnkAnimTreeNode_ovb0097** it = self->children;
        for (UnkAnimTreeNode_ovb0097** end = it + n; it != end; ++it)
            fn_71002df844(*it);
    }
}

void fn_71002df91c(UnkAnimTreeChild_b66a2* child, bool recurse) {
    auto* self = reinterpret_cast<UnkAnimTreeNode_ovb0097*>(child);
    self->UpdateGlobalWeight();
    if (recurse) {
        UnkAnimTreeNode_ovb0097** end = self->children + self->child_count;
        for (UnkAnimTreeNode_ovb0097** it = self->children; it != end; ++it)
            fn_71002df91c(reinterpret_cast<UnkAnimTreeChild_b66a2*>(*it), true);
    }
}
