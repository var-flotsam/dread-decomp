// Unit 1176 (CCentralUnitComponent): sets the central unit up for a mode (other than 7): hooks its callbacks in,
// binds the linked spawn point's entity and its Emmy AI, places the entity at the tracked object, stores the
// unit's state in the blackboard, tells the script and unlocks the linked door locks in mode 1. Names and
// layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkModeFlags.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTU367.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game/lua_lib.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CStrId;

struct UnkCentralUnit_l7744d0;
struct UnkObj_7101129240;
void fn_710077b8f8();
void fn_710077bcd8();

struct UnkLinkHandle_l7744d0 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

// A lazily resolved link to another entity: the key is resolved to a handle on first use.
struct UnkEntityLink_l7744d0 {
    u8 pad_0x0[0x8];
    u8 key[0x28];  // 0x8
    u32 state;     // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38

    void Resolve() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
    }
    UnkWeakTarget* Get() {
        Resolve();
        UnkLinkHandle_l7744d0* h = reinterpret_cast<UnkLinkHandle_l7744d0*>(handle.ptr);
        return h ? h->target : nullptr;
    }
};
static_assert(sizeof(UnkEntityLink_l7744d0) == 0x40, "layout");

struct UnkEntryOwner_l7744d0 {
    u8 pad_0x0[0x198];
    u8 list[8];  // 0x198
};

struct UnkEntry_l7744d0 {
    void** vtable;
    u8 pad_0x8[0x1a4];
    u8 field_0x1b4;
    u8 pad_0x1b5[3];
    UnkEntryOwner_l7744d0* owner;  // 0x1b8
    u8 pad_0x1c0[0x80];
    base::reflection::UnkReflObject* ai;  // 0x240

    void SetPosition(const UnkVec3f* v, bool flag) {
        reinterpret_cast<void (*)(UnkEntry_l7744d0*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, v, flag);
    }
    void SetAngle(const UnkVec3f* v, bool flag) {
        reinterpret_cast<void (*)(UnkEntry_l7744d0*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, v, flag);
    }
};

struct UnkTrackedObject_l7744d0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkTracked_l7744d0 {
    u8 pad_0x0[0x10];
    UnkTrackedObject_l7744d0* object;  // 0x10
};

struct UnkValue_l7744d0 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkCallback_l7744d0 {
    void* object;
    void (*function)();
};

struct UnkCentralUnit_l7744d0 {
    void** vtable;
    u8 pad_0x8[0x3e8];
    u32 mode;  // 0x3f0
    u8 pad_0x3f4[0x540 - 0x3f4];
    UnkEntityLink_l7744d0 spawn;  // 0x540
    u8 pad_0x580[0x640 - 0x580];
    UnkEntityLink_l7744d0* locks;  // 0x640
    u32 lock_count;                // 0x648
    u8 pad_0x64c[0x6d8 - 0x64c];
    base::UnkRefHandle emmy;  // 0x6d8
    UnkLinkList* list;        // 0x6e0
    void* object_0x6e8;
    void (*function_0x6f0)();
    UnkTracked_l7744d0* tracked;  // 0x6f8
    u8 pad_0x700[0x770 - 0x700];
    void* object_0x770;
    void (*function_0x778)();
    u8 pad_0x780[0x7a0 - 0x780];
    UnkValue_l7744d0* values;  // 0x7a0
    u64* keys;                 // 0x7a8
    u32 count;                 // 0x7b0

    UnkValue_l7744d0* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, mode) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, spawn) == 0x540, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, emmy) == 0x6d8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, tracked) == 0x6f8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, object_0x770) == 0x770, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l7744d0, values) == 0x7a0, "layout");

// The entry fn_7101129b08 returns (its declaration leaves the result out).
static inline UnkEntry_l7744d0* EntryOf(UnkSpawnRegistered_ovb0065* object) {
    return reinterpret_cast<UnkEntry_l7744d0* (*)(UnkObj_7101129240*, bool)>(&fn_7101129b08)(
        reinterpret_cast<UnkObj_7101129240*>(object), false);
}

void fn_71007744d0(UnkCentralUnit_l7744d0* self, s32 mode) {
    if (mode == 7)
        return;
    if (g_unk_71726bc570->field_0xb8) {
        self->object_0x770 = self;
        self->function_0x778 = fn_710077bcd8;
    }
    if (self->spawn.state == 1) {
        {
            base::UnkRefPtrMember ref = fn_7100353be8(self->spawn.key);
            fn_710031d208(&self->spawn.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        self->spawn.state = 2;
    }
    if (!self->spawn.Get())
        return;
    UnkWeakTarget* target = self->spawn.Get();
    if (!target)
        return;
    UnkSpawnRegistered_ovb0065* registered = fn_7100360044(target);
    if (!registered)
        return;
    fn_71011269c0(reinterpret_cast<CSpawnPointComponent*>(registered));
    self->object_0x6e8 = self;
    self->object_0x770 = self;
    self->function_0x6f0 = fn_710077b8f8;
    self->function_0x778 = fn_710077bcd8;
    UnkEntry_l7744d0* entry = EntryOf(registered);
    if (!entry)
        return;
    self->emmy.Set(reinterpret_cast<base::UnkRefCounted*>(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entry))));
    UnkEntryOwner_l7744d0* owner = entry->owner;
    UnkLinkList** slot = &self->list;
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
    *slot = reinterpret_cast<UnkLinkList*>(owner->list);
    fn_71002af070(reinterpret_cast<UnkLinkList*>(owner->list), slot);
    if (base::reflection::UnkReflObject* ai = entry->ai) {
        if (fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
            reinterpret_cast<void (*)(base::reflection::UnkReflObject*, UnkCentralUnit_l7744d0*)>(
                (*reinterpret_cast<void***>(ai))[0x810 / 8])(ai, self);
    }
    if (self->tracked && self->tracked->object) {
        u8 flag = entry->field_0x1b4;
        fn_7100232864(reinterpret_cast<UnkEntityState*>(entry), false);
        UnkVec3f vec = self->tracked->object->xform.GetTranslation();
        entry->SetPosition(&vec, true);
        UnkTrackedObject_l7744d0* object = self->tracked ? self->tracked->object : nullptr;
        vec = object->xform.GetAngle();
        entry->SetAngle(&vec, true);
        if (flag)
            fn_7100232864(reinterpret_cast<UnkEntityState*>(entry), true);
    }
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    bool flag = self->mode != 1;
    UnkValue_l7744d0* name = self->Find(g_unk_7172714220);
    if (CBlackboard::CSection* section = fn_7101047270(&g_unk_7101d47310->field_0x968, section_id))
        fn_710006d11c(section, &name->id, &flag);
    fn_7100232864(reinterpret_cast<UnkEntityState*>(entry), !flag);
    if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnCentralUnitEmmyReady")) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->vtable[0x18 / 8]);
        fn_7100fbc4e0(g_unk_7172a01d18, entry);
        char buffer[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 2, buffer, 0x100, nullptr, nullptr);
    }
    if (self->mode == 1) {
        UnkEntityLink_l7744d0* end = self->locks + self->lock_count;
        for (UnkEntityLink_l7744d0* link = self->locks; link != end; ++link) {
            if (UnkWeakTarget* t = link->Get()) {
                if (UnkDoorLock* lock = fn_71002af860(t)) {
                    CStrId id = g_unk_7101d470e0;
                    fn_71008d586c(lock, false, false, &id);
                }
            }
        }
    }
}
