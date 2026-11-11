// Unit 2510: empties an owner of reference-counted pointer arrays without freeing their storage. It first
// unlinks every target that still points back at the owner, then releases each array's references.
// Names and layouts are placeholders.
#include "game/UnkTU107.h"
#include "types.h"

struct UnkOwner_ld5afa4;

struct UnkTarget_ld5afa4 {
    u8 pad_0x0[0x30];
    union {
        UnkOwner_ld5afa4* owner;  // 0x30
    };
    union {
        s32 index;  // 0x38
    };
};

struct UnkRefObj_ld5afa4 {
    s32 ref_count;  // 0x8
    UnkTarget_ld5afa4* target;  // 0x10
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRefArray_ld5afa4 {
    UnkRefObj_ld5afa4** data;
    union {
        u32 count;
    };
    u32 capacity;

    UnkRefObj_ld5afa4** begin() { return data; }
    UnkRefObj_ld5afa4** end() { return data + count; }

    inline void Unlink(UnkOwner_ld5afa4* owner) {
        if (count == 0)
            return;
        for (UnkRefObj_ld5afa4** it = begin(); it != end(); ++it) {
            UnkRefObj_ld5afa4* o = *it;
            if (!o)
                continue;
            UnkTarget_ld5afa4* target = o->target;
            if (target && target->owner == owner) {
                target->owner = nullptr;
                target->index = -1;
            }
        }
    }

    inline void ReleaseAll() {
        for (u64 i = 0; i < count; ++i) {
            UnkRefObj_ld5afa4* o = data[i];
            if (o && --o->ref_count == 0)
                o->vf_0x10();
        }
        count = 0;
    }
};

struct UnkPlainArray_ld5afa4 {
    void* data;
    u32 count;
    u32 capacity;
};

struct UnkHolder_ld5afa4 {
    u8 pad_0x0[0x10];
    UnkRefArray_ld5afa4 a;  // 0x10
    u8 pad_0x20[0x70];
};

struct UnkOwner_ld5afa4 {
    u8 pad_0x0[0x8];
    UnkRefArray_ld5afa4 a;  // 0x8
    u8 pad_0x18[0x18];
    UnkRefArray_ld5afa4 b;  // 0x30
    u8 pad_0x40[0x1c];
    u32 field_0x5c;
    UnkPlainArray_ld5afa4 plain;  // 0x60
    u8 pad_0x70[0x18];
    u8 field_0x88;
    u8 pad_0x89[0x7];
    UnkRefArray_ld5afa4 d;  // 0x90
    u8 pad_0xa0[0x18];
    u32 element_count;  // 0xb8
    u8 elements[0x90];  // 0xbc, UnkHolder_ld5afa4 records
};

// NON_MATCHING(minor): in both unlink loops the count and the -1 constant use swapped registers (w8/w9).
void fn_7100d5afa4(UnkOwner_ld5afa4* self) {
    self->a.Unlink(self);
    self->b.Unlink(self);
    fn_710010d9f4(&self->plain);
    self->field_0x5c = 0;
    self->field_0x88 = 0;
    self->d.ReleaseAll();
    UnkHolder_ld5afa4* holder = reinterpret_cast<UnkHolder_ld5afa4*>(self->elements);
    UnkHolder_ld5afa4* holders_end = holder + self->element_count;
    for (; holder != holders_end; ++holder)
        holder->a.ReleaseAll();
    self->a.ReleaseAll();
    self->b.ReleaseAll();
}
