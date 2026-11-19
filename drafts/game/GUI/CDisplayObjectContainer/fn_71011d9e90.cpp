// CDisplayObjectContainer's unit (unit 2748): offers an input to the children of a visible container (when it
// accepts input, or its vf_0x208 says so), first to last, until one takes it (or to all with `all` set); if
// none takes it, the container's own handler (fn_71011cff5c) does. Names and layouts are placeholders.
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkDisplayChild_l1d9e90 {
    void** vtable;

    void* vf_0x210(u32 input, void* event, bool d, bool all, bool f) {
        return reinterpret_cast<void* (*)(UnkDisplayChild_l1d9e90*, u32, void*, bool, bool, bool)>(
            vtable[0x210 / 8])(this, input, event, d, all, f);
    }
};

struct UnkDisplayChildNode_l1d9e90 {
    UnkDisplayChild_l1d9e90* child;    // 0x0
    UnkDisplayChildNode_l1d9e90* next;  // 0x8
};

struct UnkDisplayContainer_l1d9e90 {
    void** vtable;
    u8 pad_0x8[0xac];
    bool visible;  // 0xb4
    u8 pad_0xb5[0xd4];
    bool accepts_input;  // 0x189
    u8 pad_0x18a[0x46];
    UnkDisplayChildNode_l1d9e90* children;  // 0x1d0

    bool vf_0x208(void* event) {
        return reinterpret_cast<bool (*)(UnkDisplayContainer_l1d9e90*, void*)>(vtable[0x208 / 8])(this, event);
    }
};
static_assert(__builtin_offsetof(UnkDisplayContainer_l1d9e90, children) == 0x1d0, "layout");

void* fn_71011d9e90(UnkDisplayContainer_l1d9e90* self, u32 input, void* event, bool d, bool all, bool f) {
    if (!self->visible)
        return nullptr;
    if (!self->accepts_input && !self->vf_0x208(event))
        return nullptr;
    if (UnkDisplayChildNode_l1d9e90* node = self->children) {
        void* taken = nullptr;
        do {
            void* hit = node->child->vf_0x210(input, event, d, all, f);
            if (hit)
                taken = hit;
            node = node->next;
        } while (node && (!taken || all));
        if (taken)
            return taken;
    }
    return fn_71011cff5c(self, input, event, d, all, f);
}
