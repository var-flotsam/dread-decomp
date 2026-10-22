// Initialises an entity's two model handles from the global model registry (created on first
// use), runs two virtual set-up steps and stores the squares of two floats.
#include "types.h"

struct UnkModelRegistry;

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

extern UnkModelRegistry* g_unk_71726a4510;
void* fn_71000839a0(u64 size, u32 flags);
void fn_7100ebf278(UnkModelRegistry* registry);
void* fn_7100ec0d20(UnkModelRegistry* registry, void* arg);
bool fn_71011ace94(UnkModelEntity* self);

static inline UnkModelRegistry* GetRegistry() {
    UnkModelRegistry* registry = g_unk_71726a4510;
    if (!registry) {
        registry = static_cast<UnkModelRegistry*>(fn_71000839a0(0x62ec8, 0x3f));
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
