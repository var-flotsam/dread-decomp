// CCoreXAIComponent's unit (tools/tu_map.py unit 759): after the boss step, disables the owner's
// component at +0x1f0, loads an X parasite actor definition at the owner's position, and sends its AI
// to a point above the middle of the arena (mode 3), stored in its blackboard as a CVector3D.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXParasiteAI.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
struct UnkPropertyName;

// The parasite's actor definition, and the property its target goes under.
extern CStrId g_unk_71726d90c0 __attribute__((visibility("hidden")));
extern UnkPropertyName g_unk_71726d90c8 __attribute__((visibility("hidden")));

// As src/game/CDaivoSpitAttack/fn_71004dc590.cpp defines it: points the parasite at its spawner.
struct UnkSelf_71006eb484;
void fn_71006eb484(UnkSelf_71006eb484* self, UnkWeakTarget* other);

struct UnkRefl_fn_71004daf78;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71004daf78>(void*);

struct UnkCoreXName_l4dadec {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkCoreXDefType_l4dadec {
    u32 id;
};

struct UnkCoreXVec_l4dadec {
    f32 x, y, z;
};

// What fn_71006e9384 is asked to do (only the kind is set here).
struct UnkXParasiteMode_A3_b75 {
    u32 kind;
    u32 unk_0x4;
    u64 unk_0x8;
};

struct UnkCoreXOwner_l4dadec {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    UnkStruct_710024fa0c* field_0x1f0;
};

struct UnkCoreXSpawned_l4dadec {
    void** vtable;

    void SetPosition(const UnkCoreXVec_l4dadec* position, bool flag) {
        reinterpret_cast<void (*)(UnkCoreXSpawned_l4dadec*, const UnkCoreXVec_l4dadec*, bool)>(
            vtable[0xd0 / 8])(this, position, flag);
    }
};

struct UnkCoreXAI_l4dadec {
    u8 pad_0x0[0x30];
    UnkCoreXOwner_l4dadec* owner;  // 0x30
    u8 pad_0x38[0x149a8 - 0x38];
    f32 field_0x149a8;  // the arena's left edge
    f32 field_0x149ac;  // its floor
    f32 field_0x149b0;  // its right edge
};
static_assert(__builtin_offsetof(UnkCoreXAI_l4dadec, field_0x149a8) == 0x149a8, "layout");

void fn_71004dadec(UnkCoreXAI_l4dadec* self, void* arg) {
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    if (UnkStruct_710024fa0c* component = self->owner->field_0x1f0)
        fn_710024fa0c(component, false);

    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkCoreXName_l4dadec* name = reinterpret_cast<UnkCoreXName_l4dadec*>(g_unk_71726d90c0.data);
    ++name->use_count;
    UnkCoreXDefType_l4dadec type;
    type.id = g_unk_7172a0ba48;
    UnkCoreXSpawned_l4dadec* spawned = reinterpret_cast<UnkCoreXSpawned_l4dadec*>(
        fn_710119438c(factory, name->text, &g_unk_71726d90c0,
                      reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (spawned) {
        {
            UnkCoreXOwner_l4dadec* owner = self->owner;
            UnkCoreXVec_l4dadec position = {owner->x, owner->y, owner->z};
            spawned->SetPosition(&position, true);
        }
        UnkXParasiteAI* ai = reinterpret_cast<UnkXParasiteAI*>(
            base::reflection::refl_FindTyped<UnkRefl_fn_71004daf78>(spawned));
        if (ai) {
            UnkCoreXVec_l4dadec target = {
                self->field_0x149a8 + (self->field_0x149b0 - self->field_0x149a8) * 0.5f,
                self->field_0x149ac + 500.0f, 0.0f};
            {
                UnkXParasiteMode_A3_b75 mode;
                mode.kind = 3;
                fn_71006e9384(ai, &mode);
            }
            XParasiteAI_SetCanBeAbsorbed(ai, false);
            ai->field_0x14724 = 4;
            fn_71006eb484(reinterpret_cast<UnkSelf_71006eb484*>(ai), reinterpret_cast<UnkWeakTarget*>(self));

            CBlackboard::CSection* section = fn_71010472e0(&ai->blackboard, g_unk_71726b7cb8);
            if (section && !ai->blackboard.field_0xe8) {
                UnkTypedValue value;
                value.type = fn_71000246e4();
                value.value = &target;
                fn_7100332c80(section, &g_unk_71726d90c8, &value);
            }
        }
    }
}
