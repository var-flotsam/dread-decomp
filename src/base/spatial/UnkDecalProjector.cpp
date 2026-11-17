// From unit 246 of tools/tu_map.py (0x71001bb7cc-0x71001bbb3c, no reflected class): a 0x90-byte
// object built like the view of UnkFrustumCull.cpp (the same header words and bounding sphere) that
// owns a 0x88-byte object entered in the same registry (g_unk_716e34c2e0) and given the
// "mp_prj_blend_decal000" resource, which suggests a projected decal. fn_7100151090 allocates it and
// calls the constructor, fn_71001bb7cc. Names and layouts are placeholders.
#include <new>
#include <utility>

#include "base/global/CRntDictionary.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkSphere.h"
#include "game/UnkTU204.h"
#include "game/UnkTU211.h"
#include "game/UnkTU228.h"
#include "game/UnkTU308.h"
#include "types.h"

using base::global::CFilePathStrId;

// The registry the projectors' objects are entered in: a word, then a set of the objects (each its own
// value) at +0x8.
struct UnkProjectorRegistry {
    u8 pad_0x0[0x8];
    base::global::CRntDictionary<u64, u64> objects;  // +0x8
};

// The resource the decal draws with: loaded by path from the manager g_unk_716e352450 and released
// to it.
struct UnkDecalResource {
    u8 pad_0x0[0x18];
    s32 field_0x18;
};
struct UnkDecalResourceMgr;

// The 0x88-byte object: its resource and whether that resource's word at +0x18 is not 1.
struct UnkDecal {
    u8 pad_0x0[0x20];
    UnkDecalResource* resource;  // +0x20
    u8 pad_0x28[0x44];
    u32 field_0x6c;
    u8 pad_0x70[0x18];
};
static_assert(sizeof(UnkDecal) == 0x88, "UnkDecal layout");
void fn_710019a2b8(UnkDecal* self);  // its constructor

struct UnkDecalProjector {
    u32 field_0x0;  // 0x10000 once built (8 for the view of UnkFrustumCull.cpp)
    u32 field_0x4;
    s32 field_0x8;
    u32 field_0xc;
    UnkDecalProjector* field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    UnkSphere sphere;  // 0x20
    bool field_0x70;
    UnkDecal* decal;  // 0x78, entered in g_unk_716e34c2e0
    u64 field_0x80;
    u16 field_0x88;
};
static_assert(sizeof(UnkDecalProjector) == 0x90, "UnkDecalProjector layout");

// The constructor: defaults and the sphere, a new decal entered in the registry, then the decal's
// resource.
void fn_71001bb7cc(UnkDecalProjector* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->field_0x10 = nullptr;
    fn_710021685c(&self->sphere);
    self->decal = nullptr;
    self->field_0x70 = false;
    self->field_0x80 = 0;
    self->field_0x88 = 1;
    auto* registry = static_cast<UnkProjectorRegistry*>(g_unk_716e34c2e0);
    auto* decal = static_cast<UnkDecal*>(fn_71000839a0(sizeof(UnkDecal), 0x32));
    fn_710019a2b8(decal);
    if (decal)
        registry->objects.InsertChecked((u64)decal, (u64)decal);
    self->decal = decal;
    static CFilePathStrId s_path("mp_prj_blend_decal000", true);
    if (UnkDecalResource* resource = fn_710019ffe8(g_unk_716e352450, &s_path, true)) {
        // Swapped through a reference, so the store may alias the decal pointer (it is read again).
        UnkDecalResource* old = std::exchange(self->decal->resource, resource);
        self->decal->field_0x6c = resource->field_0x18 != 1;
        if (old)
            fn_71001a080c(g_unk_716e352450, old);
    }
    self->field_0x0 = 0x10000;
    self->field_0x4 = 1;
    self->field_0x10 = self;
}
