// Unit 294 of tools/tu_map.py (no name hint): collects the values (vf_0x88) of a source's nodes into a
// list: unless `all` is set, only the nodes inside all six planes of the owner's plane set (each told whether it
// is, through vf_0x60), with it, every node whose flag at 0xf5 is set. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/UnkSphere.h"
#include "game/UnkTU308.h"
#include "types.h"

struct UnkPlane_l20e960 {
    s32 nx;
    s32 ny;
    s32 nz;
    s32 pad_0xc;
    s64 d;
};
static_assert(sizeof(UnkPlane_l20e960) == 0x18, "layout");

// A sphere's quantised bounds as the plane test reads them (UnkSphere's layout).
struct UnkCullBounds_l20e960 {
    u8 pad_0x0[0x38];
    s64 limit;           // 0x38
    s32 x;               // 0x40
    s32 y;               // 0x44
    s32 z;               // 0x48
    bool always_inside;  // 0x4c
    u8 plane;            // 0x4d: the plane to test first
};

struct UnkPlaneSet_l20e960 {
    u8 pad_0x0[0x1d8];
    UnkPlane_l20e960 planes[6];  // 0x1d8
};

// Whether the bounds lie inside all six planes, starting from the one that rejected them last.
static bool PlanesContain(const UnkPlaneSet_l20e960* set, UnkCullBounds_l20e960* b) {
    if (b->always_inside)
        return true;
    u8 plane = b->plane;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane_l20e960& p = set->planes[plane];
        s32 dot = b->x * p.nx + b->y * p.ny + b->z * p.nz;
        if (p.d + dot > b->limit)
            return false;
        u8 next = plane + 1;
        if (next == 6)
            next = 0;
        plane = next;
        b->plane = plane;
    }
    return true;
}

struct UnkPlaneOwner_l20e960 {
    u8 pad_0x0[0x108];
    UnkPlaneSet_l20e960* planes;  // 0x108
};

struct UnkCuller_l20e960 {
    void* vtable;
    UnkPlaneOwner_l20e960* owner;  // 0x8
};

struct UnkCullNode_l20e960 {
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
    virtual void vf_0x58();
    virtual void SetVisible(bool visible);  // 0x60
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void* GetValue();  // 0x88

    u8 pad_0x8[0x78];
    UnkCullBounds_l20e960 bounds;  // 0x80
    u8 pad_0xd0[0x24];
    bool hidden;     // 0xf4
    bool collect;    // 0xf5
    u8 pad_0xf6;
    bool cullable;   // 0xf7
};
static_assert(__builtin_offsetof(UnkCullNode_l20e960, bounds) == 0x80, "layout");
static_assert(__builtin_offsetof(UnkCullNode_l20e960, cullable) == 0xf7, "layout");

struct UnkCullSource_l20e960 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual UnkCullNode_l20e960** GetNodes(u32* count);  // 0x68
};

void fn_710020e960(UnkCuller_l20e960* self, UnkCullSource_l20e960* source, base::global::CRntVector<void*>* out,
                   bool all) {
    u32 count = 0;
    UnkCullNode_l20e960** nodes = source->GetNodes(&count);
    UnkSphere sphere;
    fn_710021685c(&sphere);
    out->Clear();
    if (all) {
        for (u32 i = 0; i < count; ++i) {
            if (nodes[i]->collect)
                out->PushBack(nodes[i]->GetValue());
        }
    } else {
        UnkPlaneSet_l20e960* set = self->owner->planes;
        for (UnkCullNode_l20e960 **p = nodes, **end = nodes + count; p != end; ++p) {
            UnkCullNode_l20e960* node = *p;
            if (!node->cullable) {
                node->SetVisible(true);
                if (!node->hidden)
                    out->PushBack(node->GetValue());
            } else {
                bool visible = PlanesContain(set, &node->bounds);
                node->SetVisible(visible);
                if (visible && !node->hidden)
                    out->PushBack(node->GetValue());
            }
        }
    }
    fn_7100216934(reinterpret_cast<char*>(&sphere));
}
