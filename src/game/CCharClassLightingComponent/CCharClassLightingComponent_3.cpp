// CCharClassLightingComponent: forwards a value (fn_71008e8108) to each light in the array at +0x60
// whose definition (+0x8, +0x8, +0x30) is the given id, through its vtable slot 0x68; fn_71008e8090
// does the same with a flag through slot 0x58.
#include "types.h"

struct UnkLightDef {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightRes {
    u8 pad_0x0[0x8];
    UnkLightDef* def;
};

struct UnkLight {
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
    virtual void vf_0x58(bool flag);
    virtual void vf_0x60();
    virtual void vf_0x68(f32 value);
    UnkLightRes* res;
};

struct UnkLightingComponent {
    u8 pad_0x0[0x60];
    UnkLight** lights;
    u32 count;
};

void fn_71008e8108(UnkLightingComponent* self, const u64* id, f32 value) {
    for (UnkLight **it = self->lights, **end = it + self->count; it != end; ++it) {
        UnkLight* light = *it;
        UnkLightDef* def = light->res->def;
        u64 key = *id;
        if (def->id == key)
            light->vf_0x68(value);
    }
}

void fn_71008e8090(UnkLightingComponent* self, const u64* id, bool flag) {
    for (UnkLight **it = self->lights, **end = it + self->count; it != end; ++it) {
        UnkLight* light = *it;
        UnkLightDef* def = light->res->def;
        u64 key = *id;
        if (def->id == key)
            light->vf_0x58(flag);
    }
}
