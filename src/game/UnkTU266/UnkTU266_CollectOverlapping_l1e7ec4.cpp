// Unit 266 (tools/tu_map.py, no class name): collects the entries of two lists whose spheres overlap the
// owner's own sphere (or are flagged to be taken anyway), each one checked by a visibility test when the
// graphics owner asks for it. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/UnkSphere.h"
#include "game/UnkTU308.h"
#include "types.h"

using base::global::CRntVector;

bool fn_7100216c2c(const UnkSphere* a, const UnkSphere* b);

struct UnkGfxOwner_l1e7ec4 {
    u8 pad_0x0[0x3183660];
    bool on;  // 0x3183660
};

bool fn_71001d70ac(UnkGfxOwner_l1e7ec4* self);

// A node with a sphere and a flag that takes it regardless of overlap.
struct UnkSphereNode_l1e7ec4 {
    u8 pad_0x0[0x20];
    UnkSphere sphere;  // 0x20
    bool always;       // 0x70
};

struct UnkNodeEntry_l1e7ec4 {
    u8 pad_0x0[0x10];
    UnkSphereNode_l1e7ec4* node;  // 0x10
};

struct UnkEntryOwner_l1e7ec4 {
    u8 pad_0x0[0x35c];
    bool active;  // 0x35c
};

struct UnkSphereEntry_l1e7ec4 {
    UnkEntryOwner_l1e7ec4* owner;  // 0x0
    u8 pad_0x8[0x40];
    UnkSphere sphere;              // 0x48
};

struct UnkVisibilityTest_l1e7ec4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30(const UnkSphere* sphere, void* view);
};

struct UnkCollector_l1e7ec4 {
    u8 pad_0x0[0x10];
    UnkSphereNode_l1e7ec4* node;              // 0x10
    u8 pad_0x18[0x20];
    UnkNodeEntry_l1e7ec4** nodeEntries;       // 0x38
    u32 nodeEntryCount;                       // 0x40
    u8 pad_0x44[4];
    UnkSphereEntry_l1e7ec4** sphereEntries;   // 0x48
    u32 sphereEntryCount;                     // 0x50
    u8 pad_0x54[0x7c];
    void* view;                               // 0xd0
    CRntVector<UnkNodeEntry_l1e7ec4*> nodesFound;     // 0xd8
    CRntVector<UnkSphereEntry_l1e7ec4*> spheresFound;  // 0x100
    UnkGfxOwner_l1e7ec4* gfx;                 // 0x128
    u8 pad_0x130[8];
    UnkVisibilityTest_l1e7ec4* visibility;    // 0x138
};
static_assert(__builtin_offsetof(UnkCollector_l1e7ec4, visibility) == 0x138, "layout");

// Copies where and how large `src` is, keeping this sphere's own offset.
inline void CopyPlacement(UnkSphere& dst, const UnkSphere& src) {
    dst.radius = src.radius;
    dst.pos.x = src.pos.x;
    dst.pos.y = src.pos.y;
    dst.pos.z = src.pos.z;
    dst.field_0x24 = src.field_0x24;
    dst.field_0x28 = src.field_0x28;
    dst.field_0x2c = src.field_0x2c;
    dst.base_radius = src.base_radius;
    dst.radius_fixed = src.radius_fixed;
    dst.cell[0] = src.cell[0];
    dst.cell[1] = src.cell[1];
    dst.cell[2] = src.cell[2];
    dst.field_0x4c = src.field_0x4c;
    dst.field_0x4d = src.field_0x4d;
}

void fn_71001e7ec4(UnkCollector_l1e7ec4* self) {
    UnkSphere bounds;
    fn_710021685c(&bounds);
    CopyPlacement(bounds, self->node->sphere);

    u32 nodeCount = self->nodeEntryCount;
    for (u32 i = 0; i < nodeCount; ++i) {
        UnkNodeEntry_l1e7ec4* entry = self->nodeEntries[i];
        UnkSphere sphere;
        fn_710021685c(&sphere);
        CopyPlacement(sphere, entry->node->sphere);
        bool inside = fn_7100216ca4(&bounds, &sphere);
        bool overlaps = fn_7100216c2c(&bounds, &sphere);
        if (inside || overlaps || entry->node->always) {
            if (!fn_71001d70ac(self->gfx) || self->gfx->on)
                self->nodesFound.PushBack(entry);
            else if (self->visibility->vf_0x30(&sphere, self->view))
                self->nodesFound.PushBack(entry);
        }
        fn_7100216934(reinterpret_cast<char*>(&sphere));
    }

    u32 sphereCount = self->sphereEntryCount;
    for (u32 i = 0; i < sphereCount; ++i) {
        UnkSphereEntry_l1e7ec4* entry = self->sphereEntries[i];
        if (entry == nullptr || !entry->owner->active)
            continue;
        if (!fn_7100216ca4(&bounds, &entry->sphere) && !fn_7100216c2c(&bounds, &entry->sphere))
            continue;
        if (!fn_71001d70ac(self->gfx) || self->gfx->on)
            self->spheresFound.PushBack(entry);
        else if (self->visibility->vf_0x30(&entry->sphere, self->view))
            self->spheresFound.PushBack(entry);
    }
    fn_7100216934(reinterpret_cast<char*>(&bounds));
}
