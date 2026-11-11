// CFrozenPlatformComponent's unit (tools/tu_map.py unit 1496): its destructor (fn_71009d5cb0). The
// member at +0x58 (a polymorphic object) drops its counted reference (+0x90), falls back to its
// base's vtable and frees its callback array (+0x60, each callback destroyed through its manager's
// +0x40); the deferred-set entry at +0x40 is removed (fn_71004f3ea4); then the component's base
// destructor runs (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CFrozenPlatformComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A type-erased callback: storage and the manager that destroys it.
struct UnkCallbackManager_b0119 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* callback);  // 0x40
};

struct UnkCallback_b0119 {
    u64 storage;
    UnkCallbackManager_b0119* manager;

    ~UnkCallback_b0119() {
        if (manager) {
            manager->destroy(this);
            manager = nullptr;
        }
    }
};

struct UnkCallbackList_b0119 {
    void** vtable;
    base::UnkVector<UnkCallback_b0119> callbacks;  // +0x8
    u8 pad_0x30[0x8];
    UnkRefObject* ref;  // +0x38
};

// An entry in a deferred set: the set, then two words.
struct UnkSetEntry_b0119 {
    base::UnkDeferredSet<void>* set;
    void* field_0x8;
    void* field_0x10;
};

struct UnkFrozenPlatformComponent_b0119 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkSetEntry_b0119 entry;  // 0x40
    UnkCallbackList_b0119 list;  // 0x58
};

void fn_71009d5cb0(UnkFrozenPlatformComponent_b0119* self) {
    self->vtable = &g_unk_7101a89908[2];
    UnkCallbackList_b0119* list = &self->list;
    list->vtable = &g_unk_7101a33fa0[2];
    if (UnkRefObject* ref = list->ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    list->vtable = &g_unk_7101a20998[2];
    list->callbacks.Free();
    if (self->entry.set) {
        fn_71004f3ea4(self->entry.set, &self->entry);
        self->entry.set = nullptr;
    }
    self->entry.field_0x8 = nullptr;
    self->entry.field_0x10 = nullptr;
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
