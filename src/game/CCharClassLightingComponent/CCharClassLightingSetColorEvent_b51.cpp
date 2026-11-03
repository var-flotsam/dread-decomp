// CCharClassLightingSetColorEvent unit (tools/tu_map.py, the name from reflection): unless its light id
// (+0x10) is the empty string, resolves the target's lighting component (fn_71008d5d2c on *target) and
// hands it the id's hash (+0x30 of the string data), the colour at +0x18 and the value at +0x28
// (fn_71008e7e98, CCharClassLightingComponent_2.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkUnit2186Object;
struct UnkLightingComponent2;
UnkUnit2186Object* fn_71008d5d2c(void* owner);
void fn_71008e7e98(UnkLightingComponent2* self, const u64* id, void* arg, f32 value);

struct UnkLightingColor_b51 {
    u64 rg;
    u32 b;
    u32 a;
};

struct UnkLightingSetColorEvent_b51 {
    u8 unk_0x0[0x10];
    base::global::UnkStringData* light;  // 0x10, a CStrId's data
    UnkLightingColor_b51 color;          // 0x18
    f32 value;                           // 0x28
};

void fn_7100bbd750(UnkLightingSetColorEvent_b51* self, void** target) {
    if (self->light == &g_unk_7101d47108->empty)
        return;
    UnkUnit2186Object* lighting = *target ? fn_71008d5d2c(*target) : nullptr;
    if (!lighting)
        return;
    u64 id = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(self->light) + 0x30);
    u64 rg = self->color.rg;
    u32 b = self->color.b;
    u32 a = self->color.a;
    UnkLightingColor_b51 color;
    color.rg = rg;
    color.b = b;
    color.a = a;
    fn_71008e7e98(reinterpret_cast<UnkLightingComponent2*>(lighting), &id, &color, self->value);
}
