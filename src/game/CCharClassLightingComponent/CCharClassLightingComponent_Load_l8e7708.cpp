// CCharClassLightingComponent (unit 1384): rebuilds the component's lights. The old ones are shut down
// and deleted; when a lights definition (CLightsDef) is loaded, one light of the matching kind is
// created and initialised for each entry. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

// The four light kinds' vtables (at least the slots used here).
extern void* const g_unk_7101a7f6d0[5];
extern void* const g_unk_7101a7f958[5];
extern void* const g_unk_7101a7fad8[5];
extern void* const g_unk_7101a7fa18[5];

// One entry of the definition: its kind first.
struct UnkLightDef_l8e7708 {
    u32 kind;
    u8 pad_0x4[0x5c];
};

struct UnkLightsDef_l8e7708 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x58];
    base::global::CRntVector<UnkLightDef_l8e7708> lights;  // 0x60
};

struct UnkLight_l8e7708 {
    void* const* vtable;
    u8 pad_0x8[0x10];
    u32 kind;  // 0x18
    u8 field_0x1c[0x104];
    void* field_0x120;

    UnkLight_l8e7708(void* const* vt, u32 kind) : kind(kind), field_0x1c() {
        vtable = vt + 2;
        field_0x120 = nullptr;
    }

    void Init(void* owner, const UnkLightDef_l8e7708* def) {
        reinterpret_cast<void (*)(UnkLight_l8e7708*, void*, const UnkLightDef_l8e7708*)>(vtable[2])(this, owner, def);
    }
    void Shutdown() { reinterpret_cast<void (*)(UnkLight_l8e7708*)>(vtable[4])(this); }
    void Delete() { reinterpret_cast<void (*)(UnkLight_l8e7708*)>(vtable[1])(this); }
};

struct UnkLightingComponent_l8e7708 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x10];
    base::global::CStrId name;  // 0x48
    base::global::CFilePathStrId path;  // 0x50
    UnkLightsDef_l8e7708* def;  // 0x58
    base::global::CRntVector<UnkLight_l8e7708*> lights;  // 0x60
};
static_assert(__builtin_offsetof(UnkLightingComponent_l8e7708, lights) == 0x60, "layout");

static inline void AddLight(UnkLightingComponent_l8e7708* self, void* const* vtable, u32 kind,
                            const UnkLightDef_l8e7708* def) {
    UnkLight_l8e7708* light = new UnkLight_l8e7708(vtable, kind);
    light->Init(self->field_0x30, def);
    self->lights.PushBack(light);
}

void fn_71008e7708(UnkLightingComponent_l8e7708* self) {
    for (UnkLight_l8e7708* light : self->lights) {
        light->Shutdown();
        if (light)
            light->Delete();
    }
    self->lights.count = 0;

    if (self->name.data == &g_unk_7101d47108->empty || self->path.data == &g_unk_7101d47200->empty)
        return;
    UnkLightsDef_l8e7708* def = self->def;
    if (!def || !fn_710009d50c(def->vf_0x0(), fn_7100d9cf20()))
        return;

    self->lights.Reserve(def->lights.count);
    for (const UnkLightDef_l8e7708& entry : def->lights) {
        switch (entry.kind) {
        case 0:
            AddLight(self, g_unk_7101a7f6d0, 0, &entry);
            break;
        case 1:
            AddLight(self, g_unk_7101a7f958, 1, &entry);
            break;
        case 2:
            AddLight(self, g_unk_7101a7fad8, 2, &entry);
            break;
        case 3:
            AddLight(self, g_unk_7101a7fa18, 3, &entry);
            break;
        }
    }
}
