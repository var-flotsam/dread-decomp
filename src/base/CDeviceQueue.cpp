// base::file::CDeviceQueue: its destructors, adding a device, and the calls it forwards to its
// devices.
#include "base/CDeviceQueue.h"

using base::file::CDeviceQueue;
using base::file::UnkDevice;
using base::file::UnkDeviceNode;

void fn_7100084778(CDeviceQueue* self) {
    self->CDeviceQueue::~CDeviceQueue();
}

void fn_710008489c(CDeviceQueue* self) {
    self->CDeviceQueue::~CDeviceQueue();
    __builtin_operator_delete(self);
}

// Slot 0x20: per device, slot 0x30 or else slot 0x20.
bool fn_7100084954(CDeviceQueue* self) {
    bool ok = true;
    for (UnkDeviceNode* n = self->devices.head; n; n = n->next) {
        if (!ok)
            break;
        ok = n->device->vf_0x30() || n->device->vf_0x20();
    }
    return ok;
}

bool fn_7100084a04(CDeviceQueue* self) {
    bool ok = true;
    for (UnkDeviceNode* n = self->devices.head; n; n = n->next) {
        if (!ok)
            break;
        ok = n->device->vf_0x30();
    }
    return ok;
}

s32 fn_7100084bf4(CDeviceQueue* self, void* a, void* b, void* c) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x38(a, b, c);
    return r;
}

s32 fn_7100084c68(CDeviceQueue* self, void* a, void* b, bool c, void* d, u32 e, u32 f) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x40(a, b, c, d, e, f);
    return r;
}

s32 fn_7100084cf8(CDeviceQueue* self, void* a, bool b) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x48(a, b);
    return r;
}

s32 fn_7100084a58(CDeviceQueue* self, void* a) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x68(a);
    return r;
}

s32 fn_7100084ab0(CDeviceQueue* self, void* a) {
    s32 r = 2;
    for (UnkDeviceNode* n = self->devices.head; n && r == 2; n = n->next)
        r = n->device->vf_0x70(a);
    return r;
}

s32 fn_7100084b08(CDeviceQueue* self, void* a, bool b) {
    s32 r = 2;
    for (UnkDeviceNode* n = self->devices.head; n && r == 2; n = n->next)
        r = n->device->vf_0x78(a, b);
    return r;
}

s32 fn_7100084b74(CDeviceQueue* self, void* a, u32 b, bool c, bool d, void* e) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x80(a, b, c, d, e);
    return r;
}

s32 fn_7100084d64(CDeviceQueue* self, void* a, void* b, void* c, bool d) {
    s32 r = 2;
    for (UnkDeviceNode* n = self->devices.head; n && r == 2; n = n->next)
        r = n->device->vf_0x88(a, b, c, d);
    return r;
}

s32 fn_7100084ddc(CDeviceQueue* self, void* a) {
    s32 r = 1;
    for (UnkDeviceNode* n = self->devices.head; n && r == 1; n = n->next)
        r = n->device->vf_0x90(a);
    return r;
}

// Appends a device.
void fn_7100084e34(CDeviceQueue* self, UnkDevice* device) {
    self->devices.PushBack(device);
}

// Copy-assignment.
CDeviceQueue* fn_71000851dc(CDeviceQueue* self, const CDeviceQueue* other) {
    return &(*self = *other);
}
