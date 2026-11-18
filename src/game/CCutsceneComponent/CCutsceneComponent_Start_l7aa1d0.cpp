// CCutsceneComponent's unit (tools/tu_map.py unit 1208): starts the cutscene. Keeps a handle to the
// player (+0x1e0), prepares or resets the actors (+0x210), and with records loaded (+0x200) collects the
// sub-area manager's sub-areas the cutscene names (+0x80) into +0xa8 once, then hands the owner's entity
// (+0x18) to the listeners of the lists at +0xf8 and +0x120. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkManager;
struct UnkCutsceneComponent_ovb0077;
void fn_71007a9048(UnkCutsceneComponent_ovb0077* self);
void fn_71007a941c(UnkCutsceneComponent_ovb0077* self);
struct UnkTriggerListElement;

// A sub-area as the sub-area manager lists them (0x188 bytes, its name first).
struct UnkSubareaEntry_l7aa1d0 {
    CStrId name;
    u8 pad_0x8[0x180];
};
static_assert(sizeof(UnkSubareaEntry_l7aa1d0) == 0x188, "layout");

struct UnkSubareaManager_l7aa1d0 {
    u8 pad_0x0[0x860];
    base::global::CRntVector<UnkSubareaEntry_l7aa1d0> entries;  // 0x860
};

static inline void* SubareaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

struct UnkCutsceneListener_l7aa1d0 {
    void** vtable;
    void vf_0x18(UnkReflObject* entity) {
        reinterpret_cast<void (*)(UnkCutsceneListener_l7aa1d0*, UnkReflObject*)>(vtable[0x18 / 8])(this, entity);
    }
};

struct UnkCutsceneComponent_l7aa1d0 {
    u8 pad_0x0[0x18];
    UnkReflObject* owner;  // 0x18
    u8 pad_0x20[0x60];
    base::global::CRntVector<base::UnkStrIdMember> subareas;  // 0x80
    base::global::CRntVector<UnkSubareaEntry_l7aa1d0*> active_subareas;  // 0xa8
    u8 pad_0xd0[0x28];
    base::global::CRntVector<UnkCutsceneListener_l7aa1d0*> listeners;  // 0xf8
    base::global::CRntVector<UnkCutsceneListener_l7aa1d0*> other_listeners;  // 0x120
    u8 pad_0x148[0x18];
    base::global::CRntVector<u8> flags;  // 0x160
    u8 pad_0x188[0x58];
    UnkWeakHandle* player;  // 0x1e0
    u8 pad_0x1e8[0x18];
    void* records;  // 0x200
    u8 pad_0x208[0x8];
    bool prepared;  // 0x210
    bool started;   // 0x211
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7aa1d0, listeners) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7aa1d0, other_listeners) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7aa1d0, flags) == 0x160, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7aa1d0, player) == 0x1e0, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7aa1d0, started) == 0x211, "layout");

// The owner as an entity, or null when it is not one.
static inline UnkReflObject* OwnerEntity(UnkCutsceneComponent_l7aa1d0* self) {
    UnkReflObject* owner = self->owner;
    if (!owner)
        return nullptr;
    return fn_710009d50c(owner->vf_0x0(), fn_7100d70c20()) ? owner : nullptr;
}

static inline void NotifyListeners(UnkCutsceneComponent_l7aa1d0* self,
                                   base::global::CRntVector<UnkCutsceneListener_l7aa1d0*>* list) {
    u32 count = list->count;
    for (u32 i = 0; i != count; ++i) {
        UnkCutsceneListener_l7aa1d0* listener =
            *reinterpret_cast<UnkCutsceneListener_l7aa1d0**>(fn_71007a804c(list, i));
        listener->vf_0x18(OwnerEntity(self));
    }
}

void fn_71007aa1d0(UnkCutsceneComponent_l7aa1d0* self) {
    self->started = true;
    AssignRef(&self->player, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(fn_7100defcd4(g_unk_7101d47310))));
    if (self->prepared) {
        fn_71007a9048(reinterpret_cast<UnkCutsceneComponent_ovb0077*>(self));
    } else {
        self->prepared = false;
        self->flags.Resize(0);
    }
    if (!self->records)
        return;
    fn_71007a941c(reinterpret_cast<UnkCutsceneComponent_ovb0077*>(self));
    if (self->subareas.count && !self->active_subareas.count) {
        UnkSubareaManager_l7aa1d0* manager = static_cast<UnkSubareaManager_l7aa1d0*>(SubareaManager());
        // An empty id the original builds and drops around the search without reading it.
        CStrId none(base::global::UnkStrIdInline(), nullptr, false);
        for (UnkSubareaEntry_l7aa1d0& entry : manager->entries) {
            for (base::UnkStrIdMember* name = self->subareas.begin(); name != self->subareas.end(); ++name) {
                if (entry.name.data == name->data) {
                    self->active_subareas.PushBack(&entry);
                    break;
                }
            }
        }
    }
    NotifyListeners(self, &self->listeners);
    NotifyListeners(self, &self->other_listeners);
}
