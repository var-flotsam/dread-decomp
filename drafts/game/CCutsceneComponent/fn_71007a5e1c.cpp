// CCutsceneComponent's unit: the destructor body: runs fn_71007a6214, releases the shared source, the
// own entry, the path and name ids and the seven arrays, then the base destructor (fn_710024f6d4).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CCutsceneComponent.h"
#include "types.h"

using base::UnkRefPtrMember;
using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkCutsceneComponent_ovb0077;
void fn_71007a6214(UnkCutsceneComponent_ovb0077* self);

extern void* g_unk_7101a6ff40[];
extern char g_unk_7101a70078[];

// An object deleted through its vtable.
struct UnkCutsceneObject_l7a5e1c {
    virtual void vf_0x0();
    virtual ~UnkCutsceneObject_l7a5e1c();
};

// An array element that owns its object.
struct UnkCutsceneOwned_l7a5e1c {
    UnkCutsceneObject_l7a5e1c* ptr;

    ~UnkCutsceneOwned_l7a5e1c() {
        UnkCutsceneObject_l7a5e1c* old = ptr;
        ptr = nullptr;
        if (old)
            delete old;
    }
};

struct UnkCutsceneRef_l7a5e1c {
    UnkRefPtrMember ptr;
    u64 field_0x8;
};

struct UnkCutsceneOffset_l7a5e1c {
    u32 values[3];
};

struct UnkCutsceneEntry_l7a5e1c {
    virtual void vf_0x0();
    virtual ~UnkCutsceneEntry_l7a5e1c();
    u8 pad_0x8[0x80];
};

// Shared between threads: the count is changed atomically.
struct UnkCutsceneSource_l7a5e1c {
    virtual void vf_0x0();
    virtual ~UnkCutsceneSource_l7a5e1c();
    s32 ref_count;
};

struct UnkCutsceneOwnEntry_l7a5e1c {
    void** vtable;
    u8 pad_0x8[0x80];
    UnkRefPtrMember owner;  // 0x88
    u64 field_0x90;
    u32 field_0x98;
};

struct UnkCutsceneComponent_l7a5e1c {
    void** vtable;
    u8 pad_0x8[0x28];
    CRntVector<UnkCutsceneRef_l7a5e1c> refs;  // 0x30
    CRntVector<UnkCutsceneOffset_l7a5e1c> offsets;  // 0x58
    CRntVector<CStrId> names;  // 0x80
    CRntVector<u64> list_0xa8;
    CRntVector<UnkCutsceneEntry_l7a5e1c> entries;  // 0xd0
    CRntVector<UnkCutsceneOwned_l7a5e1c> owned_0xf8;
    CRntVector<UnkCutsceneOwned_l7a5e1c> owned_0x120;
    CStrId name;  // 0x148
    CFilePathStrId path;  // 0x150
    UnkCutsceneOwnEntry_l7a5e1c own;  // 0x158
    u64 field_0x1f8;
    UnkCutsceneSource_l7a5e1c* source;  // 0x200
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a5e1c, own) == 0x158, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a5e1c, source) == 0x200, "layout");

void fn_71007a5e1c(UnkCutsceneComponent_l7a5e1c* self) {
    self->vtable = g_unk_7101a6ff40 + 2;
    fn_71007a6214(reinterpret_cast<UnkCutsceneComponent_ovb0077*>(self));
    if (UnkCutsceneSource_l7a5e1c* source = self->source) {
        __atomic_fetch_sub(&source->ref_count, 1, __ATOMIC_SEQ_CST);
        if (__atomic_load_n(&source->ref_count, __ATOMIC_SEQ_CST) == 0)
            delete source;
    }
    self->own.vtable = reinterpret_cast<void**>(g_unk_7101a70078 + 0x10);
    self->own.owner.~UnkRefPtrMember();
    fn_71007a8eec(&self->own);
    self->path.~CFilePathStrId();
    self->name.~CStrId();
    self->owned_0x120.Free();
    self->owned_0xf8.Free();
    self->entries.Free();
    self->list_0xa8.Free();
    self->names.Free();
    self->offsets.Free();
    self->refs.Free();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}
