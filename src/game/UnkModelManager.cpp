// The constructor of the manager that owns the debug-overlay models (UnkModelManager.h): it sets
// up two chained maps and two pooled containers, then loads the seven models from the file-path
// pool, puts each on layer 0 of the render manager and enables it.
#include "base/global/CFilePathStrId.h"
#include "game/UnkModelManager.h"

using base::global::CFilePathStrId;

void fn_71011aad4c(UnkChainedMap* map, u32 a, u32 b);
void fn_71011a8d30(UnkModelManagerPool_0x70* p, u32 a, u32 b, u32 c, u64 d);
void fn_71011a8e48(UnkModelManagerPool_0x70* p, u32 a, u32 b, u32 c, u64 d);
void fn_7100a0feac(UnkRenderPool* pool, CFilePathStrId* path, UnkOverlayModel** out, base::global::CStrId* id);
void fn_7100145818(UnkOverlayModel* model, u32 flag);

static inline void LoadModel(UnkOverlayModel*& slot, const char* path) {
    UnkRenderManager* render = g_unk_7101d47300;
    UnkOverlayModel* model;
    CFilePathStrId id(path, true);
    model = nullptr;
    fn_7100a0feac(render->field_0x218, &id, &model, &g_unk_7101d470e0);
    fn_7100219ae0(render, model ? &model->node : nullptr, 0);
    slot = model;
}

void fn_71011a870c(UnkModelManager* self) {
    for (UnkOverlayModel*& m : self->models)
        m = nullptr;
    new (&self->map_a) UnkChainedMap();
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    fn_71011aad4c(&self->map_a, 0x400, 0x400);
    fn_71011a8d30(&self->field_0xc0, 0x400, 0, 0x13, 0);
    new (&self->map_b) UnkChainedMap();
    self->map_b.ResizeBuckets(11);
    fn_71011a8e48(&self->field_0x178, 0x400, 0, 0x13, 0);
    LoadModel(self->models[0], "system/engine/models/damage.bcmdl");
    LoadModel(self->models[1], "system/engine/models/damagehard.bcmdl");
    LoadModel(self->models[2], "system/engine/models/selected_hi.bcmdl");
    LoadModel(self->models[3], "system/engine/models/selected_lo.bcmdl");
    LoadModel(self->models[4], "system/fx/custommaterials/immune.bcmdl");
    LoadModel(self->models[5], "system/engine/models/sedisolve.bcmdl");
    LoadModel(self->models[6], "system/engine/models/sedisolver.bcmdl");
    for (UnkOverlayModel* m : self->models)
        fn_7100145818(m, 1);
}
