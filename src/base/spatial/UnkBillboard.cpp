// From unit 243 of tools/tu_map.py (0x71001b4aac-0x71001b76ec, no reflected class): a renderable
// built like the cull view of UnkFrustumCull.cpp and the projector of UnkDecalProjector.cpp (the
// same header words and bounding sphere) that draws with the "mp_billboardmtl00" material, which
// suggests a billboard. fn_7100151110 allocates it and calls the constructor, fn_71001b4aac. Names
// and layouts are placeholders.
#include <string.h>

#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkSphere.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The material: loaded by path from the resource manager g_unk_716e352450 and released to it (the
// declarations are src/base/spatial/UnkDecalProjector.cpp's, which sees the same resources).
struct UnkDecalResource;
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
UnkDecalResource* fn_710019ffe8(UnkDecalResourceMgr* mgr, const CFilePathStrId* path, bool flag);
void fn_71001a080c(UnkDecalResourceMgr* mgr, UnkDecalResource* resource);
struct UnkBillboardMaterial {
    u8 pad_0x0[0x20];
    CStrId field_0x20;
    u8 pad_0x28[0x368];
    u64 field_0x390;
};

// The identity matrix (a constant in .data, reached through the GOT).
extern UnkMatrix44 g_unk_7101a1a680;

// The drawable at +0x80 (constructor fn_710019a4e8): fn_710019b184 sets the matrix of one of its
// parts.
struct UnkBillboardDrawable {
    u8 pad_0x0[0x128];
};
void fn_710019b184(UnkBillboardDrawable* self, u32 index, u32 part, const UnkMatrix44* matrix);

// The part at +0x1b0 (fn_7100196e64 builds it over the drawable): the drawable, the material and a
// word set to 1 once the material is loaded.
struct UnkBillboardPart {
    UnkBillboardDrawable* drawable;
    UnkBillboardMaterial* material;
    void* field_0x10;
    u32 field_0x18;
    u16 field_0x1c;
};
static_assert(sizeof(UnkBillboardPart) == 0x20, "UnkBillboardPart placeholder layout");
void fn_7100196e64(UnkBillboardPart* self, UnkBillboardDrawable* drawable, void* a, void* b);
void fn_710019a4e8(UnkBillboardDrawable* self, u32 a, u32 b);  // the drawable's constructor
void fn_710021685c(void* self);  // UnkSphere's constructor

struct UnkColor4 {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

struct UnkBillboard {
    u32 field_0x0;  // 0x800 once built
    u32 field_0x4;  // 1 once built
    s32 field_0x8;
    u32 field_0xc;
    UnkBillboard* field_0x10;  // itself once built
    u32 field_0x18;
    u32 field_0x1c;
    UnkSphere sphere;  // 0x20
    bool field_0x70;
    u64 field_0x78;
    UnkBillboardDrawable drawable;  // +0x80
    UnkBillboardMaterial* material;  // +0x1a8
    UnkBillboardPart part;           // +0x1b0
    u8 block_0x1d0[0x5d];  // zeroed as a block (fn_71001b7550 sets the word at +0x1dc)
    u8 pad_0x22d[0x3];
    u64 field_0x230[8];
    u8 block_0x270[0xd];  // zeroed as a block
    u8 pad_0x27d[0x3];
    u32 field_0x280;
    f32 field_0x284[6];
    u8 pad_0x29c[0x4];
    u64 field_0x2a0;
    u32 field_0x2a8;
    f32 field_0x2ac[7];
    u32 field_0x2c8;
    u32 field_0x2cc[5];
    u64 field_0x2e0;  // the material's +0x390
    CStrId field_0x2e8;  // the material's id
    u64 field_0x2f0;
    u32 field_0x2f8;
    UnkColor4 field_0x2fc;
    UnkColor4 field_0x30c;
    UnkColor4 field_0x31c;
    f32 field_0x32c[6];
    f32 field_0x344[5];
    u64 field_0x358;
    u64 field_0x360;
    u64 field_0x368;
    bool field_0x370;
    bool field_0x371;
    u16 field_0x372;
    u16 field_0x374;

    // Takes the material's id and its word at +0x390.
    void Refresh() {
        field_0x2e0 = material->field_0x390;
        field_0x2e8 = material->field_0x20;
    }
    void SetMaterial(UnkBillboardMaterial* m) {
        material = m;
        part.material = m;
        Refresh();
    }
};
static_assert(__builtin_offsetof(UnkBillboard, material) == 0x1a8, "UnkBillboard placeholder layout");
static_assert(__builtin_offsetof(UnkBillboard, field_0x230) == 0x230, "UnkBillboard placeholder layout");
static_assert(__builtin_offsetof(UnkBillboard, field_0x2a0) == 0x2a0, "UnkBillboard placeholder layout");
static_assert(__builtin_offsetof(UnkBillboard, field_0x2e8) == 0x2e8, "UnkBillboard placeholder layout");
static_assert(__builtin_offsetof(UnkBillboard, field_0x358) == 0x358, "UnkBillboard placeholder layout");
static_assert(sizeof(UnkBillboard) == 0x378, "UnkBillboard placeholder layout");

// Loads the "mp_billboardmtl00" material (releasing the one held before), resets the drawable's
// first matrix to the identity and marks the part.
void fn_71001b4de8(UnkBillboard* self) {
    static CFilePathStrId s_material("mp_billboardmtl00", true);
    UnkBillboardMaterial* old = self->material;
    self->SetMaterial(reinterpret_cast<UnkBillboardMaterial*>(fn_710019ffe8(g_unk_716e352450, &s_material, true)));
    if (old)
        fn_71001a080c(g_unk_716e352450, reinterpret_cast<UnkDecalResource*>(old));
    self->Refresh();
    fn_710019b184(&self->drawable, 0, 0, &g_unk_7101a1a680);
    self->part.field_0x18 = 1;
}

// Values the billboards share, taken from the graphics system by the first one built (GOT-reached
// globals of the unit): a count, a pointer and the count divided by six.
extern u32 g_unk_716e5546f4;
extern u32 g_unk_716e5546f8;
extern void* g_unk_716e554700;
struct UnkGraphicsCounts {
    u8 pad_0x0[0x317af48];
    u32 field_0x317af48;
    u8 pad_0x317af4c[0x4];
    void* field_0x317af50;
};

// The empty id (the pool created first if there is none yet).
static inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The constructor: the header words, the sphere, the drawable and its part, zeroed state with a few
// scales of 1, the empty material id, default colours, then the shared values on first use.
// NON_MATCHING(minor): the stores between +0x1d0 and +0x2ef (after the part is built) and those
// after the empty id come in another order and are paired and merged differently (for example
// +0x2ac..+0x2ef from one base register in the original); between each pair of calls the same
// bytes get the same values (660 B original, 664 B here).
void fn_71001b4aac(UnkBillboard* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->field_0x10 = nullptr;
    fn_710021685c(&self->sphere);
    self->field_0x70 = false;
    self->field_0x78 = 0;
    fn_710019a4e8(&self->drawable, 1, 1);
    self->material = nullptr;
    fn_7100196e64(&self->part, &self->drawable, nullptr, nullptr);
    memset(self->block_0x1d0, 0, sizeof(self->block_0x1d0));
    for (u64& w : self->field_0x230)
        w = 0;
    memset(self->block_0x270, 0, sizeof(self->block_0x270));
    self->field_0x280 = 0;
    self->field_0x284[0] = 0.0f;
    self->field_0x284[1] = 0.0f;
    self->field_0x284[2] = 1.0f;
    self->field_0x284[3] = 0.0f;
    self->field_0x284[4] = 0.0f;
    self->field_0x284[5] = 0.0f;
    self->field_0x2a0 = 0;
    self->field_0x2a8 = 0;
    self->field_0x2ac[0] = 1.0f;
    self->field_0x2ac[1] = 1.0f;
    self->field_0x2ac[2] = 0.0f;
    self->field_0x2ac[3] = 0.0f;
    self->field_0x2ac[4] = 0.0f;
    self->field_0x2ac[5] = 0.0f;
    self->field_0x2ac[6] = 0.0f;
    self->field_0x2c8 = 1;
    for (u32& w : self->field_0x2cc)
        w = 0;
    self->field_0x2e0 = 0;
    InitEmpty(&self->field_0x2e8);
    self->field_0x2f0 = 0;
    self->field_0x2f8 = 5;
    self->field_0x2fc = {1.0f, 1.0f, 1.0f, 1.0f};
    self->field_0x30c = {0.5f, 0.5f, 0.5f, 0.8f};
    self->field_0x31c = {1.0f, 1.0f, 1.0f, 1.0f};
    for (f32& f : self->field_0x32c)
        f = 0.0f;
    self->field_0x344[0] = 1.0f;
    self->field_0x344[1] = 0.0f;
    self->field_0x344[2] = 1.0f;
    self->field_0x344[3] = 0.0f;
    self->field_0x344[4] = 1.0f;
    self->field_0x358 = 0;
    self->field_0x360 = 0;
    self->field_0x368 = 0;
    self->field_0x370 = false;
    self->field_0x371 = true;
    self->field_0x372 = 0;
    self->field_0x374 = 0;
    self->field_0x0 = 0x800;
    self->field_0x4 = 1;
    self->field_0x10 = self;
    if (!g_unk_716e554700) {
        auto* gfx = reinterpret_cast<UnkGraphicsCounts*>(g_unk_710b372a98);
        u32 count = gfx->field_0x317af48;
        g_unk_716e5546f4 = count;
        g_unk_716e554700 = gfx->field_0x317af50;
        g_unk_716e5546f8 = count / 6;
    }
}
