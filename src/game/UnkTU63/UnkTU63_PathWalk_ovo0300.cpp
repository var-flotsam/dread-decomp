#include "base/UnkDestructors.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::UnkStrIdMemberOf;

struct UnkRegistryString_b0248 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// A path id and the link to the next one.
struct UnkListedPath_b0248 {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath_b0248* next;
};

extern void* g_unk_7101a1b030[];
struct UnkFilePathList_b0248 {
    void** vtable;
    u32 count;
    UnkListedPath_b0248* head;
    UnkListedPath_b0248* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath_b0248* n = head; n;) {
            UnkListedPath_b0248* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

class UnkRegistryHandler_b0248 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const void* vf_0x18();
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
    virtual u32 vf_0x80(UnkListedPath_b0248* path, u32 a, bool b, u32 c, UnkFilePathList_b0248* out);
};

struct UnkRegistryNode_b0248 {
    UnkRegistryString_b0248* key;
    UnkRegistryHandler_b0248* value;  // 0x8
    u8 pad_0x10[0x10];
    UnkRegistryNode_b0248* next;  // 0x20
};

struct UnkRegistry_b0248 {
    u8 pad_0x0[0x18];
    UnkRegistryNode_b0248** buckets;  // 0x18
    u32 count;                        // 0x20
    u32 capacity;                     // 0x24
    u8 pad_0x28[0x38];
    void* context;                    // 0x60
    void (*on_error)(void* context, UnkRegistryHandler_b0248* handler, u32 result);  // 0x68
};

bool fn_7100087b0c(UnkRegistry_b0248* self, UnkRegistryHandler_b0248* handler, UnkListedPath_b0248* path, u32 depth,
                   UnkFilePathList_b0248* out) {
    if (depth == 0) {
        u32 result = handler->vf_0x80(path, 0, true, 0, out);
        if (result == 0)
            return true;
        if (result != 1 && self->on_error)
            self->on_error(self->context, handler, result);
        return false;
    }
    UnkFilePathList_b0248 children;
    u32 result = handler->vf_0x80(path, 0, true, 0, &children);
    bool ok = false;
    if (result == 0) {
        ok = true;
        for (UnkListedPath_b0248* n = children.head; n; n = n->next) {
            if (!ok)
                break;
            ok = fn_7100087b0c(self, handler, n, depth - 1, out);
        }
    } else if (result != 1 && self->on_error) {
        self->on_error(self->context, handler, result);
    }
    return ok;
}
