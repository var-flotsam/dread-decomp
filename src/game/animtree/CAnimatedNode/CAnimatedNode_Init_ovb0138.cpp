// animtree::CAnimatedNode's unit (tools/tu_map.py): the node's set-up from its definition (+0x10,
// laid out as in src/game/animtree/CAnimatedNodeDef/CAnimatedNodeDef_Lifetime_ovb0138.cpp). It
// creates its bone filter (0x178 bytes, fn_710070d188; the old one is deleted), gives it the
// skeleton, a value from vtable slot 0x90 and the field at +0x68, points it at the owner's pose,
// copies the definition's two flags and adds every filtered bone and bone hierarchy (by name hash) to
// the mask at +0x28 (slots 0xc0 and 0xc8). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "types.h"

using base::global::CStrId;

// The bone filter the node creates (0x178 bytes).
struct UnkBoneFilter_ovb0138 {
    virtual void vf_0x0();
    virtual void Delete();  // 0x8
    u8 pad_0x8[0x18];
    void* field_0x20;
};
void fn_710070d188(UnkBoneFilter_ovb0138* self);
void fn_710070d7f0(UnkBoneFilter_ovb0138* self, void* skeleton);
void fn_710070d7e8(UnkBoneFilter_ovb0138* self, void* value);
void fn_710070d7e0(UnkBoneFilter_ovb0138* self, void* field);

struct UnkNodeOwner_ovb0138 {
    u8 pad_0x0[0x220];
    u8* field_0x220;
};

struct UnkAnimatedNodeDefView_ovb0138 {
    u8 pad_0x0[0x18];
    CStrId* bones;
    u32 bone_count;
    u8 pad_0x24[0x1c];
    CStrId* hierarchies;
    u32 hierarchy_count;
    u8 pad_0x4c[0x1c];
    bool launch_events;      // 0x68
    bool allow_root_motion;  // 0x69
};

struct UnkBoneMask_ovb0138 {
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
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void AddBone(s64 hash);       // 0xc0
    virtual void AddHierarchy(s64 hash);  // 0xc8
};

struct UnkAnimatedNode_ovb0138 {
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
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void* vf_0x90();

    u8 pad_0x8[8];
    UnkAnimatedNodeDefView_ovb0138* def;  // 0x10
    u8 pad_0x18[0x10];
    UnkBoneMask_ovb0138* mask;  // 0x28
    u8 pad_0x30[0x38];
    u8 field_0x68[8];
    UnkBoneFilter_ovb0138* filter;  // 0x70
    u8 pad_0x78[5];
    bool launch_events;      // 0x7d
    bool allow_root_motion;  // 0x7e
};
static_assert(__builtin_offsetof(UnkAnimatedNode_ovb0138, allow_root_motion) == 0x7e, "layout");

UnkNodeOwner_ovb0138* fn_71002dbb7c(UnkAnimatedNode_ovb0138* self);

// A copy of a string id that only lives for one call: retained, then released without being reset.
struct UnkBoneId_ovb0138 {
    base::global::UnkStringData* data;

    explicit UnkBoneId_ovb0138(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkBoneId_ovb0138() {
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool && data != &pool->empty) {
            if (--data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
    s64 Hash() const { return reinterpret_cast<base::global::UnkStringEntry*>(data)->buffer.key; }
};

void fn_71002d24cc(UnkAnimatedNode_ovb0138* self) {
    UnkBoneFilter_ovb0138* filter = static_cast<UnkBoneFilter_ovb0138*>(fn_71000839a0(0x178, 0x40));
    fn_710070d188(filter);
    UnkBoneFilter_ovb0138* old = self->filter;
    self->filter = filter;
    if (old)
        old->Delete();
    fn_710070d7f0(self->filter, fn_71002dbb7c(self));
    fn_710070d7e8(self->filter, self->vf_0x90());
    fn_710070d7e0(self->filter, self->field_0x68);
    UnkBoneFilter_ovb0138* f = self->filter;
    f->field_0x20 = fn_71002dbb7c(self)->field_0x220 + 0x80;
    UnkAnimatedNodeDefView_ovb0138* def = self->def;
    self->allow_root_motion = def->allow_root_motion;
    self->launch_events = def->launch_events;
    for (CStrId *it = def->bones, *end = it + def->bone_count; it != end; ++it) {
        UnkBoneId_ovb0138 id(*it);
        self->mask->AddBone(id.Hash());
    }
    def = self->def;
    for (CStrId *it = def->hierarchies, *end = it + def->hierarchy_count; it != end; ++it) {
        UnkBoneId_ovb0138 id(*it);
        self->mask->AddHierarchy(id.Hash());
    }
}
