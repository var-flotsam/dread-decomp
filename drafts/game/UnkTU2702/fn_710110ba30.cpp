// Unit 2702: an AI component rebuilds a list of the entries that accept a request. Names and layouts are
// placeholders.
#include "base/CDeviceQueue.h"
#include "types.h"

using base::file::UnkDevice;
using base::file::UnkDeviceList;

struct UnkCollectRequest_l10ba30 {
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
    virtual bool vf_0x58(UnkDeviceList* list, u32 arg);
};

// A count followed by 4-byte-aligned pointer entries.
struct UnkCollectEntries_l10ba30 {
    u32 count;
    struct __attribute__((packed, aligned(4))) Entry {
        UnkCollectRequest_l10ba30* p;
    } items[1];
};

struct UnkCollectAI_l10ba30 {
    u8 pad_0x0[0x1618];
    UnkCollectEntries_l10ba30 entries;  // 0x1618
};

struct UnkCollectResult_l10ba30 {
    u8 pad_0x0[0x28];
    UnkDeviceList list;  // 0x28
};

bool fn_710110ba30(UnkCollectAI_l10ba30* self, UnkCollectResult_l10ba30* result, u32 arg) {
    result->list.Clear();
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkCollectRequest_l10ba30* entry = self->entries.items[i].p;
        if (entry->vf_0x58(&result->list, arg)) {
            UnkDevice* device = reinterpret_cast<UnkDevice*>(entry);
            result->list.PushBack(device);
        }
    }
    return result->list.count != 0;
}
