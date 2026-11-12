// GUI::CDisplayObjectContainer: adds a child to the container's list unless already present, then notifies the GUI root.
// Names and layouts are placeholders.

#include "game/UnkGuiRoot.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkChildKey {
    u8 pad_0x0[0x130];
    u64 field_0x130;
};

struct UnkChildNode {
    UnkChildKey* child;
    UnkChildNode* next;
};

struct UnkNodeAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u64 align, u32 tag);
};

struct UnkDisplayContainer {
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
    virtual void OnFirstChild();

    u8 pad_0x8[0x17c];
    u8 field_0x184;
    u8 pad_0x185[0x43];
    s32 count;
    u8 pad_0x1cc[4];
    UnkChildNode* head;
    UnkChildNode* tail;
    UnkNodeAllocator* allocator;
    u32 allocTag;
};

static_assert(__builtin_offsetof(UnkDisplayContainer, count) == 0x1c8, "");
static_assert(__builtin_offsetof(UnkDisplayContainer, allocTag) == 0x1e8, "");

void fn_71011ba2bc(UnkGuiRoot* root, void* child);

static inline UnkChildKey* FindSame(UnkChildNode* n, const UnkChildKey& key) {
    UnkChildKey* c;
    do {
        c = n->child;
        n = n->next;
        if (c->field_0x130 != key.field_0x130) {
            c = nullptr;
        }
    } while ((c == nullptr) & (n != nullptr));
    return c;
}

bool fn_71011d968c(UnkDisplayContainer* self, UnkChildKey* child) {
    if (self->head && FindSame(self->head, *child)) {
        return false;
    }

    fn_71011cef74(child, self);

    UnkChildNode* node = (UnkChildNode*)self->allocator->Alloc(0x10, 8, self->allocTag);
    if (node) {
        node->child = child;
        node->next = nullptr;
        if (self->count == 0) {
            self->head = node;
            self->tail = node;
        } else {
            self->tail->next = node;
            node->next = nullptr;
            self->tail = node;
        }
        self->count++;
    }

    if (!self->field_0x184) {
        self->field_0x184 = 1;
        self->OnFirstChild();
    }

    fn_71011ba2bc(g_unk_717271e8f8, child);
    return true;
}
