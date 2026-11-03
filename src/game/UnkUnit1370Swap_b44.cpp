// A swap of a blend's two ends in tools/tu_map.py unit 1370 (no class name known): the words at
// +0x60 and +0x98, the two refcounted handles at +0x8 and +0x10 (through fn_710031d208 and a
// counted copy), the vectors at +0x78 and +0x84, and the weight at +0x90 becomes 1 - weight. Names
// and layout are placeholders.
#include "base/UnkRefHandle.h"
#include "types.h"

// A counted reference; the destructor decrements and tests in one step.
struct UnkRef_b44a2 {
    base::UnkRefCounted* ptr;
    ~UnkRef_b44a2() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// Copied member by member.
struct UnkBlendVec3_b44a2 {
    f32 x, y, z;
    UnkBlendVec3_b44a2(const UnkBlendVec3_b44a2& o) : x(o.x), y(o.y), z(o.z) {}
    UnkBlendVec3_b44a2& operator=(const UnkBlendVec3_b44a2& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkBlend_b44a2 {
    u8 pad_0x0[0x8];
    base::UnkRefHandle field_0x8;
    base::UnkRefHandle field_0x10;
    u8 pad_0x18[0x48];
    u64 field_0x60;
    u8 pad_0x68[0x10];
    UnkBlendVec3_b44a2 field_0x78;
    UnkBlendVec3_b44a2 field_0x84;
    f32 field_0x90;
    u32 pad_0x94;
    u64 field_0x98;
};

void fn_71008c16e0(UnkBlend_b44a2* self) {
    u64 w = self->field_0x60;
    self->field_0x60 = self->field_0x98;
    self->field_0x98 = w;
    {
        base::UnkRefHandle* a = &self->field_0x8;
        base::UnkRefHandle* b = &self->field_0x10;
        UnkRef_b44a2 old;
        old.ptr = nullptr;
        if (base::UnkRefCounted* p = b->ptr) {
            ++p->ref_count;
            old.ptr = p;
        }
        fn_710031d208(b, a);
        fn_710031d208(a, reinterpret_cast<const base::UnkRefHandle*>(&old));
    }
    UnkBlendVec3_b44a2 v = self->field_0x78;
    self->field_0x78 = self->field_0x84;
    self->field_0x84 = v;
    self->field_0x90 = 1.0f - self->field_0x90;
}
