// CCharClassLightingAttParamsEvent unit (tools/tu_map.py, the name from reflection): the same code as
// fn_7100bbd750 (CCharClassLightingSetColorEvent_b51.cpp), handing the light's hash, the parameters at +0x18
// and the value at +0x28 to fn_71008e7f18 (CCharClassLightingComponent.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkUnit2186Object;
struct UnkLightingComponent;
UnkUnit2186Object* fn_71008d5d2c(void* owner);
void fn_71008e7f18(UnkLightingComponent* self, const u64* id, void* arg, f32 value);

struct UnkLightingAttParams_A6_b63 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkLightingAttParamsEvent_A6_b63 {
    u8 unk_0x0[0x10];
    base::global::UnkStringData* light;  // 0x10, a CStrId's data
    UnkLightingAttParams_A6_b63 params;  // 0x18
    f32 value;                           // 0x28
};

void fn_7100bbb590(UnkLightingAttParamsEvent_A6_b63* self, void** target) {
    if (self->light == &g_unk_7101d47108->empty)
        return;
    UnkUnit2186Object* lighting = *target ? fn_71008d5d2c(*target) : nullptr;
    if (!lighting)
        return;
    u64 id = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(self->light) + 0x30);
    u64 p0 = self->params.field_0x0;
    u32 p1 = self->params.field_0x8;
    u32 p2 = self->params.field_0xc;
    UnkLightingAttParams_A6_b63 params;
    params.field_0x0 = p0;
    params.field_0x8 = p1;
    params.field_0xc = p2;
    fn_71008e7f18(reinterpret_cast<UnkLightingComponent*>(lighting), &id, &params, self->value);
}
