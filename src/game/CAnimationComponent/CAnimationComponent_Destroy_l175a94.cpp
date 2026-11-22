// CAnimationComponent's unit (tools/tu_map.py): the destructor of the component's state that
// fn_71011757f4 constructs (the layout is that file's). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkHashStream.h"
#include "base/UnkInitRecord_0x5c.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CComponent.h"
#include "game/UnkAnimOwned_l1757f4.h"
#include "game/UnkComponent.h"
#include "game/UnkResourceRef_l1757f4.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU39.h"
#include "game/UnkVtableHolder_l1757f4.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkStruct_71007142e8;
struct UnkPropBag;
void fn_710050e8e4(UnkPropBag*);

// The pool list at +0x4c0 (an out-of-line base::refl_PoolListCtor).

struct UnkAnimPoolTag_l1757f4;
template <>
struct base::UnkPoolListTraits<UnkAnimPoolTag_l1757f4> {
    static char* vtable() { return g_unk_7101cb4840; }
    static char* base_vtable() { return g_unk_7101cb4860; }
};

// The listener lists: active, pending and queued, and the lock count of the passes over them.
struct UnkAnimListenerSet_l1757f4 : UnkVtableHolder_l1757f4 {
    CRntVector<void*> active;   // 0x8
    CRntVector<void*> pending;  // 0x30
    CRntVector<void*> queued;   // 0x58
    s32 lock;                   // 0x80

    UnkAnimListenerSet_l1757f4()
        : UnkVtableHolder_l1757f4(g_unk_7101cb4880 + 2), active(0xf), pending(0xf), queued(0xf), lock(0) {}
    ~UnkAnimListenerSet_l1757f4() {
        vtable = g_unk_7101cb4880 + 2;
        queued.Free();
        pending.Free();
        active.Free();
    }
};

struct UnkAnimState_l1757f4 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAnimOwned_l1757f4* owned;  // 0x40
    CRntVector<UnkResourceRef_l1757f4> resources;  // 0x48
    u64 field_0x70;
    u32 field_0x78;
    CRntVector<void*> field_0x80;  // 0x80 to 0x108: destroyed together by fn_710050e8e4
    UnkHashStream_0x38 stream;     // 0xa8
    CRntVector<void*> field_0xe0;
    u8 field_0x108[0x70];
    u8 field_0x178[0x70];
    u8 field_0x1e8[0x70];
    u8 record[0x5c];  // 0x258
    u32 field_0x2b4;
    CStrId name_a;  // 0x2b8
    CStrId name_b;  // 0x2c0
    u32 field_0x2c8;
    u8 pad_0x2cc[4];
    u8 field_0x2d0[0xb0];
    u8 field_0x380[0xb0];
    UnkAnimListenerSet_l1757f4 listeners;  // 0x430
    u8 field_0x4b8;
    u8 pad_0x4b9;
    u8 field_0x4ba;
    u8 pad_0x4bb[5];
    base::UnkPoolList<UnkAnimPoolTag_l1757f4, 8, 0x10> pool;  // 0x4c0
};
static_assert(__builtin_offsetof(UnkAnimState_l1757f4, stream) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkAnimState_l1757f4, field_0x2b4) == 0x2b4, "layout");
static_assert(__builtin_offsetof(UnkAnimState_l1757f4, listeners) == 0x430, "layout");
static_assert(__builtin_offsetof(UnkAnimState_l1757f4, pool) == 0x4c0, "layout");

void fn_7101175a94(UnkAnimState_l1757f4* self) {
    self->vtable = g_unk_7101cb46c0 + 2;
    self->pool.~UnkPoolList();
    self->listeners.~UnkAnimListenerSet_l1757f4();
    fn_710002f67c(self->field_0x380);
    fn_710002f67c(self->field_0x2d0);
    self->name_b.~CStrId();
    self->name_a.~CStrId();
    fn_710050e8e4(reinterpret_cast<UnkPropBag*>(&self->field_0x80));
    self->resources.Free();
    UnkAnimOwned_l1757f4* owned = self->owned;
    self->owned = nullptr;
    if (owned)
        owned->vf_0x10();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
