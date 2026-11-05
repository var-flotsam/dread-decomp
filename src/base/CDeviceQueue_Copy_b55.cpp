// base::file::CDeviceQueue's copy constructors (reflection slots 0x30 and 0x38): the name's
// string is shared, and the device list is rebuilt node by node from the same allocator.
#include "base/CDeviceQueue.h"

using base::file::CDeviceQueue;
using base::file::UnkDeviceNode;

extern void* g_unk_7101a1adb0[];  // CDeviceQueue's vtable
extern void* g_unk_7101a1ae58[];  // UnkDeviceList's vtable

static __attribute__((always_inline)) void CopyConstruct(CDeviceQueue* dst, const CDeviceQueue* src) {
    *reinterpret_cast<void**>(dst) = reinterpret_cast<char*>(g_unk_7101a1adb0) + 0x10;
    dst->name.data = src->name.data;
    if (&g_unk_7101d47108->empty != dst->name.data)
        ++dst->name.data->ref_count;
    void** list_vtable = g_unk_7101a1ae58;
    asm("" : "+r"(list_vtable));
    dst->devices.count = 0;
    dst->devices.head = nullptr;
    dst->devices.tail = nullptr;
    asm volatile("" ::: "memory");
    *reinterpret_cast<void**>(&dst->devices) = reinterpret_cast<char*>(list_vtable) + 0x10;
    dst->devices.allocator = src->devices.allocator;
    dst->devices.flags = src->devices.flags;
    for (UnkDeviceNode* n = src->devices.head; n; n = n->next)
        dst->devices.PushBack(n->device);
}

void fn_7100084f44(void* dst, const void* src) {
    CopyConstruct(static_cast<CDeviceQueue*>(dst), static_cast<const CDeviceQueue*>(src));
}

void fn_7100085030(void* dst, const void* src) {
    CopyConstruct(static_cast<CDeviceQueue*>(dst), static_cast<const CDeviceQueue*>(src));
}
