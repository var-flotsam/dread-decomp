// Unit 63 of tools/tu_map.py (engine/base, the 0x70-byte registry of base/UnkRegistry.h): handing a
// request to the handler registered for its owner. The owner's key is the xor of its two words at
// +0x30; the handler's vf_0x78 answers 0 (handled), 1 (ignored) or another code, which goes to the
// registry's listener (+0x68, with its context at +0x60). Names and layouts are placeholders.
#include "types.h"

struct UnkRequestOwner63 {
    u8 pad_0x0[0x30];
    u32 key_lo;
    u32 key_hi;
};

struct UnkHandler63 {
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
    virtual u32 vf_0x78(void* args, bool flag);
};

struct UnkHandlerNode63 {
    UnkRequestOwner63* owner;
    UnkHandler63* handler;
    u8 pad_0x10[0x10];
    UnkHandlerNode63* next;
};

struct UnkRequest63 {
    UnkRequestOwner63* owner;
    u8 args[1];  // +0x8
};

struct UnkHandlerTable63 {
    u8 pad_0x0[0x18];
    UnkHandlerNode63** buckets;
    u32 count;
    u32 bucket_count;
    u8 pad_0x28[0x38];
    void* listener_context;
    void (*listener)(void* context, UnkHandler63* handler, u32 code);
};

bool fn_710008868c(UnkHandlerTable63* self, UnkRequest63* request, u32 flags) {
    UnkRequestOwner63* owner = request->owner;
    u32 hash = owner->key_lo ^ owner->key_hi;
    if (self->count == 0)
        return false;
    UnkHandlerNode63** bucket = hash % self->bucket_count + self->buckets;
    if (!bucket)
        return false;
    UnkHandlerNode63* node = *bucket;
    if (!node)
        return false;
    while (node->owner != owner) {
        node = node->next;
        if (!node)
            return false;
    }
    UnkHandler63* handler = node->handler;
    if (!handler)
        return false;
    u32 code = handler->vf_0x78(request->args, flags & 1);
    if (code == 0)
        return true;
    if (code != 1 && self->listener)
        self->listener(self->listener_context, handler, code);
    return false;
}
