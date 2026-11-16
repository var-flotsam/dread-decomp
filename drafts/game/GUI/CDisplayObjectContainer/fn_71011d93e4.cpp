// GUI::CDisplayObjectContainer unit (tools/tu_map.py 2748): adds a child at the front of the container's
// list at +0x1c0 unless a child with its name is listed already, then hands it to the GUI root.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

struct UnkChildListTag_l1d93e4;
using UnkChildList_l1d93e4 = base::UnkSList<UnkChildListTag_l1d93e4>;

struct UnkDisplayObject_l1d93e4 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();

    u8 pad_0x8[0x128];
    CStrId name;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;  // vf_0xa0 has run
    u8 pad_0x185[0x3b];
    UnkChildList_l1d93e4 children;  // 0x1c0
};
static_assert(__builtin_offsetof(UnkDisplayObject_l1d93e4, name) == 0x130, "layout");
static_assert(__builtin_offsetof(UnkDisplayObject_l1d93e4, field_0x184) == 0x184, "layout");
static_assert(__builtin_offsetof(UnkDisplayObject_l1d93e4, children) == 0x1c0, "layout");

void fn_71011ba2bc(UnkGuiRoot* root, UnkDisplayObject_l1d93e4* child);

static inline UnkDisplayObject_l1d93e4* FindChild(UnkDisplayObject_l1d93e4* self, const CStrId& name) {
    for (UnkChildList_l1d93e4::Node* n = self->children.head; n; n = n->next) {
        UnkDisplayObject_l1d93e4* child = reinterpret_cast<UnkDisplayObject_l1d93e4*>(n->value);
        if (child->name == name)
            return child;
    }
    return nullptr;
}

bool fn_71011d93e4(UnkDisplayObject_l1d93e4* self, UnkDisplayObject_l1d93e4* child) {
    if (FindChild(self, child->name))
        return false;
    fn_71011cef74(child, self);
    self->children.PushFront(reinterpret_cast<u64>(child));
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
    fn_71011ba2bc(g_unk_717271e8f8, child);
    return true;
}
