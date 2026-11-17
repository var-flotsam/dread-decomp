// CRotationalPlatformComponent's unit (tools/tu_map.py unit 1498): its destructor (fn_71009d81cc). The
// vtables, the link at +0xd0 taken out of its list (fn_71002aeb9c) and its two words cleared, the
// string-id table at +0xa8 reset and freed, the string id at +0xa0 released, the callback list at +0x48
// (its counted reference dropped, its callbacks destroyed), then the component's base destructor
// (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CRotationalPlatformComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// A type-erased callback: storage and the manager that destroys it.
struct UnkCallbackManager_l9d81cc {
    u8 pad_0x0[0x40];
    void (*destroy)(void* callback);  // 0x40
};

struct UnkCallback_l9d81cc {
    u64 storage;
    UnkCallbackManager_l9d81cc* manager;

    ~UnkCallback_l9d81cc() {
        if (manager) {
            manager->destroy(this);
            manager = nullptr;
        }
    }
};

struct UnkCallbackList_l9d81cc {
    void** vtable;
    base::global::CRntVector<UnkCallback_l9d81cc> callbacks;  // +0x8
    u8 pad_0x30[0x8];
    UnkRefObject* ref;  // +0x38
};

struct UnkRotationalPlatformComponent_l9d81cc {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkCallbackList_l9d81cc list;  // 0x48
    u8 pad_0x88[0x18];
    CStrId field_0xa0;
    UnkOwnedStrIdTable field_0xa8;
    UnkLinkList* link;  // 0xd0
    void* field_0xd8;
    void* field_0xe0;
};
static_assert(__builtin_offsetof(UnkRotationalPlatformComponent_l9d81cc, field_0xa8) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkRotationalPlatformComponent_l9d81cc, link) == 0xd0, "layout");

void fn_71009d81cc(UnkRotationalPlatformComponent_l9d81cc* self) {
    self->vtable = &g_unk_7101a89c50[2];
    self->vtable_0x40 = &g_unk_7101a89c50[0x34];
    UnkLinkList** slot = &self->link;
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
    self->field_0xd8 = nullptr;
    self->field_0xe0 = nullptr;
    self->field_0xa8.~UnkOwnedStrIdTable();
    self->field_0xa0.Reset();
    UnkCallbackList_l9d81cc* list = &self->list;
    list->vtable = &g_unk_7101a33fa0[2];
    if (UnkRefObject* ref = list->ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    list->vtable = &g_unk_7101a20998[2];
    list->callbacks.Free();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
