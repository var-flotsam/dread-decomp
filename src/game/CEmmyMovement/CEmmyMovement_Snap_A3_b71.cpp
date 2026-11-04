// CEmmyMovement's unit (tools/tu_map.py unit 814): takes the direction the object at +0x848 reports
// (vf_0xb0; down when there is none), asks fn_71010cd278 for a result within 30 units and, when it finds one,
// hands it to vf_0x218. The result holds two counted references, released in reverse order. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

// Returned in memory: not trivially copyable.
struct UnkEmmyMoveDir_A3_b71 {
    f32 x;
    f32 y;
    f32 z;
    UnkEmmyMoveDir_A3_b71() = default;
    UnkEmmyMoveDir_A3_b71(const UnkEmmyMoveDir_A3_b71& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkEmmyMoveSource_A3_b71 {
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
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual UnkEmmyMoveDir_A3_b71 vf_0xb0();
};

struct UnkEmmyMoveRefObj_A3_b71 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

// A counted reference whose count has type-based alias info (the pointer isn't reloaded).
struct UnkEmmyMoveRef_A3_b71 {
    UnkEmmyMoveRefObj_A3_b71* ptr = nullptr;
    ~UnkEmmyMoveRef_A3_b71() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkEmmyMoveHit_A3_b71 {
    UnkEmmyMoveRef_A3_b71 first;
    UnkEmmyMoveRef_A3_b71 second;
    s32 index = -1;
    u8 field_0x14[0x30] = {};
};

struct UnkEmmyMovement_A3_b71;
bool fn_71010cd278(UnkEmmyMovement_A3_b71* self, const base::math::CVector3D* dir, UnkEmmyMoveHit_A3_b71* hit,
                   void* filter, f32 range);

struct UnkEmmyMovement_A3_b71 {
    virtual void vf_0x0();
    u8 pad_0x8[0x848 - 8];
    UnkEmmyMoveSource_A3_b71* source;

    void Apply(UnkEmmyMoveHit_A3_b71* hit) {
        reinterpret_cast<void (*)(UnkEmmyMovement_A3_b71*, UnkEmmyMoveHit_A3_b71*)>(
            (*reinterpret_cast<void***>(this))[0x218 / 8])(this, hit);
    }
};

void fn_7100571808(UnkEmmyMovement_A3_b71* self) {
    UnkEmmyMoveHit_A3_b71 hit;
    base::math::CVector3D dir;
    dir.x = 0.0f;
    dir.y = -1.0f;
    dir.z = 0.0f;
    if (self->source) {
        UnkEmmyMoveDir_A3_b71 d = self->source->vf_0xb0();
        __builtin_memcpy(&dir, &d, sizeof(dir));
    }
    if (fn_71010cd278(self, &dir, &hit, nullptr, 30.0f))
        self->Apply(&hit);
}
