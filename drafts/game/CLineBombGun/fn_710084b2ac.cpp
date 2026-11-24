// CLineBombGun's unit (tools/tu_map.py unit 1307): laying a line bomb. The actor definition named by the
// tweak track's id (or the id table's id at +0x1638) is loaded through the actordef factory; when it is an
// entity with a CLineBombMovement, the movement is set up from the gun's owner and tunables, aimed at the
// owner's target, handed to a boss AI, and keyed. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkBombAimMovement_ovb0078;
struct UnkWeakTarget;
void CBombMovement_SetTargetAndCacheAngle(UnkBombAimMovement_ovb0078* self, UnkWeakTarget* target);

struct UnkRefl_fn_710084b690;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710084b690>(void*);

extern const u64 g_unk_710164acf8;

struct alignas(8) UnkLineBombDefType_l84b2ac {
    u32 id;
};

// A counted object: its count at +8, released through its vtable's +0x10 slot.
struct UnkCounted_l84b2ac {
    void** vtable;
    s32 refs;  // 0x8
    u8 pad_0xc[0x4];
    UnkReflObject* target;  // 0x10

    void Release() { reinterpret_cast<void (*)(UnkCounted_l84b2ac*)>(vtable[0x10 / 8])(this); }
};

struct UnkCountedRef_l84b2ac {
    UnkCounted_l84b2ac* ptr;

    UnkCountedRef_l84b2ac() : ptr(nullptr) {}
    UnkCountedRef_l84b2ac(const UnkCountedRef_l84b2ac& other) : ptr(other.ptr) {
        if (ptr)
            ++ptr->refs;
    }
    ~UnkCountedRef_l84b2ac() {
        if (ptr && --ptr->refs == 0)
            ptr->Release();
    }
};

struct UnkTargetPair_l84b2ac {
    UnkCountedRef_l84b2ac first;   // 0x0
    UnkCountedRef_l84b2ac second;  // 0x8
};

struct UnkLineBombTargets_l84b2ac {
    u8 pad_0x0[0x128];
    UnkTargetPair_l84b2ac pair;  // 0x128
};

struct UnkLineBombOwner_l84b2ac {
    u8 pad_0x0[0x1f0];
    UnkLineBombTargets_l84b2ac* targets;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkLineBombOwner_l84b2ac, animation) == 0x220, "layout");

struct UnkLineBombGun_l84b2ac {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLineBombOwner_l84b2ac* owner;  // 0x18

    bool vf_0x48() { return reinterpret_cast<bool (*)(UnkLineBombGun_l84b2ac*)>(vtable[0x48 / 8])(this); }
    f32 vf_0x180() { return reinterpret_cast<f32 (*)(UnkLineBombGun_l84b2ac*)>(vtable[0x180 / 8])(this); }
    s32 vf_0x198() { return reinterpret_cast<s32 (*)(UnkLineBombGun_l84b2ac*)>(vtable[0x198 / 8])(this); }
    f32 vf_0x1a0() { return reinterpret_cast<f32 (*)(UnkLineBombGun_l84b2ac*)>(vtable[0x1a0 / 8])(this); }
    f32 vf_0x1a8() { return reinterpret_cast<f32 (*)(UnkLineBombGun_l84b2ac*)>(vtable[0x1a8 / 8])(this); }
};

struct UnkBombLink_l84b2ac {
    u64 field_0x0;
    u32 field_0x8;

    UnkBombLink_l84b2ac() : field_0x0(0), field_0x8(0) {}
};

struct UnkLineBombMovement_l84b2ac {
    void** vtable;
    u8 pad_0x8[0x340];
    u8 request[0xb4];  // 0x348
    s32 field_0x3fc;
    u8 pad_0x400[0x8];
    f32 field_0x408;
    f32 field_0x40c;

    void vf_0x2c8(UnkLineBombOwner_l84b2ac* owner, UnkBombLink_l84b2ac* a, UnkBombLink_l84b2ac* b) {
        reinterpret_cast<void (*)(UnkLineBombMovement_l84b2ac*, UnkLineBombOwner_l84b2ac*, UnkBombLink_l84b2ac*,
                                  UnkBombLink_l84b2ac*)>(vtable[0x2c8 / 8])(this, owner, a, b);
    }
    void vf_0x290(UnkLineBombOwner_l84b2ac* owner) {
        reinterpret_cast<void (*)(UnkLineBombMovement_l84b2ac*, UnkLineBombOwner_l84b2ac*)>(vtable[0x290 / 8])(this,
                                                                                                          owner);
    }
};
static_assert(__builtin_offsetof(UnkLineBombMovement_l84b2ac, field_0x40c) == 0x40c, "layout");

void fn_7100ac7db4(UnkLineBombMovement_l84b2ac* self, const char* name, bool flag);

struct UnkLineBombEntity_l84b2ac : UnkReflObject {
    void vf_0xd0(UnkBombLink_l84b2ac* link, bool flag) {
        reinterpret_cast<void (*)(UnkLineBombEntity_l84b2ac*, UnkBombLink_l84b2ac*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd0 / 8])(this, link, flag);
    }
    void vf_0xd8(UnkBombLink_l84b2ac* link, bool flag) {
        reinterpret_cast<void (*)(UnkLineBombEntity_l84b2ac*, UnkBombLink_l84b2ac*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd8 / 8])(this, link, flag);
    }
};

struct UnkBossAI_l84b2ac : UnkReflObject {
    void vf_0x7c0(UnkLineBombEntity_l84b2ac* bomb) {
        reinterpret_cast<void (*)(UnkBossAI_l84b2ac*, UnkLineBombEntity_l84b2ac*)>(
            (*reinterpret_cast<void***>(this))[0x7c0 / 8])(this, bomb);
    }
};

struct UnkBossActor_l84b2ac {
    u8 pad_0x0[0x240];
    UnkBossAI_l84b2ac* ai;  // 0x240
};

struct UnkBossHolder_l84b2ac {
    u8 pad_0x0[0x10];
    UnkBossActor_l84b2ac* actor;  // 0x10
};

static inline u32 TargetFlags(UnkReflObject* target) {
    return reinterpret_cast<u32 (*)(UnkReflObject*)>((*reinterpret_cast<void***>(target))[0x58 / 8])(target);
}

// Has the actordef factory load the definition the id names.
static inline UnkLineBombEntity_l84b2ac* LoadDef(UnkActorDefFactory* factory, CStrId& id) {
    u32 type_id = g_unk_7172a0ba48;
    const char* path = id.Text();
    UnkLineBombDefType_l84b2ac type;
    type.id = type_id;
    return reinterpret_cast<UnkLineBombEntity_l84b2ac*>(
        fn_710119438c(factory, path, &id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
}

bool fn_710084b2ac(UnkLineBombGun_l84b2ac* self) {
    if (!self->owner || !self->vf_0x48())
        return false;
    UnkTrackQuery query;
    query.cls = fn_7100c5ce98();
    query.result = nullptr;
    query.value = -1.0f;
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117b08c(animation, &query);
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkLineBombEntity_l84b2ac* entity;
    if (query.result)
        entity = LoadDef(factory, *reinterpret_cast<CStrId*>(reinterpret_cast<char*>(query.result) + 0x18));
    else
        entity = LoadDef(factory, *reinterpret_cast<CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1638));
    if (!entity || !fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()))
        return false;
    UnkLineBombMovement_l84b2ac* movement = reinterpret_cast<UnkLineBombMovement_l84b2ac*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_710084b690>(entity));
    if (!movement)
        return true;

    UnkBombLink_l84b2ac first;
    UnkBombLink_l84b2ac second;
    movement->vf_0x2c8(self->owner, &first, &second);
    entity->vf_0xd0(&first, true);
    entity->vf_0xd8(&second, true);
    fn_7100ac7db4(movement, "Bomb", false);
    movement->vf_0x290(self->owner);
    fn_71008c1538(reinterpret_cast<UnkRequest*>(movement->request), self->vf_0x180());
    movement->field_0x3fc = self->vf_0x198();
    movement->field_0x408 = self->vf_0x1a0();
    movement->field_0x40c = self->vf_0x1a8();

    UnkLineBombTargets_l84b2ac* targets = self->owner->targets;
    UnkTargetPair_l84b2ac pair = targets ? targets->pair : UnkTargetPair_l84b2ac();
    if (pair.first.ptr) {
        UnkReflObject* target = pair.first.ptr->target;
        if (target && (TargetFlags(target) & 0x8000))
            CBombMovement_SetTargetAndCacheAngle(reinterpret_cast<UnkBombAimMovement_ovb0078*>(movement),
                          reinterpret_cast<UnkWeakTarget*>(target));
    }

    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    if (UnkBossHolder_l84b2ac* holder = *reinterpret_cast<UnkBossHolder_l84b2ac**>(manager + 0x100)) {
        if (UnkBossActor_l84b2ac* actor = holder->actor) {
            if (UnkBossAI_l84b2ac* ai = actor->ai) {
                if (fn_710009d50c(ai->vf_0x0(), fn_71003ab524()))
                    ai->vf_0x7c0(entity);
            }
        }
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(entity), &g_unk_710164acf8, false);
    return true;
}
