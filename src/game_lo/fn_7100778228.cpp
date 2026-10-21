// Destructor of a pair of identical containers (at +0 and +0x40, destroyed in reverse order).
// Each releases a reference-counted object, then destroys its 16-byte elements through their
// per-element operation tables and frees the storage through its allocator.
#include "types.h"

extern void* unk_7101a20950[];  // the derived container's vtable (reached through the GOT)
extern void* unk_7101a20998[];  // the base container's vtable (reached through the GOT)

struct UnkElem_7100778228;
struct UnkElemOps_7100778228 {
    void* slots[8];
    void (*destroy)(UnkElem_7100778228* elem);
};

struct UnkElem_7100778228 {
    u64 field_0x0;
    UnkElemOps_7100778228* ops;
};

class UnkAllocator_7100778228 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* p);
};

class UnkRefCounted_7100778228 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();

    s32 ref_count;
};

struct UnkContainer_7100778228 {
    void** vtable;
    UnkElem_7100778228* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    UnkAllocator_7100778228* allocator;
    u64 field_0x30;
    UnkRefCounted_7100778228* ref;

    __attribute__((always_inline)) ~UnkContainer_7100778228() {
        vtable = &unk_7101a20950[2];
        if (ref) {
            if (--ref->ref_count == 0)
                ref->vf_0x10();
        }
        vtable = &unk_7101a20998[2];
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkElem_7100778228* e = &data[i];
                if (e->ops) {
                    e->ops->destroy(e);
                    e->ops = nullptr;
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkPair_7100778228 {
    UnkContainer_7100778228 first;
    UnkContainer_7100778228 second;
};

void fn_7100778228(UnkPair_7100778228* self) {
    self->second.~UnkContainer_7100778228();
    self->first.~UnkContainer_7100778228();
}
