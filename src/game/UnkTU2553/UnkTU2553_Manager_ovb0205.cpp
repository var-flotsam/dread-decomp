// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553, hint only): game-manager helpers: the
// player's ability objects, the "[MANTAIN]" save steps, the screen mode and flag setters, the id-table
// lookups and the clean-up walks over its handles and id set. Names and layouts are placeholders.
#include <stdio.h>
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkHashMap.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBlackboard.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU57.h"
#include "game/UnkTU83.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkCompared_0x30;
struct UnkStepCallbacks;
struct UnkStepCallback;

// The id table (fn_7100031894 builds it).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xf18];
    CStrId field_0xf18;
    u8 pad_0xf20[0x1150 - 0xf20];
    CStrId field_0x1150;
    u8 pad_0x1158[0x18a0 - 0x1158];
    CStrId field_0x18a0;
    u8 pad_0x18a8[0x1f00 - 0x18a8];
    CStrId field_0x1f00;
    u8 pad_0x1f08[0x2050 - 0x1f08];
    CStrId field_0x2050;
    u8 pad_0x2058[0x2578 - 0x2058];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}


struct UnkAbility;
struct UnkAbilitySet;

extern char g_unk_71727bee18[] __attribute__((visibility("hidden")));

struct UnkTgmPlayer_b205 {
    u8 pad_0x0[0x2e0];
    UnkAbilitySet* abilities;
};
struct UnkTgmHandle_b205 {
    u8 pad_0x0[0x10];
    UnkTgmPlayer_b205* player;
};
struct UnkTgm_b205 {
    u8 pad_0x0[0x29b8];
    UnkTgmHandle_b205* field_0x29b8;
};


UnkReflObject* fn_7100df554c(UnkTgm_b205* self) {
    UnkTgmHandle_b205* handle = self->field_0x29b8;
    if (!handle)
        return nullptr;
    UnkTgmPlayer_b205* player = handle->player;
    if (!player)
        return nullptr;
    UnkAbilitySet* abilities = player->abilities;
    if (!abilities)
        return nullptr;
    UnkReflObject* ability =
        reinterpret_cast<UnkReflObject*>(fn_71003012f8(abilities, reinterpret_cast<u64>(g_unk_71727bee18)));
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->vf_0x0(), fn_7100306914()) ? ability : nullptr;
}

UnkReflObject* fn_7100df5460(UnkTgm_b205* self) {
    UnkTgmHandle_b205* handle = self->field_0x29b8;
    if (!handle)
        return nullptr;
    UnkTgmPlayer_b205* player = handle->player;
    if (!player)
        return nullptr;
    UnkAbilitySet* abilities = player->abilities;
    if (!abilities)
        return nullptr;
    UnkReflObject* ability = reinterpret_cast<UnkReflObject*>(
        fn_71003012f8(abilities, reinterpret_cast<u64>(&GameManagerIdTable().field_0xf18)));
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->vf_0x0(), fn_7100303134()) ? ability : nullptr;
}
void fn_7100df2b68(void* self);
void fn_7100dec7a0(void* self);
struct UnkWeaponDamage_ovb0054;

// ---- fn_7100df2acc
struct UnkTgmStart_b205 {
    u8 pad_0x0[0x2a28];
    CRntString field_0x2a28;
    CRntString field_0x2a58;
    CRntString field_0x2a88;
    u8 pad_0x2ab8[0x2ae4 - 0x2ab8];
    u32 field_0x2ae4;
};
void fn_7100df28d0(UnkGameManager* manager, const CRntString& a, const CRntString& b,
                   const CRntString& c, const CStrId& id, bool flag, const CRntString& d,
                   const CStrId& e, u32 f, void* g, f32 h);

void fn_7100df2acc(UnkGameManager* manager) {
    UnkTgmStart_b205* self = reinterpret_cast<UnkTgmStart_b205*>(manager);
    fn_7100df2b68(self);
    u64 unset;  // the original leaves this argument register as it was
    auto start = reinterpret_cast<void (*)(UnkGameManager*, const CRntString&, const CRntString&,
                                           const CRntString&, u64, u32, const CRntString&, const CStrId&, u32,
                                           void*, f32)>(fn_7100df28d0);
    start(manager, self->field_0x2a28, self->field_0x2a58, self->field_0x2a88, unset, self->field_0x2ae4,
          CRntString(g_unk_710162e2ed), g_unk_7101d470e0, 0,
          nullptr, 0.0f);
}

// ---- fn_7100dec69c
struct UnkCallableBase_b205 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Destroy();
    virtual void DestroyDeallocate();
};
struct UnkSmallFunction_b205 {
    struct Callable {
        char* vtable;
        u64 a;
        u64 b;
    };
    union {
        Callable buf;
        u8 raw[0x20];
    };
    UnkCallableBase_b205* target;
    u64 field_0x28;
    ~UnkSmallFunction_b205() {
        if (reinterpret_cast<void*>(this) == reinterpret_cast<void*>(target))
            target->Destroy();
        else if (target)
            target->DestroyDeallocate();
    }
};
extern char g_unk_7101aa87e8[];
void fn_7100dec38c(UnkGameManager* self, UnkSmallFunction_b205* f, bool a, u32 b, u32 c);
void fn_7100d8be38(void* self, f32 t);

void fn_7100dec69c(UnkGameManager* self, f32 value, bool flag) {
    {
        UnkSmallFunction_b205 f;
        f.buf.vtable = g_unk_7101aa87e8 + 0x10;
        f.buf.a = 0xa0;
        f.buf.b = 1;
        f.target = reinterpret_cast<UnkCallableBase_b205*>(&f);
        fn_7100dec38c(self, &f, flag, 1, 1);
    }
    fn_7100d8be38(reinterpret_cast<char*>(self) + 0x2848, value);
}

void fn_7100dec7a0(void* self);

struct UnkTgmScreenFlags_b205 {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    bool field_0x3;
    u8 pad_0x4[4];
    u8 field_0x8[0x100];
    bool field_0x108;
};
struct UnkTgmScreen_b205 {
    u8 pad_0x0[0x2328];
    s32 mode;
    u32 value;
    CStrId id;
    UnkTgmScreenFlags_b205 flags;
};
static_assert(__builtin_offsetof(UnkTgmScreen_b205, flags) == 0x2338, "layout");

static inline __attribute__((always_inline)) void SetScreen(UnkTgmScreen_b205* self, s32 mode, u32 value, bool a,
                                                            bool keep, const CStrId* id, bool c) {
    bool same = self->mode == mode;
    bool either = same | keep;
    bool kept;
    if (same) {
        if (!keep)
            kept = self->flags.field_0x1;
        else
            kept = either;
    } else {
        kept = either;
    }
    self->mode = mode;
    self->value = value;
    self->flags.field_0x0 = a;
    self->flags.field_0x1 = kept;
    self->id = *id;
    self->flags.field_0x2 = c;
}

void fn_7100df3300(UnkTgmScreen_b205* self, s32 mode, u32 value, bool a, bool keep, const CStrId* id, bool c, void* x7) {
    SetScreen(self, mode, value, a, keep, id, c);
    if (x7) {
        self->flags.field_0x3 = true;
        fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(self->flags.field_0x8),
                      static_cast<const UnkWeaponDamage_ovb0054*>(x7));
    }
    if (!self->flags.field_0x108)
        fn_7100dec7a0(self);
}

void fn_7100df3418(UnkGameManager* manager, bool a, bool b, void* x3) {
    CStrId id = g_unk_7101d470e0;
    fn_7100df3300(reinterpret_cast<UnkTgmScreen_b205*>(manager), 2, 0, a, b, &id, false, x3);
}


// ---- fn_7100e010a0: sets a flag on the model of every object a registry holds
struct UnkTgmModelState_b205 {
    u8 pad_0x0[0x114];
    u8 field_0x114;
    u8 pad_0x115[7];
    u8 field_0x11c;
};
struct UnkTgmModel_b205 {
    u8 pad_0x0[0x50];
    UnkTgmModelState_b205* state;
};
struct UnkTgmHolder_b205 {
    u8 pad_0x0[0x18];
    void* entity;
};
struct UnkTgmRef_b205 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkTgmHolder_b205* target;
};
struct UnkTgmRefVector_b205 {
    UnkTgmRef_b205** data;
    u32 count;
};
UnkTgmRefVector_b205* fn_71006e9fb4();
struct UnkStruct_7100233684;
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684* self);

void fn_7100e010a0(UnkGameManager* self, u32 value) {
    UnkTgmRefVector_b205* refs = fn_71006e9fb4();
    u32 count = refs->count;
    for (UnkTgmRef_b205** it = refs->data, **end = it + count; it != end; ++it) {
        UnkTgmRef_b205* ref = *it;
        if (!ref)
            continue;
        ++ref->ref_count;
        if (UnkTgmHolder_b205* holder = ref->target) {
            if (holder->entity) {
                if (UnkTgmModel_b205* model = reinterpret_cast<UnkTgmModel_b205*>(
                        fn_7100233684(static_cast<UnkStruct_7100233684*>(holder->entity)))) {
                    if (UnkTgmModelState_b205* state = model->state) {
                        if (state->field_0x11c)
                            state->field_0x114 = value & 1;
                    }
                }
            }
        }
        if (--ref->ref_count == 0)
            delete ref;
    }
}

// ---- fn_7100dfaf88
struct UnkStepCallbacks;
struct UnkStepCallback;
bool fn_7100dfc0c8(UnkGameManager* self, const base::UnkNamedEntry* a, const base::UnkNamedEntry* b, bool c, bool d,
                   bool e, bool f, bool g, s32 mode);
struct UnkTgmCallback_b205 {
    void* context;
    void (*fn)(void* context);
};
struct UnkTgmMantain_b205 {
    u8 pad_0x0[0x2948];
    u8 field_0x2948[8];
    u32 field_0x2950;
};
struct UnkSoundGlobal_b205 {
    u8 pad_0x0[0xb8];
    u8 field_0xb8;
};
struct UnkManager;
extern UnkManager* g_unk_71726bc570;

bool fn_7100dfaf88(UnkGameManager* manager, void* name, const CRntString& tag, bool flag) {
    UnkTgmMantain_b205* self = reinterpret_cast<UnkTgmMantain_b205*>(manager);
    if (reinterpret_cast<UnkSoundGlobal_b205*>(g_unk_71726bc570)->field_0xb8)
        return false;
    for (u32 i = 0; i < self->field_0x2950; ++i) {
        UnkTgmCallback_b205* cb = reinterpret_cast<UnkTgmCallback_b205*>(
            fn_7100dfc428(reinterpret_cast<UnkStepCallbacks*>(self->field_0x2948), i));
        cb->fn(cb->context);
    }
    return fn_7100dfc0c8(manager, static_cast<const base::UnkNamedEntry*>(name),
                         reinterpret_cast<const base::UnkNamedEntry*>(&tag), flag, false, true, true, false, 1);
}

// ---- fn_7100e015b8
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608* scenario);
void fn_7100ca7a60(UnkGameManagerSub_0x1af8*, CBlackboard*, const CStrId*);

void fn_7100e015b8(UnkGameManager* self) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    UnkGameManagerSub_0x1af8* sub = self->field_0x1af8;
    CStrId id = self->field_0x608 ? fn_71011a2fbc(self->field_0x608) : g_unk_7101d470e0;
    fn_7100ca7a60(sub, blackboard, &id);
}

// ---- fn_7100deb830
struct UnkTgmCharClass_b205 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x90];
    u64 field_0x98;
};
struct UnkTgmEntityState_b205 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};
struct UnkTgmEntityObj_b205 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x20];
    UnkTgmCharClass_b205* field_0x28;
    u8 pad_0x30[0x188];
    UnkTgmEntityState_b205* field_0x1b8;
};
struct UnkTgmEntityNode_b205 {
    UnkTgmEntityObj_b205* object;
    UnkTgmEntityNode_b205* next;
};
struct UnkTgmScenario_b205 {
    u8 pad_0x0[0x2b0];
    UnkTgmEntityNode_b205* field_0x2b0;
};
struct UnkTgmScenarioOwner_b205 {
    u8 pad_0x0[0x608];
    UnkTgmScenario_b205* field_0x608;
};

s32 fn_7100deb830(UnkGameManager* manager, const CStrId& id) {
    UnkTgmScenarioOwner_b205* self = reinterpret_cast<UnkTgmScenarioOwner_b205*>(manager);
    s32 count = 0;
    for (UnkTgmEntityNode_b205* n = self->field_0x608->field_0x2b0; n; n = n->next) {
        UnkTgmEntityObj_b205* object = n->object;
        if (!object)
            continue;
        if (!fn_710009d50c(object->vf_0x0(), fn_7100d70c20()))
            continue;
        UnkTgmCharClass_b205* component = object->field_0x28;
        UnkTgmCharClass_b205* typed =
            fn_710009d50c(component->vf_0x0(), fn_71011abbd4()) ? component : nullptr;
        if (typed->field_0x98 != reinterpret_cast<u64>(id.data))
            continue;
        if (UnkTgmEntityState_b205* state = object->field_0x1b8) {
            if (state->field_0x70 != 2)
                ++count;
        }
    }
    return count;
}

// ---- fn_7100defd08
struct UnkTgmIdNode_b205 {
    base::global::CFilePathStrId id;
    u8* object;
    UnkTgmIdNode_b205* next;
};
struct UnkTgmIdSet_b205 {
    base::UnkAllocator* allocator;
    u8 pad_0x8[4];
    u32 count;
    void* buckets;
    u8 pad_0x18[4];
    u32 capacity;
    u8 pad_0x20[0x18];
    u64 field_0x38;
    UnkTgmIdNode_b205* head;
};
struct UnkTgmIdSetOwner_b205 {
    u8 pad_0x0[0x29c8];
    UnkTgmIdSet_b205 set;
};
static_assert(__builtin_offsetof(UnkTgmIdSetOwner_b205, set.head) == 0x2a08, "layout");

// CStrId::Reset on the file-path pool.
static inline void ResetPath(base::global::CFilePathStrId& id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
        id.data = &pool->empty;
    }
}

void fn_7100defd08(UnkGameManager* manager) {
    UnkTgmIdSetOwner_b205* self = reinterpret_cast<UnkTgmIdSetOwner_b205*>(manager);
    for (UnkTgmIdNode_b205* n = self->set.head; n; n = n->next) {
        u8* object = n->object;
        fn_71002199d8(g_unk_7101d47300,
                      object ? reinterpret_cast<UnkItemBase_71011a91c4*>(object + 8) : nullptr, 0);
    }
    if (self->set.count) {
        for (UnkTgmIdNode_b205* n = self->set.head; n;) {
            UnkTgmIdNode_b205* next = n->next;
            ResetPath(n->id);
            self->set.allocator->vf_0x48(n);
            n = next;
        }
        memset(self->set.buckets, 0, self->set.capacity * u64(8));
        self->set.count = 0;
        self->set.field_0x38 = 0;
        self->set.head = nullptr;
    }
}

// ---- fn_7100dfecec
struct UnkTgmStrVector_b205 {
    CRntString* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkTgmStrVector_b205(u32 step_, u32 flags_)
        : data(nullptr), count(0), capacity(0), step(step_), field_0x14(false), flags(flags_),
          allocator(fn_7100083990()) {}
    void Free() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                fn_710007ecd4(&data[i]);
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
struct UnkStrVector;
s32 fn_7100dfeb10(UnkGameManager* self, bool flag);
void fn_7100e0621c();
struct UnkTgmHooks_b205 {
    u8 pad_0x0[0x1ab0];
    UnkGameManager* field_0x1ab0;
    void (*field_0x1ab8)();
    u8 field_0x1ac0[1];
};

s32 fn_7100dfecec(UnkGameManager* manager, s32 flag) {
    UnkTgmHooks_b205* self = reinterpret_cast<UnkTgmHooks_b205*>(manager);
    UnkTgmStrVector_b205 empty(0, 0xf);
    self->field_0x1ab0 = manager;
    self->field_0x1ab8 = fn_7100e0621c;
    fn_71000e0524(reinterpret_cast<UnkStrVector*>(self->field_0x1ac0), reinterpret_cast<UnkStrVector*>(&empty));
    s32 result = fn_7100dfeb10(manager, flag & 1);
    empty.Free();
    return result;
}

// ---- fn_7100dfc344
struct UnkStepCallbacks;
struct UnkStepCallback;
struct UnkCompared_0x30;
bool fn_7100dfc0c8(UnkGameManager* self, const base::UnkNamedEntry* a, const base::UnkNamedEntry* b, bool c, bool d,
                   bool e, bool f, bool g, s32 mode);
void fn_7100dfc638(UnkGameManager* self, CBlackboard* blackboard, const CRntString* name, const CRntString* tag);
struct UnkTgmAfter_b205 {
    u8 pad_0x0[0x968];
    CBlackboard field_0x968;
    u8 pad_0xa58[0xe88 - 0xa58];
    u8 field_0xe88[1];
    u8 pad_0xe89[0x2970 - 0xe89];
    u8 field_0x2970[8];
    u32 field_0x2978;
};

bool fn_7100dfc344(UnkGameManager* manager, const char* const& a, const char* const& b, const CRntString& name,
                   bool flag) {
    UnkTgmAfter_b205* self = reinterpret_cast<UnkTgmAfter_b205*>(manager);
    if (!fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&a),
                       reinterpret_cast<const UnkCompared_0x30*>(self->field_0xe88)))
        return false;
    for (u32 i = 0; i < self->field_0x2978; ++i) {
        UnkTgmCallback_b205* cb = reinterpret_cast<UnkTgmCallback_b205*>(
            fn_7100dfc428(reinterpret_cast<UnkStepCallbacks*>(self->field_0x2970), i));
        cb->fn(cb->context);
    }
    auto run = reinterpret_cast<bool (*)(UnkGameManager*, const base::UnkNamedEntry*, const base::UnkNamedEntry*,
                                         bool, bool, bool, bool, u32, s32)>(fn_7100dfc0c8);
    if (!run(manager, reinterpret_cast<const base::UnkNamedEntry*>(&b),
             reinterpret_cast<const base::UnkNamedEntry*>(&name), flag, false, true, false, 3, 1))
        return false;
    fn_7100dfc638(manager, &self->field_0x968,
                  reinterpret_cast<const CRntString*>(&b), &name);
    return true;
}

// ---- fn_7100dffc58 / fn_7100dffe04
extern UnkPropertyEx<> g_unk_7172759d90;
bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
struct UnkTgmBlackboards_b205 {
    u8 pad_0x0[0x820];
    CBlackboard field_0x820;
};

static inline UnkQualifiedId& Qid_b205(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

s32 fn_7100dffc58(UnkGameManager* manager) {
    UnkTgmBlackboards_b205* self = reinterpret_cast<UnkTgmBlackboards_b205*>(manager);
    s32 value = -1;
    CBlackboard* blackboard = &self->field_0x820;
    const CStrId& section_id = GameManagerIdTable().field_0x2050;
    UnkQualifiedId& name = Qid_b205(g_unk_7172759d90);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_7100395a30(section, &name, &value);
    return static_cast<u32>(value) < 4 ? value : 0;
}

void fn_7100dffe04(UnkGameManager* manager, s32 value) {
    UnkTgmBlackboards_b205* self = reinterpret_cast<UnkTgmBlackboards_b205*>(manager);
    CBlackboard* blackboard = &self->field_0x820;
    const CStrId& section_id = GameManagerIdTable().field_0x2050;
    UnkQualifiedId& name = Qid_b205(g_unk_7172759d90);
    UnkTypedValue typed;
    s32 stored = value;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            typed.type = fn_7100014768();
            typed.value = &stored;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &typed);
        }
    }
}

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);
extern UnkPropertyEx<> g_unk_7172759178;

// ---- fn_7100e00f74
struct UnkTgmFlagOwner_b205 {
    u8 pad_0x0[0x968];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x1ea8 - 0xa58];
    u8 field_0x1ea8[1];
    u8 pad_0x1ea9[0x2442 - 0x1ea9];
    bool field_0x2442;
};

void fn_7100e00f74(UnkGameManager* manager, bool value) {
    UnkTgmFlagOwner_b205* self = reinterpret_cast<UnkTgmFlagOwner_b205*>(manager);
    bool* flag = &self->field_0x2442;
    if (*flag == value)
        return;
    *flag = value;
    fn_71000303c0(reinterpret_cast<UnkDeferredSet<UnkDeferredCallback<bool>>*>(self->field_0x1ea8), flag);
    CBlackboard* blackboard = &self->field_0x968;
    const CStrId& section_id = GameManagerIdTable().field_0x1150;
    UnkQualifiedId& name = Qid_b205(g_unk_7172759178);
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue typed;
            typed.type = fn_710000d228();
            typed.value = flag;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &typed);
        }
    }
}

// ---- fn_7100de9ae4
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
CBlackboard* fn_710120384c(UnkUserDataOwner* self);
bool fn_7100a68fb0(CBlackboard::CSection* section, const void* name, u32* out);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

s32 fn_7100de9ae4() {
    u32 value = 0;
    CBlackboard* data = fn_710120384c(GetOwner());
    {
        CStrId section_id("USER_DATA", true);
        CStrId name("GameCompletedAmount", true);
        if (CBlackboard::CSection* section = fn_7101047270(data, section_id))
            fn_7100a68fb0(section, &name, &value);
    }
    return value;
}


// ---- fn_7100dee270
struct UnkCompared_0x30;
void fn_7100d7afa8(void* player, CRntString* previous, const CRntString* next);

struct UnkTgmMode_b205 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28(CRntString* previous);
    virtual void vf_0x30(const CRntString* next);
    CRntString name;
};
struct UnkTgmModeNode_b205 {
    UnkTgmMode_b205* mode;
    UnkTgmModeNode_b205* next;
};
struct UnkTgmPlayerHandle_b205 {
    u8 pad_0x0[0x10];
    void* player;
};
struct UnkTgmModes_b205 {
    u8 pad_0x0[0x438];
    CRntString field_0x438;
    u8 pad_0x468[0x478 - 0x468];
    UnkTgmModeNode_b205* field_0x478;
    u8 pad_0x480[0x498 - 0x480];
    UnkTgmMode_b205* field_0x498;
    u8 pad_0x4a0[0x5c8 - 0x4a0];
    bool field_0x5c8;
    u8 pad_0x5c9[7];
    CRntString field_0x5d0;
    u8 pad_0x600[0x29b8 - 0x600];
    UnkTgmPlayerHandle_b205* field_0x29b8;
};

void fn_7100dee270(UnkGameManager* manager) {
    UnkTgmModes_b205* self = reinterpret_cast<UnkTgmModes_b205*>(manager);
    if (!self->field_0x5c8)
        return;
    CRntString wanted(self->field_0x5d0);
    UnkTgmMode_b205* found = nullptr;
    for (UnkTgmModeNode_b205* n = self->field_0x478; n; n = n->next) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&n->mode->name),
                          reinterpret_cast<const UnkCompared_0x30*>(&wanted))) {
            found = n->mode;
            break;
        }
    }
    if (found && found != self->field_0x498) {
        if (self->field_0x498) {
            const CRntString* current = &self->field_0x498->name;
            fn_710007eaa0(&self->field_0x438, current);
            self->field_0x498->vf_0x30(&wanted);
        }
        if (self->field_0x29b8 && self->field_0x29b8->player)
            fn_7100d7afa8(self->field_0x29b8->player, &self->field_0x438, &wanted);
        self->field_0x5c8 = false;
        fn_710007e77c(&self->field_0x5d0, g_unk_710162e2ed);
        self->field_0x498 = found;
        found->vf_0x28(&self->field_0x438);
    } else {
        self->field_0x5c8 = false;
        fn_710007e77c(&self->field_0x5d0, g_unk_710162e2ed);
    }
}

// ---- the handle walk shared by fn_7100dffd48 and fn_7100dfff10
struct UnkTgmEntity_b205;
void fn_7100d7b238(UnkTgmEntity_b205* entity, CBlackboard* blackboard);
void fn_7100f0bf10(void* self);
struct UnkTgmHandleTarget_b205 {
    u8 pad_0x0[0x10];
    UnkTgmEntity_b205* target;
};
struct UnkTgmHandles_b205 {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
    u8 pad_0x39[0x968 - 0x39];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x2478 - 0xa58];
    u8 field_0x2478[8];
    u8 pad_0x2480[0x2998 - 0x2480];
    UnkTgmHandleTarget_b205* field_0x2998;
    UnkTgmHandleTarget_b205* field_0x29a0;
    UnkTgmHandleTarget_b205* field_0x29a8;
    UnkTgmHandleTarget_b205* field_0x29b0;
    UnkTgmHandleTarget_b205* field_0x29b8;
};
static inline UnkTgmEntity_b205* TargetOf(UnkTgmHandleTarget_b205* h) { return h ? h->target : nullptr; }
void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);

static inline __attribute__((always_inline)) void ResetHandles(UnkTgmHandles_b205* self) {
    CBlackboard* blackboard = &self->field_0x968;
    UnkTgmEntity_b205* a = TargetOf(self->field_0x29b8);
    UnkTgmEntity_b205* c =
        a == TargetOf(self->field_0x2998) ? TargetOf(self->field_0x29a0) : TargetOf(self->field_0x2998);
    if (a)
        fn_7100d7b238(a, blackboard);
    if (c)
        fn_7100d7b238(c, blackboard);
    fn_7100f0bf10(self->field_0x38);
    bool on = true;
    fn_71000303c0(reinterpret_cast<UnkDeferredSet<UnkDeferredCallback<bool>>*>(self->field_0x2478), &on);
}

// ---- fn_7100dfff10
struct UnkTgmSub_b205;
void fn_7100df2b68(void* self);
void fn_7100e0005c(void* self);
void* fn_7100e0014c(void* self);
void fn_7100e003ac(void* self);
void fn_7100e00640(void* self);
void fn_710119deb8(UnkSubAreaGuiCache* self);
void fn_7100ed5a04(void* player);
void fn_7101193370(UnkActorDefFactory* factory, bool flag);
extern UnkSubAreaGuiCache* g_unk_71727254f8;
extern UnkActorDefFactory* g_unk_71726a0220;
extern void* g_unk_71726af0b0;
extern const u8 g_unk_710164bda8[] __attribute__((visibility("hidden")));

void fn_7100dfff10(UnkGameManager* manager) {
    UnkTgmHandles_b205* self = reinterpret_cast<UnkTgmHandles_b205*>(manager);
    fn_7100df2b68(self);
    fn_7100e0005c(self);
    fn_7100df492c(self, fn_7100e0014c(self), g_unk_710164bda8, nullptr);
    fn_7100e003ac(self);
    fn_7100e00640(self);
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    fn_710119deb8(cache);
    fn_7100ed5a04(g_unk_71726af0b0);
    ResetHandles(self);
    fn_7101193370(g_unk_71726a0220, true);
}

void fn_7100dffd48(UnkGameManager* manager) {
    ResetHandles(reinterpret_cast<UnkTgmHandles_b205*>(manager));
}

// ---- fn_7100e00900
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkFileRequestArgs {
    u64 field_0x0;
    u64 field_0x8;
};
void fn_7100089754(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 a, u32 b,
                   const UnkFileRequestArgs* args);
void fn_7100107b74(UnkSoundMgr* self, const CFilePathStrId* path);
void fn_71000898c4(base::file::UnkFileDevice* self);
extern const CStrId g_unk_71727bedd0 __attribute__((visibility("hidden")));

static inline base::file::UnkFileDevice* FileDevice() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 reads;
    };
};
template <class Id>
static inline const CRntString& IdText(const Id& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    return reinterpret_cast<UnkIdTextData*>(id.data)->str;
}
static inline const char* CStr(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

struct UnkTgmPlayers_b205 {
    u8 pad_0x0[0x2282];
    bool field_0x2282;
};

void fn_7100e00900(UnkGameManager* manager, bool load, bool unused) {
    UnkTgmPlayers_b205* self = reinterpret_cast<UnkTgmPlayers_b205*>(manager);
    if (!load || self->field_0x2282)
        return;
    self->field_0x2282 = load;
    char path[0x200];
    sprintf(path, "/packs/players/%s.pkg", CStr(IdText(g_unk_71727bedd0)));
    CFilePathStrId id(path, true);
    base::file::UnkFileDevice* device = FileDevice();
    UnkFileRequestArgs args = {0, 0};
    fn_7100089754(device, &id, 0, 9, &args);
    fn_7100107b74(g_unk_7101d4d820, &id);
    fn_71000898c4(FileDevice());
}

// ---- fn_7100de8e18
struct UnkIdHashData {
    u8 pad_0x0[0x30];
    u64 hash;
};
extern const u64 g_unk_71727bef58 __attribute__((visibility("hidden")));

bool fn_7100de8e18(const CStrId& id) {
    {
        UnkGameManagerIdTable& table = GameManagerIdTable();
        if (id.data == table.field_0x1f00.data)
            return true;
    }
    {
        UnkGameManagerIdTable& table = GameManagerIdTable();
        if (id.data == table.field_0x18a0.data)
            return true;
    }
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    return reinterpret_cast<UnkIdHashData*>(id.data)->hash == g_unk_71727bef58;
}
