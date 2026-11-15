// Unit 2744 (gui/scripts/guimanager.lua): the GUI root sends an event to its target object in three
// phases, down through the target's ancestors, to the target, then back up when the event bubbles.
// Names and layouts are placeholders.

#include "game/UnkGuiRoot.h"
#include "types.h"

namespace {

struct UnkGuiEventTarget {
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
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void HandleEvent(struct UnkGuiDispatchEvent* event);  // vf_0xb8

    u8 pad_0x8[0x118];
    UnkGuiEventTarget* parent;
};
static_assert(__builtin_offsetof(UnkGuiEventTarget, parent) == 0x120, "UnkGuiEventTarget placeholder layout");

struct UnkGuiEventHandle {
    u8 pad_0x0[0x10];
    UnkGuiEventTarget* target;
};

struct UnkGuiDispatchEvent {
    UnkGuiEventHandle* handle;
    u8 pad_0x8[0x11];
    bool bubbles;
    bool stopped;
    u8 pad_0x1b[5];
    u64 phase;

    UnkGuiEventTarget* Target() const { return handle ? handle->target : nullptr; }
};
static_assert(__builtin_offsetof(UnkGuiDispatchEvent, phase) == 0x20, "UnkGuiDispatchEvent placeholder layout");

struct UnkGuiAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u64 align, u32 tag);  // vf_0x40
    virtual void Free(void* p);                         // vf_0x48
};

struct UnkGuiPathNode {
    UnkGuiEventTarget* value;
    UnkGuiPathNode* next;
    UnkGuiPathNode* prev;
};

// The root's list of the target's ancestors, outermost first.
struct UnkGuiPathList {
    u32 count;
    u8 pad_0x4[4];
    UnkGuiPathNode* head;
    UnkGuiPathNode* tail;
    u32 tag;
    u8 pad_0x1c[4];
    UnkGuiAllocator* allocator;

    void Clear() {
        for (UnkGuiPathNode* node = head; node;) {
            UnkGuiPathNode* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }

    void PushFront(UnkGuiEventTarget* value) {
        UnkGuiPathNode* node = static_cast<UnkGuiPathNode*>(allocator->Alloc(sizeof(UnkGuiPathNode), 8, tag));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        node->prev = nullptr;
        if (count != 0) {
            head->prev = node;
            node->next = head;
            head = node;
            count++;
        } else {
            head = node;
            tail = node;
            count = 1;
        }
    }
};

struct UnkGuiDispatchRoot {
    u8 pad_0x0[0xdc0];
    UnkGuiPathList path;
};
static_assert(__builtin_offsetof(UnkGuiDispatchRoot, path) == 0xdc0, "UnkGuiDispatchRoot placeholder layout");

}  // namespace

void fn_71011b6e0c(UnkGuiRoot* root, UnkGuiEvent* message) {
    UnkGuiPathList& path = reinterpret_cast<UnkGuiDispatchRoot*>(root)->path;
    UnkGuiDispatchEvent* event = reinterpret_cast<UnkGuiDispatchEvent*>(message);

    if (!event->Target())
        return;

    path.Clear();
    for (UnkGuiEventTarget* p = event->handle->target->parent; p; p = p->parent)
        path.PushFront(p);

    bool stopped = event->stopped;
    event->phase = 1;
    if (!stopped) {
        for (UnkGuiPathNode* node = path.head; node && event->Target(); node = node->next) {
            node->value->HandleEvent(event);
            stopped = event->stopped;
            if (stopped)
                break;
        }
    }

    if (event->Target() && !stopped) {
        event->phase = 2;
        event->Target()->HandleEvent(event);
    }

    if (!event->Target() || event->stopped || !event->bubbles)
        return;

    event->phase = 4;
    for (UnkGuiPathNode* node = path.tail; node && event->Target(); node = node->prev) {
        node->value->HandleEvent(event);
        if (event->stopped)
            break;
    }
}
