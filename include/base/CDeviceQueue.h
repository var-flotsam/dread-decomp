#pragma once

// base::file::CDeviceQueue (the name comes from reflection: its slot 0x0 returns the class that
// base::file::CDeviceQueue::refl_GetClass builds; size 0x40). A device that forwards each call
// to a list of devices in order, stopping at the first that doesn't report success. The device
// interface and the list keep placeholder names; members keep offset-based ones.

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace base::reflection {
class CClass;
}

namespace base::file {

// The device interface (CDeviceQueue's own vtable has the same slots). Most calls return a
// status: 1 (or 2 for slots 0x70, 0x78 and 0x88) means success.
struct UnkDevice {
    virtual const reflection::CClass* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30();
    virtual s32 vf_0x38(void* a, void* b, void* c);
    virtual s32 vf_0x40(void* a, void* b, bool c, void* d, u32 e, u32 f);
    virtual s32 vf_0x48(void* a, bool b);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual s32 vf_0x68(void* a);
    virtual s32 vf_0x70(void* a);
    virtual s32 vf_0x78(void* a, bool b);
    virtual s32 vf_0x80(void* a, u32 b, bool c, bool d, void* e);
    virtual s32 vf_0x88(void* a, void* b, void* c, bool d);
    virtual s32 vf_0x90(void* a);
};

struct UnkDeviceNode {
    UnkDevice* device;
    UnkDeviceNode* next;
};

// A singly linked list of devices, its nodes from `allocator` with `flags` (vtable via GOT
// 0x7101cbc378; its destructor out of line is fn_7100084838). Its body from +8 is
// base::UnkLinkedList, whose count is a union member: appending reloads the count after storing
// the new node's link, as the original does.
struct UnkDeviceList : UnkLinkedList<UnkDeviceNode> {
    virtual ~UnkDeviceList() { Clear(); }
    virtual void vf_0x8();

    void PushBack(UnkDevice* const& device) {
        auto* node = static_cast<UnkDeviceNode*>(allocator->vf_0x40(sizeof(UnkDeviceNode), 8, flags));
        if (!node)
            return;
        node->device = device;
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

// The queue's name: a CStrId (its two-step destructor and copy-assignment are what the queue's
// destructor and fn_7100084efc's callers inline).
using UnkDeviceName = global::CStrId;

// Vtable via GOT 0x7101cbc370 (slots as UnkDevice).
class CDeviceQueue {
public:
    virtual const reflection::CClass* vf_0x0();  // fn_7100084ee0
    // Slots 0x8 and 0x10 (fn_7100084778, fn_710008489c).
    virtual ~CDeviceQueue() {}
    virtual UnkDeviceName* vf_0x18();  // fn_7100084efc: &name

    UnkDeviceName name;  // +0x8
    UnkDeviceList devices;  // +0x10

    __attribute__((always_inline)) CDeviceQueue& operator=(const CDeviceQueue& other) {
        name = other.name;
        if (this != &other) {
            devices.Clear();
            for (UnkDeviceNode* n = other.devices.head; n; n = n->next)
                devices.PushBack(n->device);
        }
        return *this;
    }
};

}  // namespace base::file
