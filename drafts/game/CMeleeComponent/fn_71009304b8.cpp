// CMeleeComponent's unit: copies a hit description, swaps its two ends (handles and points), flips its
// ratio and hands it to the target's handler. Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMeleeRefCounted_l9304b8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;
    u64 field_0x10;
};

// A counted reference; assigned by fn_71002a5d1c.
struct UnkMeleeHandle_l9304b8 {
    UnkMeleeRefCounted_l9304b8* ptr;

    UnkMeleeHandle_l9304b8() : ptr(nullptr) {}
    UnkMeleeHandle_l9304b8(const UnkMeleeHandle_l9304b8& o) : ptr(nullptr) {
        if (o.ptr) {
            o.ptr->ref_count++;
            ptr = o.ptr;
        }
    }
    UnkMeleeHandle_l9304b8& operator=(const UnkMeleeHandle_l9304b8& o) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&o));
        return *this;
    }
    ~UnkMeleeHandle_l9304b8() { Release(); }

    void Release() {
        UnkMeleeRefCounted_l9304b8* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
    void Set(UnkMeleeRefCounted_l9304b8* p) {
        if (p) {
            p->ref_count++;
            Release();
            ptr = p;
        }
    }
};

struct UnkMeleeHit_l9304b8 {
    UnkMeleeHandle_l9304b8 from;
    UnkMeleeHandle_l9304b8 to;
    u32 field_0x10;
    UnkVec3f field_0x14;
    UnkVec3f from_pos;
    UnkVec3f to_pos;
    UnkVec3f field_0x38;  // x: how far along the hit is

    UnkMeleeHit_l9304b8(const UnkMeleeHit_l9304b8& o)
        : from(o.from) {
        to.Set(o.to.ptr);
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        from_pos = o.from_pos;
        to_pos = o.to_pos;
        field_0x38 = o.field_0x38;
    }

    void Reverse() {
        UnkMeleeHandle_l9304b8 handle(from);
        from = to;
        to = handle;
        UnkVec3f pos(from_pos);
        from_pos = to_pos;
        to_pos = pos;
        field_0x38.x = 1.0f - field_0x38.x;
    }
};

struct UnkMeleeHandler_l9304b8 {
    void** vtable;

    void vf_0x198(u64 owner, UnkMeleeHit_l9304b8* hit, u64 target, s32 mode) {
        reinterpret_cast<void (*)(UnkMeleeHandler_l9304b8*, u64, UnkMeleeHit_l9304b8*, u64, s32)>(
            vtable[0x198 / 8])(this, owner, hit, target, mode);
    }
};

struct UnkMeleeTarget_l9304b8 {
    u8 pad_0x0[0x258];
    UnkMeleeHandler_l9304b8* field_0x258;
};

struct UnkMeleeComponent_l9304b8 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

bool fn_71009304b8(UnkMeleeComponent_l9304b8* self, UnkMeleeTarget_l9304b8* target, UnkMeleeHit_l9304b8* hit) {
    bool sent = false;
    if (hit && target->field_0x258) {
        UnkMeleeHit_l9304b8 reversed(*hit);
        reversed.Reverse();
        target->field_0x258->vf_0x198(self->field_0x30, &reversed,
                                      reversed.to.ptr ? reversed.to.ptr->field_0x10 : 0, 2);
        sent = true;
    }
    return sent;
}
