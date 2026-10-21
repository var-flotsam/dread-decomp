// Fetches four values (e.g. edge margins) from the object at field_0x78, clamps each to be
// non-negative and stores them through fn_7100030728 (which clamps again).
#include "types.h"

// Four non-negative floats.
struct UnkStruct_71001c287c_Box {
    f32 v[4];
};

bool fn_71001997c8(void* obj, u32 a, u32 b, UnkStruct_71001c287c_Box* out, u32 mode);
void fn_7100030728(UnkStruct_71001c287c_Box* dst, const UnkStruct_71001c287c_Box* src);

struct UnkStruct_71001c287c {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

static inline f32 UnkClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

bool fn_71001c287c(UnkStruct_71001c287c* self, u32 a, u32 b, UnkStruct_71001c287c_Box* out, u32 mode) {
    UnkStruct_71001c287c_Box box = {};
    bool r = fn_71001997c8(self->field_0x78, a, b, &box, mode);
    UnkStruct_71001c287c_Box tmp;
    tmp.v[0] = UnkClampPositive(box.v[0]);
    tmp.v[1] = UnkClampPositive(box.v[1]);
    tmp.v[2] = UnkClampPositive(box.v[2]);
    tmp.v[3] = UnkClampPositive(box.v[3]);
    fn_7100030728(out, &tmp);
    return r;
}
