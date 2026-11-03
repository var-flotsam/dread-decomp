// Unit 322 (tools/tu_map.py: 0x710021c4cc-0x710021d8ac), next to the media units: an object holding a
// counted resource (+0x10), an array of shared objects (+0x18, with its allocator at +0x38) and a
// member at +0x40. Its destructor (fn_710021c808) and deleting destructor (fn_710021c9d8). Names and
// layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

// An object shared through an atomic count at +8; slot 0x10 destroys it.
struct UnkShared_b50 {
    void** vtable;
    s32 ref_count;

    void vf_0x10() { reinterpret_cast<void (*)(UnkShared_b50*)>(vtable[2])(this); }
};

// The allocator behind the array (placeholder slot names).
class UnkMovieAllocator322_b50 {
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
    virtual void vf_0x48(void* p);  // frees
};

struct UnkMovieUnit322_b50 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkCountedResource* field_0x10;
    UnkShared_b50** field_0x18;
    u32 field_0x20;  // count
    u32 field_0x24;  // capacity
    u8 pad_0x28[0x10];
    UnkMovieAllocator322_b50* field_0x38;
    u8 field_0x40[0x8];
};

extern void* g_unk_7101a20420[];  // the vtable
void fn_710021c8dc(void* member);

static inline __attribute__((always_inline)) void Destroy_b50(UnkMovieUnit322_b50* self) {
    self->vtable = g_unk_7101a20420 + 2;
    fn_710021c8dc(self->field_0x40);
    if (self->field_0x18) {
        for (u64 i = 0; i < self->field_0x20; ++i) {
            if (UnkShared_b50* obj = self->field_0x18[i]) {
                __atomic_fetch_sub(&obj->ref_count, 1, __ATOMIC_SEQ_CST);
                if (__atomic_load_n(&obj->ref_count, __ATOMIC_SEQ_CST) == 0)
                    obj->vf_0x10();
            }
        }
        self->field_0x20 = 0;
        self->field_0x38->vf_0x48(self->field_0x18);
        self->field_0x24 = 0;
    }
    if (UnkCountedResource* res = self->field_0x10) {
        if (--res->ref_count == 0)
            fn_710001737c(res);
    }
}

void fn_710021c808(UnkMovieUnit322_b50* self) {
    Destroy_b50(self);
}

void fn_710021c9d8(UnkMovieUnit322_b50* self) {
    Destroy_b50(self);
    operator delete(self);
}
