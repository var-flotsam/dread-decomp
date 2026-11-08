// In the CCharClassLightingComponent unit: more per-light functions over the array at +0x60 (as in
// CCharClassLightingComponent.cpp), the destructor body and the link-time cast. Names and layouts
// are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkComponent.h"

using base::reflection::UnkReflObject;

struct UnkLightingDesc3 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightingDescHolder3 {
    u8 pad_0x0[0x8];
    UnkLightingDesc3* desc;
};

struct UnkLightingLight3 {
    virtual void vf_0x0();
    virtual void Delete();  // the deleting destructor
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();  // releases what the light holds
    UnkLightingDescHolder3* holder;
};

struct UnkLightingComponent3 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x30];
    UnkStrIdPathResource field_0x48;
    UnkLightingLight3** lights;  // 0x60
    u32 count;                   // 0x68
    u8 pad_0x6c[0x1c];
    UnkReflObject* lighting;  // 0x88, the object as a CCharClassLightingComponent
};

void fn_71008e61f4(UnkLightingLight3* light, f32 a, f32 b);
void fn_710024f910(UnkLightingComponent3* self);  // the base destructor body

// Applies `a` and `b` (fn_71008e61f4) to each light whose definition is `id`.
void fn_71008e7f98(UnkLightingComponent3* self, const u64* id, f32 a, f32 b) {
    u32 count = self->count;
    if (count == 0)
        return;
    UnkLightingLight3** it = self->lights;
    u64 size = (u64)count * sizeof(*it);
    asm("" : "+r"(size));
    do {
        UnkLightingLight3* light = *it;
        UnkLightingDesc3* desc = light->holder->desc;
        u64 key = *id;
        if (desc->id == key)
            fn_71008e61f4(light, a, b);
        ++it;
        size -= sizeof(*it);
    } while (size != 0);
}

// The destructor body: drops the resource at +0x48, releases and deletes every light, empties the
// array, then runs the base (fn_710024f910).
void fn_71008e7bc0(UnkLightingComponent3* self) {
    fn_7100027044(&self->field_0x48);
    u32 count = self->count;
    if (count != 0) {
        UnkLightingLight3** it = self->lights;
        u64 size = (u64)count * sizeof(*it);
        asm("" : "+r"(size));
        do {
            UnkLightingLight3* light = *it;
            light->vf_0x20();
            if (light)
                light->Delete();
            ++it;
            size -= sizeof(*it);
        } while (size != 0);
    }
    self->count = 0;
    fn_710024f910(self);
}

void fn_71008e756c(UnkLightingComponent3* self, void* arg);
extern __attribute__((visibility("hidden"))) base::reflection::CClass g_unk_7172732948;  // its class

// Calls the base method; when the object at +0x10 is a CCharClassLightingComponent, caches it at
// +0x88 and passes its +0x58 to fn_71008e756c, else clears the cache.
bool fn_71008e74f0(UnkLightingComponent3* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* object = self->object;
    if (object) {
        const base::reflection::CClass* type = object->vf_0x0();
        fn_71008e6bd0();
        if (fn_710009d50c(type, &g_unk_7172732948)) {
            self->lighting = object;
            fn_71008e756c(self, reinterpret_cast<char*>(object) + 0x58);
            return result;
        }
    }
    self->lighting = nullptr;
    return result;
}
