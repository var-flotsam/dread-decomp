// CCharClassLightingComponent: two more per-light forwards like those of CCharClassLightingComponent_3.cpp:
// a flag to each light at +0x60 whose definition (+0x8, +0x8, +0x30) is the given id, through vtable
// slot 0x38 (fn_71008e7d20) or 0x48 (fn_71008e8018).
#include "types.h"

struct UnkLightDef2 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightRes2 {
    u8 pad_0x0[0x8];
    UnkLightDef2* def;
};

struct UnkLight2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(bool flag);
    virtual void vf_0x40();
    virtual void vf_0x48(bool flag);
    virtual void vf_0x50();
    virtual void vf_0x58(bool flag);
    virtual void vf_0x60();
    virtual void vf_0x68(f32 value);
    UnkLightRes2* res;
};
struct UnkLightingComponent2 {
    u8 pad_0x0[0x60];
    UnkLight2** lights;
    u32 count;
};

void fn_71008e7d20(UnkLightingComponent2* self, const u64* id, bool flag) {
    for (UnkLight2 **it = self->lights, **end = it + self->count; it != end; ++it) {
        UnkLight2* light = *it;
        UnkLightDef2* def = light->res->def;
        u64 key = *id;
        if (def->id == key)
            light->vf_0x38(flag);
    }
}

void fn_71008e8018(UnkLightingComponent2* self, const u64* id, bool flag) {
    for (UnkLight2 **it = self->lights, **end = it + self->count; it != end; ++it) {
        UnkLight2* light = *it;
        UnkLightDef2* def = light->res->def;
        u64 key = *id;
        if (def->id == key)
            light->vf_0x48(flag);
    }
}
