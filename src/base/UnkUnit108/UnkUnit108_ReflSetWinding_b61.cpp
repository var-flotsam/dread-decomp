// Unit 108 (tools/tu_map.py, no name hint): a reflected function's invoker (fn_710011e40c) that sets
// a polygon's winding from a bool argument (state at +0x40: 1 one way, 2 the other). When the state
// changes and the polygon is flagged at +0x2c, the signed area of its 12-byte points (x at +0, y at
// +4) decides whether they are reversed first (fn_7100118c78). Answers the empty status {0, 0, 2, 1}.
// Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkPoint_b61 {
    float x;
    float y;
    float z;
};

struct UnkPolygon_b61 {
    UnkPoint_b61* points;
    u32 count;
    u8 pad_0xc[0x20];
    bool field_0x2c;
    u8 pad_0x2d[0x13];
    u32 winding;  // 0x40

    float SignedArea() const {
        float area = 0.0f;
        for (u32 i = 0; i < count; ++i) {
            u32 j = i < count - 1 ? i + 1 : 0;
            UnkPoint_b61 p = points[i];
            UnkPoint_b61 q = points[j];
            area += (q.x - p.x) * 0.5f * (p.y + q.y);
        }
        return area;
    }
};

void fn_7100118c78(UnkPolygon_b61* self);

static inline __attribute__((always_inline)) void* ArgPtr_b61(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710011e40c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkPolygon_b61**>(obj + 8);
    u8 ccw = *static_cast<const u8*>(ArgPtr_b61(&a[0]));
    if (ccw ? self->winding != 2 : self->winding != 1) {
        if (self->field_0x2c) {
            if ((ccw != 0) != (self->SignedArea() >= 0.0f))
                fn_7100118c78(self);
        }
        self->winding = ccw ? 2 : 1;
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
