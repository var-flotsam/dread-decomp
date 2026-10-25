// Loads the beam projectile models (power, plasma, wave and special burst): each model is created
// through the manager's pool at +0x218 from its path, registered with the manager, configured, and
// given a cleared 1000-byte buffer.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkModelData;

struct UnkModel {
    u8 pad_0x0[0x138];
    UnkModelData* field_0x138;
};

struct UnkBuffer {
    void* data;
    u64 field_0x8;
};

struct UnkProjectiles {
    u8 pad_0x0[0x88];
    u64 field_0x88[11];
    u8 pad_0xe0[0x20];
    UnkModel* field_0x100[7];
    u8 pad_0x138[0x10];
    UnkModel* field_0x148;
    u8 pad_0x150[0x48];
    UnkModel* field_0x198;
    UnkBuffer field_0x1a0[9];
    void* field_0x230;
    void* field_0x238;
};
static_assert(__builtin_offsetof(UnkProjectiles, field_0x148) == 0x148, "");
static_assert(__builtin_offsetof(UnkProjectiles, field_0x198) == 0x198, "");
static_assert(__builtin_offsetof(UnkProjectiles, field_0x230) == 0x230, "");

void fn_71006cae2c(UnkRenderPool* pool, const CFilePathStrId& path, UnkModel** out, base::global::CStrId* name);
void fn_7100219ae0(UnkRenderManager* manager, void* entry, u32 flag);
void fn_71001436a0(UnkModel* model, u32 flag);
void fn_71001c0968(UnkModelData* data, u32 flag);
void fn_7100ecf978(u64* field, u32 value);
void* fn_7100083ba0(u64 size, u32 align);
void fn_7100ed7634(UnkProjectiles* self);

__attribute__((always_inline)) static inline void LoadModel(UnkModel*& slot, const char* path) {
    UnkRenderManager* manager = g_unk_7101d47300;
    {
        UnkModel* model;
        CFilePathStrId id(path, true);
        model = nullptr;
        fn_71006cae2c(manager->field_0x218, id, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, model ? reinterpret_cast<char*>(model) + 8 : nullptr, 0);
        slot = model;
    }
    fn_71001436a0(slot, 0);
    fn_71001c0968(slot->field_0x138, 1);
}

__attribute__((always_inline)) static inline void* NewBuffer() {
    void* data = fn_7100083ba0(1000, 0x42);
    __builtin_memset(data, 0, 1000);
    return data;
}

void fn_7100ed4cb8(UnkProjectiles* self) {
    LoadModel(self->field_0x100[0], "actors/weapons/powerbeam/models/powerbeam_projectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[0], 1);
    self->field_0x1a0[0].data = NewBuffer();
    LoadModel(self->field_0x100[1], "actors/weapons/powerbeam/models/powerbeamcharged_projectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[1], 1);
    self->field_0x1a0[1].data = NewBuffer();
    LoadModel(self->field_0x100[2], "actors/weapons/plasmabeam/fx/plasmabeam_projectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[2], 1);
    self->field_0x1a0[2].data = NewBuffer();
    LoadModel(self->field_0x100[3], "actors/weapons/plasmabeam/fx/plasmabeam_chargedprojectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[3], 1);
    self->field_0x1a0[3].data = NewBuffer();
    LoadModel(self->field_0x100[4], "actors/weapons/wavebeam/fx/wavebeam_projectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[4], 1);
    self->field_0x1a0[4].data = NewBuffer();
    LoadModel(self->field_0x100[5], "actors/weapons/wavebeam/fx/wavebeam_chargedprojectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[5], 1);
    self->field_0x1a0[5].data = NewBuffer();
    LoadModel(self->field_0x100[6], "actors/weapons/specialburst/models/specialburst_ball.bcmdl");
    fn_7100ecf978(&self->field_0x88[6], 1);
    self->field_0x1a0[6].data = NewBuffer();
    LoadModel(self->field_0x148, "actors/weapons/specialburst/models/specialburst_burstprojectile.bcmdl");
    fn_7100ecf978(&self->field_0x88[9], 2);
    self->field_0x230 = NewBuffer();
    LoadModel(self->field_0x198, "actors/weapons/specialburst/models/specialburst_bursttail.bcmdl");
    self->field_0x238 = NewBuffer();
    fn_7100ed7634(self);
}
