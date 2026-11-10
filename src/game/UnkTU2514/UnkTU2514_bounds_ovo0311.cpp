#include "types.h"

struct UnkBox6_b0217 {
    f32 min_x, min_y, min_z;
    f32 max_x, max_y, max_z;
};

struct UnkShapeItem2_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBox6_b0217 vf_0x28(const void* transform);
};

struct UnkShapeList2_b0217 {
    u8 pad_0x0[0xa8];
    u32 count;                      // 0xa8
    u8 items_raw[0x20];             // 0xac: four pointers, not 8-aligned
};

UnkBox6_b0217 fn_7100d6211c(UnkShapeList2_b0217* self, const void* transform) {
    UnkBox6_b0217 r;
    __builtin_memset(&r, 0, sizeof(r));
    if (self->count) {
        r.min_x = 3.4028235e38f;
        r.min_y = 3.4028235e38f;
        r.min_z = 3.4028235e38f;
        r.max_x = -3.4028235e38f;
        r.max_y = -3.4028235e38f;
        r.max_z = -3.4028235e38f;
        UnkShapeItem2_b0217** items = reinterpret_cast<UnkShapeItem2_b0217**>(self->items_raw);
        for (u64 i = 0; i < self->count; i++) {
            UnkBox6_b0217 b = items[i]->vf_0x28(transform);
            r.min_x = r.min_x < b.min_x ? r.min_x : b.min_x;
            r.min_y = r.min_y < b.min_y ? r.min_y : b.min_y;
            r.min_z = r.min_z < b.min_z ? r.min_z : b.min_z;
            r.max_x = r.max_x > b.max_x ? r.max_x : b.max_x;
            r.max_y = r.max_y > b.max_y ? r.max_y : b.max_y;
            r.max_z = r.max_z > b.max_z ? r.max_z : b.max_z;
        }
    }
    return r;
}
