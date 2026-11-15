// Unit 2563 (CMinimapData hint): constructor of a record of an empty string id, a vtable pointer, five
// empty arrays (flags 0xf) and two empty 2D bounds boxes. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

struct UnkBoundsBox_e50c98 {
    f32 min_x = 3.4028235e+38f;
    f32 min_y = 3.4028235e+38f;
    f32 max_x = -3.4028235e+38f;
    f32 max_y = -3.4028235e+38f;
};

struct UnkLayerRecord_e50c98 {
    base::global::UnkStringData* id;  // 0x0
    u32 field_0x8;
    void** field_0x10;
    base::global::CRntVector<void*> first;  // 0x18
    bool field_0x40;
    u8 pad_0x41[3];
    u32 field_0x44[6];
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64[4];
    u32 field_0x74;
    base::global::CRntVector<void*> second;  // 0x78
    base::global::CRntVector<void*> third;   // 0xa0
    UnkBoundsBox_e50c98 box_a;      // 0xc8
    base::global::CRntVector<void*> fourth;  // 0xd8
    base::global::CRntVector<void*> fifth;   // 0x100
    UnkBoundsBox_e50c98 box_b;      // 0x128
};
static_assert(sizeof(UnkLayerRecord_e50c98) == 0x138, "placeholder layout");

void fn_7100e50c98(UnkLayerRecord_e50c98* self) {
    self->id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x8 = 0;
    self->id = &g_unk_7101d47108->empty;
    self->field_0x10 = g_unk_7101cac938 + 2;
    new (&self->first) base::global::CRntVector<void*>(0xf);
    self->field_0x40 = true;
    for (u32& w : self->field_0x44)
        w = 0;
    for (u32& w : self->field_0x64)
        w = 0;
    new (&self->second) base::global::CRntVector<void*>(0xf);
    new (&self->third) base::global::CRntVector<void*>(0xf);
    new (&self->fourth) base::global::CRntVector<void*>(0xf);
    new (&self->fifth) base::global::CRntVector<void*>(0xf);
    new (&self->box_b) UnkBoundsBox_e50c98();
}
