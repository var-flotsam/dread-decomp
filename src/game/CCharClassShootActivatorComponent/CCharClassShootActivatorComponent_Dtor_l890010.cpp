// CCharClassShootActivatorComponent unit: the shoot activator's destructor (fn_7100890010, the one
// fn_710088fce4 constructs for): clears its four arrays, destroys its members last first, then runs the
// base destructor (fn_71008e05e8). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CItemLifeComponent.h"
#include "types.h"

using base::global::CRntVector;

struct UnkShootActivatorBase_b54;
struct UnkClearVector_A4_b75;

extern void* g_unk_7101a7c540[];  // the component's vtable

void fn_71008904b0(UnkClearVector_A4_b75* self);

// An element of the first two arrays: destroyed through its vtable.
struct UnkShootCallback_l890010 {
    virtual ~UnkShootCallback_l890010();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkShootCallback_l890010) == 0x40, "layout");

// A callback set's base: an owned array of callbacks and a count.
struct UnkShootSetBase_l890010 {  // vtable g_unk_7101a20998
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;
};

// A callback set with the refcounted owner it belongs to.
struct UnkShootSet_l890010 : UnkShootSetBase_l890010 {  // vtable g_unk_7101a33fa0
    virtual void vf8();
    base::UnkRefHandle owner;

    ~UnkShootSet_l890010() {
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// The string-id list fn_7100893a1c builds.
struct UnkShootIdList_l890010 {
    u8 field_0x0;
    base::UnkStrIdMember id;
    UnkStrIdTable table;
};

struct UnkShootActivator_l890010 {
    char* vtable;
    u8 pad_0x8[0x380];
    CRntVector<UnkShootCallback_l890010> field_0x388;
    CRntVector<UnkShootCallback_l890010> field_0x3b0;
    CRntVector<u64> field_0x3d8;
    CRntVector<u64> field_0x400;
    base::UnkDeferredSet<void>* field_0x428;
    void* callback_owner;
    void* callback;
    u64 field_0x440;
    u64 field_0x448;
    UnkStrIdPathResource field_0x450;
    u64 field_0x468;
    s64 field_0x470;
    UnkShootIdList_l890010 list;  // 0x478
    f32 field_0x4b0;
    f32 field_0x4b4;
    u32 field_0x4b8;
    u32 field_0x4bc[5];
    base::UnkStrIdMember field_0x4d0;
    base::UnkStrIdMember field_0x4d8;
    u8 field_0x4e0[0x28];
    u8 field_0x508[0x28];
    UnkShootSet_l890010 field_0x530;
    u8 field_0x570;
    u32 field_0x574;
};
static_assert(__builtin_offsetof(UnkShootActivator_l890010, field_0x450) == 0x450, "layout");
static_assert(__builtin_offsetof(UnkShootActivator_l890010, list) == 0x478, "layout");
static_assert(__builtin_offsetof(UnkShootActivator_l890010, field_0x530) == 0x530, "layout");

void fn_7100890010(UnkShootActivatorBase_b54* self_) {
    UnkShootActivator_l890010* self = reinterpret_cast<UnkShootActivator_l890010*>(self_);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a7c540) + 0x10;
    self->field_0x3d8.Clear();
    self->field_0x388.Clear();
    self->field_0x3b0.Clear();
    self->field_0x400.Clear();

    self->field_0x530.~UnkShootSet_l890010();
    fn_71008904b0(reinterpret_cast<UnkClearVector_A4_b75*>(self->field_0x508));
    fn_71008904b0(reinterpret_cast<UnkClearVector_A4_b75*>(self->field_0x4e0));
    self->field_0x4d8.~UnkStrIdMemberOf();
    self->field_0x4d0.~UnkStrIdMemberOf();
    UnkStrIdTable* table = &self->list.table;
    fn_71004f11f8(table);
    if (table->keys)
        table->allocator->vf_0x48(table->keys);
    if (table->values)
        table->allocator->vf_0x48(table->values);
    self->list.id.~UnkStrIdMemberOf();
    UnkStrIdPathResource* path = &self->field_0x450;
    fn_7100027044(path);
    if (UnkCountedResource* p = path->resource)
        if (--p->ref_count == 0)
            fn_710001737c(p);
    path->ids.path.~UnkStrIdMemberOf();
    path->ids.id.~UnkStrIdMemberOf();
    if (self->field_0x428) {
        fn_71004f3ea4(self->field_0x428, &self->field_0x428);
        self->field_0x428 = nullptr;
    }
    self->callback = nullptr;
    self->callback_owner = nullptr;
    self->field_0x400.Free();
    self->field_0x3d8.Free();
    self->field_0x3b0.Free();
    self->field_0x388.Free();
    fn_71008e05e8(self);
}
