// CTeleporterUsableComponent's unit (tools/tu_map.py hint): the start step with the usable object
// (fn_7100a8f1d0), the handler that stores a pair of ids and runs a virtual step (fn_7100a8f34c), the
// activation toggle (fn_7100a8f448), the effect started for the current mode (fn_7100a8e4c0) and the
// handler that takes the object's handle and sets a blackboard flag (fn_7100a8e9b4). Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1600.h"
#include "game/UnkTU2523.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The object the component acts on.
struct UnkTeleporterObject_b0233 {
    u8 pad_0x0[0x3b0];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};

// ---- fn_7100a8f1d0 ----

struct UnkTeleporterActor_b0233 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkTeleporterOwner_b0233 {
    u8 pad_0x0[0x30];
    UnkTeleporterActor_b0233* actor;  // 0x30
};

struct UnkMatrix4_b0233 {
    f32 m[16];
};

void fn_710113cb74(void* component, UnkTeleporterActor_b0233* actor, void* unused, UnkMatrix4_b0233* matrix);
void fn_7100233f84(UnkTeleporterObject_b0233* object, UnkMatrix4_b0233* matrix, u32 flag);
void fn_7100aa1b08(UnkTeleporterOwner_b0233* self, UnkTeleporterObject_b0233* object, void* arg);

bool fn_7100a8f1d0(UnkTeleporterOwner_b0233* self, UnkTeleporterObject_b0233* object, void* arg) {
    if (!object)
        return false;
    UnkTeleporterActor_b0233* actor = self->actor;
    if (void* component = actor->field_0x260) {
        UnkMatrix4_b0233 matrix;
        __builtin_memset(&matrix, 0, sizeof(matrix));
        matrix.m[15] = 1.0f;
        matrix.m[10] = 1.0f;
        matrix.m[5] = 1.0f;
        matrix.m[0] = 1.0f;
        fn_710113cb74(component, actor, nullptr, &matrix);
        fn_7100233f84(object, &matrix, 1);
        object->field_0x3c0 = true;
        object->field_0x3b0 = true;
    }
    fn_7100aa1b08(self, object, arg);
    return true;
}

// ---- fn_7100a8f34c ----

struct UnkTeleporterVtable_b0233 {
    u8 pad_0x0[0x268];
    void (*vf_0x268)(struct UnkTeleporterIds_b0233* self);
    void (*vf_0x270)(struct UnkTeleporterIds_b0233* self);
};

struct UnkTeleporterIds_b0233 {
    UnkTeleporterVtable_b0233* vtable;
    u8 pad_0x8[0x550];
    CStrId first;   // 0x558
    CStrId second;  // 0x560
};

void fn_7100a8f34c(UnkTeleporterIds_b0233* self, bool enable, const CStrId* ids) {
    if (enable) {
        self->first = ids[0];
        self->second = ids[1];
        self->vtable->vf_0x268(self);
    } else {
        self->vtable->vf_0x270(self);
    }
}

// ---- fn_7100a8f448 ----

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);
struct UnkEntityState;
void fn_7100232864(UnkEntityState* object, bool flag);
extern u64 g_unk_710164b5e0;
extern u64 g_unk_710164b5e8;

struct UnkTeleporterPartner_b0233 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTeleporter_b0233 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0xa8];
    UnkTeleporterPartner_b0233* partner;  // 0xe0
    u8 pad_0xe8[0x13];
    bool field_0xfb;
    bool active;  // 0xfc
    u8 pad_0xfd[0x49b];
    u8 field_0x598[0x28];
    u32 state;  // 0x5c0
    u8 pad_0x5c4[4];
    base::UnkRefHandle target;  // 0x5c8
};

u32 fn_7100a8f448(UnkTeleporter_b0233* self, bool flag) {
    bool wasActive = self->active;
    bool inactive = !flag;
    self->field_0xfb = inactive;
    if (wasActive != flag) {
        self->active = flag;
        fn_7100d7c828(self->actor, flag ? &g_unk_710164b5e0 : &g_unk_710164b5e8, false);
        if (self->state == 1) {
            {
                UnkRef handle = fn_7100353be8(self->field_0x598);
                fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
            }
            self->state = 2;
        }
        if (base::UnkRefCounted* target = self->target.ptr) {
            if (*reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)) {
                if (self->partner && self->partner->field_0x10) {
                    // Keeps the handle's object reloaded after the partner tests.
                    asm("" : "+r"(target));
                    fn_7100232864(*reinterpret_cast<UnkEntityState**>(reinterpret_cast<char*>(target) + 0x10), flag);
                    fn_7100232864(self->partner ? static_cast<UnkEntityState*>(self->partner->field_0x10) : nullptr, inactive);
                }
            }
        }
    }
    return 0;
}

// ---- fn_7100a8e4c0 ----

// A string id that is never null: the copy and release test only against the pool's empty id.
struct UnkFxId_b0233 {
    UnkStringData* data;

    UnkFxId_b0233(const UnkFxId_b0233& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkFxId_b0233() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
    }
};

struct UnkTeleporterFxEntry_b0233 {
    u32 value;
    u32 pad_0x4;
    UnkFxId_b0233 id;
};
extern UnkTeleporterFxEntry_b0233 g_unk_7172751a60[8] __attribute__((visibility("hidden")));

struct UnkTeleporterFx_b0233 {
    u8 pad_0x0[0x228];
    void* field_0x228;
};

struct UnkTeleporterSelf_b0233 {
    u8 pad_0x0[0x30];
    UnkTeleporterFx_b0233* actor;  // 0x30
    u8 pad_0x38[0x550];
    u32 mode;  // 0x588
};

void fn_7100a9f300(UnkTeleporterSelf_b0233* self);
void fn_7100a8e614(UnkTeleporterSelf_b0233* self);
struct UnkEnergyWaveFxComp_b49;

void fn_7100a8e4c0(UnkTeleporterSelf_b0233* self) {
    fn_7100a9f300(self);
    fn_7100a8e614(self);
    for (UnkTeleporterFxEntry_b0233& entry : g_unk_7172751a60) {
        if (entry.value == self->mode) {
            UnkFxId_b0233 name = entry.id;
            void* component = self->actor->field_0x228;
            u64 hash = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(name.data) + 0x30);
            fn_71011738f8(static_cast<UnkEnergyWaveFxComp_b49*>(component), &hash, 1);
            break;
        }
    }
}

// ---- fn_7100a8e9b4 ----

struct UnkWeakTarget;
struct UnkWeakHandle;

// A typed value as the section setter takes it (as in src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern UnkPropertyEx<> g_unk_7172759438;

struct UnkTeleporterHandlesVtable_b0233 {
    u8 pad_0x0[0x2a0];
    void (*vf_0x2a0)(struct UnkTeleporterHandles_b0233* self);
};

struct UnkTeleporterHandles_b0233 {
    UnkTeleporterHandlesVtable_b0233* vtable;
    u8 pad_0x8[0xe0];
    base::UnkRefHandle handle;  // 0xe8
};

bool fn_7100a8e9b4(UnkTeleporterHandles_b0233* self, UnkTeleporterObject_b0233* object) {
    if (!object)
        return false;
    self->handle.Set(reinterpret_cast<base::UnkRefCounted*>(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(object))));
    self->vtable->vf_0x2a0(self);
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x22a];
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172759438);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172759438), g_unk_7101d470e0);
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, id)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue typed;
            typed.type = fn_710000d228();
            typed.value = &value;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&qid.id), &typed);
        }
    }
    return true;
}
