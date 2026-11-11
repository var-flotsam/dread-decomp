// GUI::CDisplayObjectContainer unit (tools/tu_map.py, the name from reflection): the container's
// children, a list at +0x1c0 (base/UnkSList.h; a pool list, base/UnkCloneHelpers.h, where it is
// copied): handing them to the GUI root (fn_71011d9264, fn_71011d90f4), taking one out
// (fn_71011d932c), passing a call on to them (fn_71011d9e20) and creating a named child of a
// reflected class (fn_71011d9780); and the erase of an 8-byte element from a (data, count) array
// (fn_71011db990). Names and layouts are placeholders inferred from the code.
#include <stdio.h>
#include <string.h>

#include "base/UnkCloneHelpers.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CMissionLog.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CClass;

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0032 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 uses;  // 0x44
    };
};

static inline const char* Text_ovb0032(const CStrId& id) {
    ++reinterpret_cast<UnkStrIdText_ovb0032*>(id.data)->uses;
    const char* text = reinterpret_cast<UnkStrIdText_ovb0032*>(id.data)->str.data;
    return text ? text : g_unk_710162e2ed;
}

struct UnkDisplayObject_ovb0032 {
    virtual void vf_0x0();
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130
};

struct UnkChildListTag_ovb0032;
using UnkChildList_ovb0032 = base::UnkSList<UnkChildListTag_ovb0032>;

struct UnkDisplayObjectContainer_ovb0032 {
    u8 pad_0x0[0x1c0];
    UnkChildList_ovb0032 children;  // 0x1c0
};

static inline UnkDisplayObject_ovb0032* Child_ovb0032(UnkChildList_ovb0032::Node* n) {
    return reinterpret_cast<UnkDisplayObject_ovb0032*>(n->value);
}

// Hands every child whose name doesn't start with `prefix` to the GUI root.
void fn_71011d9264(UnkDisplayObjectContainer_ovb0032* self, const CRntString* prefix) {
    UnkChildList_ovb0032::Node* n = self->children.head;
    while (n) {
        if (strstr(Text_ovb0032(Child_ovb0032(n)->name), prefix->data ? prefix->data : g_unk_710162e2ed) ==
            Text_ovb0032(Child_ovb0032(n)->name)) {
            n = n->next;
            continue;
        }
        UnkDisplayObject_ovb0032* child = Child_ovb0032(n);
        n = n->next;
        fn_71011b57c4(g_unk_717271e8f8, child);
    }
}

// Removes the first node holding `value` and frees it (as src/game/CScenario/CScenario_Lists_b52.cpp).
static inline __attribute__((always_inline)) void Remove_ovb0032(UnkChildList_ovb0032& list, u64 value) {
    UnkChildList_ovb0032::Node* node = list.head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkChildList_ovb0032::Node* prev = nullptr;
    UnkChildList_ovb0032::Node* cur = list.head;
    UnkChildList_ovb0032::Node* next;
    UnkChildList_ovb0032::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (list.tail == node)
            list.tail = prev;
    } else {
        list.head = next;
    }
    if (--list.count == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }
    list.allocator->vf_0x48(node);
}

// Takes `child` out of the list; returns it.
UnkDisplayObject_ovb0032* fn_71011d932c(UnkDisplayObjectContainer_ovb0032* self, UnkDisplayObject_ovb0032* child) {
    Remove_ovb0032(self->children, reinterpret_cast<u64>(child));
    return child;
}

void fn_71011cf290(UnkDisplayObjectContainer_ovb0032* self, void* context, bool recursive, bool flag);

// The display object's version of this call (fn_71011cf290), then, when `recursive`, each child's
// (its virtual at +0x1b8).
void fn_71011d9e20(UnkDisplayObjectContainer_ovb0032* self, void* context, bool recursive, bool flag) {
    fn_71011cf290(self, context, recursive, flag);
    if (!recursive)
        return;
    for (UnkChildList_ovb0032::Node* n = self->children.head; n; n = n->next) {
        UnkDisplayObject_ovb0032* child = Child_ovb0032(n);
        void** vtable = *reinterpret_cast<void***>(child);
        reinterpret_cast<void (*)(UnkDisplayObject_ovb0032*, void*, bool, bool)>(vtable[0x1b8 / 8])(child, context,
                                                                                                    true, flag);
    }
}

// --- The children as a pool list ---

struct UnkChildPoolListTag_ovb0032;
template <>
struct base::UnkPoolListTraits<UnkChildPoolListTag_ovb0032> {
    static char* vtable() { return g_unk_7101a9d490; }
    static char* base_vtable() { return g_unk_7101a9d4b0; }
};
// Nodes of {child, next}.
using UnkChildPoolList_ovb0032 = base::UnkPoolList<UnkChildPoolListTag_ovb0032, 0x8, 0x10>;

struct UnkChildNode_ovb0032 {
    void* child;
    UnkChildNode_ovb0032* next;
};

// The list's copy constructor.
void fn_7100b59c4c(UnkChildPoolList_ovb0032* self, const UnkChildPoolList_ovb0032* other);

struct UnkGuiDisplayBase;

struct UnkGuiContainer_ovb0032 {
    u8 pad_0x0[0x1c0];
    UnkChildPoolList_ovb0032 children;  // 0x1c0
};

// Hands every child to the GUI root (fn_71011b57c4), working on a copy of the list.
void fn_71011d90f4(UnkGuiDisplayBase* self) {
    UnkChildPoolList_ovb0032 children;
    fn_7100b59c4c(&children, &reinterpret_cast<UnkGuiContainer_ovb0032*>(self)->children);
    while (children.count) {
        fn_71011b57c4(g_unk_717271e8f8, reinterpret_cast<UnkChildNode_ovb0032*>(children.head)->child);
        if (char* node = children.head) {
            children.head = reinterpret_cast<char*>(reinterpret_cast<UnkChildNode_ovb0032*>(node)->next);
            children.allocator->vf_0x48(node);
            if (--children.count == 0)
                children.tail = children.head;
        }
    }
}

// --- Creating a child ---

struct UnkGuiNamedObject_ovb0032 {
    u8 pad_0x0[0x130];
    CStrId name;  // 0x130
};

// Creates a display object of class `arg` (GUI::CDisplayObject or derived; null otherwise), runs its
// virtual at +0x30, names it "Child_<index>" and adds it last to the container.
void* fn_71011d9780(char* self, u32 index, void* arg) {
    const CClass* cls = static_cast<const CClass*>(arg);
    void* mem = nullptr;
    if (fn_710009d50c(cls, fn_71011c3868()) && cls->m_fn_0x28) {
        mem = fn_7100083990()->vf_0x40(cls->m_size, 8, 0xc);
        if (mem)
            reinterpret_cast<void (*)(void*)>(cls->m_fn_0x28)(mem);
    }
    UnkGuiDisplayObject* object = static_cast<UnkGuiDisplayObject*>(mem);
    object->vf_0x30();
    char name[0x20];
    snprintf(name, sizeof(name), "Child_%d", index);
    {
        CStrId id(name, true);
        reinterpret_cast<UnkGuiNamedObject_ovb0032*>(object)->name = id;
    }
    fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(self), object);
    return object;
}

// --- An array erase ---

template <typename T>
struct UnkPodArray_ovb0032 {
    T* data;
    u32 count;
};

// Erases the element at the index `*it`, moving the rest down (as
// src/base/reflection/UnkArrayEraseAt_A1_b72.cpp).
void fn_71011db990(UnkPodArray_ovb0032<u64>* self, const u32* it) {
    u64* base = reinterpret_cast<u64*>(self);
    u64* p = reinterpret_cast<u64*>(reinterpret_cast<char*>(base) + (u64)*it * sizeof(u64));
    u32 index = (u32)(p - base);
    if (index < self->count) {
        u64* pos = self->data + index;
        __builtin_memcpy(pos, pos + 1, (self->count - index - 1) * sizeof(u64));
        --self->count;
    }
}
