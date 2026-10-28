// Initialises an entity's two smart-link group handles from the navmesh manager's registry
// (game/UnkNavMeshManager.h; the manager is created on first use), runs two virtual set-up
// steps and stores the squares of two floats.
#include "base/UnkVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkModelEntity {
    void** vtable;
    u8 pad_0x8[0x1c8];
    void* field_0x1d0;
    u8 field_0x1d8[8];
    void* field_0x1e0;
    u8 field_0x1e8[8];
    u8 pad_0x1f0[0x188];
    f32 field_0x378;
    f32 field_0x37c;
    f32 field_0x380;
    f32 field_0x384;

    void vf_0xc8() { reinterpret_cast<void (*)(UnkModelEntity*)>(vtable[0xc8 / 8])(this); }
    void vf_0xd0() { reinterpret_cast<void (*)(UnkModelEntity*)>(vtable[0xd0 / 8])(this); }
};

void* fn_7100ec0d20(UnkNavMeshManager* manager, void* key);
bool fn_71011ace94(UnkModelEntity* self);

static inline UnkNavMeshManager* GetRegistry() {
    UnkNavMeshManager* registry = g_unk_71726a4510;
    if (!registry) {
        registry = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(registry);
        g_unk_71726a4510 = registry;
    }
    return registry;
}

bool fn_7100413ef8(UnkModelEntity* self) {
    bool result = fn_71011ace94(self);
    self->field_0x1d0 = fn_7100ec0d20(GetRegistry(), self->field_0x1d8);
    self->field_0x1e0 = fn_7100ec0d20(GetRegistry(), self->field_0x1e8);
    self->vf_0xc8();
    self->vf_0xd0();
    self->field_0x378 = self->field_0x380 * self->field_0x380;
    self->field_0x37c = self->field_0x384 * self->field_0x384;
    return result;
}
