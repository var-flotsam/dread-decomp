// CSaveSnapshotToCheckpointLogicAction's unit (tools/tu_map.py): its constructor, on the logic-action
// base (fn_7100a55aa4): the vtable, empty string ids at +0x10 and +0x18, then a callback set at +0x20
// built through its base (a vtable, an empty array with allocator flags 0xf, a cleared word) before its
// own vtable and a cleared owner. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
void fn_7100a55aa4(UnkChangeSetupLogicActionDef*);
extern char g_unk_7101a8e6c0[];
extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a20950[];  // the callback set's vtable

struct UnkSaveSnapshotCallable_b52;

struct UnkSaveSnapshotCallbackSet_b52 {
    void* vtable;
    base::UnkVector<UnkSaveSnapshotCallable_b52*> items;
    u32 field_0x30;
    void* owner;
};

static inline __attribute__((always_inline)) void InitEmpty_ss52(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

struct UnkSaveSnapshotLogicAction_b52 {
    void* vtable;
    u8 unk_0x8[0x8];
    UnkStringData* first;                      // 0x10
    UnkStringData* second;                     // 0x18
    UnkSaveSnapshotCallbackSet_b52 callbacks;  // 0x20
};

void fn_7100a42800(UnkSaveSnapshotLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8e6c0 + 0x10;
    InitEmpty_ss52(&self->first);
    InitEmpty_ss52(&self->second);
    self->callbacks.vtable = g_unk_7101a20998 + 2;
    new (&self->callbacks.items) base::UnkVector<UnkSaveSnapshotCallable_b52*>(0xf);
    self->callbacks.field_0x30 = 0;
    self->callbacks.vtable = g_unk_7101a20950 + 2;
    self->callbacks.owner = nullptr;
}
