// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): initialisation. Runs the base step, keeps the
// owner's object at +0x149a0 when it is a CCharClassHydrogigaAIComponent, registers its five attacks (each
// initialised from its part of that object), and, unless the scenario's blackboard says LUA:HYDROGIGA_DEAD,
// registers with the sub-area manager. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkHydrogigaAttack;
void fn_71005b3978(UnkHydrogigaAttack* self, UnkReflObject* arg);
void CHydrogigaMaelstormAttack_InitAndAddTentacleIds(UnkHydrogigaAttack* self, UnkReflObject* arg);
void fn_71005b542c(UnkHydrogigaAttack* self, UnkReflObject* arg);
void CHydrogigaTentacleBashAttack_InitFromCharClass(UnkHydrogigaAttack* self, UnkReflObject* arg);
void CHydrogigaTongueSwirlAttack_InitFromCharClass(UnkHydrogigaAttack* self, UnkReflObject* arg);

struct UnkStrIdHandle {
    union {
        CStrId id;
    };
    void* field_0x8;
    UnkStrIdHandle() {}
    ~UnkStrIdHandle() { id.~CStrId(); }
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// CCharClassHydrogigaAIComponent: the class data of each attack.
struct UnkHydrogigaCharClass_l5bb568 {
    u8 pad_0x0[0x738];
    u8 field_0x738[0x830 - 0x738];
    u8 maelstorm[0x920 - 0x830];  // +0x830
    u8 field_0x920[0xa98 - 0x920];
    u8 tentacle_bash[0xb88 - 0xa98];  // +0xa98
    u8 tongue_swirl[0x8];             // +0xb88
};

// The component's attacks, as they are registered (the array is only 4-aligned).
struct __attribute__((packed, aligned(4))) UnkHydrogigaAttackList_l5bb568 {
    u32 count;
    UnkHydrogigaAttack* items[8];

    void Add(UnkHydrogigaAttack* attack) {
        UnkHydrogigaAttack** data = items;
        data[count] = attack;
        ++count;
    }
};

struct UnkHydrogigaAI_l5bb568 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // +0x10
    u8 pad_0x18[0x1618 - 0x18];
    UnkHydrogigaAttackList_l5bb568 attacks;  // +0x1618
    u8 pad_0x1660[0x149a0 - 0x1660];
    UnkHydrogigaCharClass_l5bb568* char_class;  // +0x149a0
    u8 pad_0x149a8[0x14dd8 - 0x149a8];
    void* registration;  // +0x14dd8
    u8 pad_0x14de0[0x14e00 - 0x14de0];
    u8 attack_0[0x90];  // +0x14e00
    u8 attack_1[0x68];  // +0x14e90
    u8 attack_2[0x78];  // +0x14ef8
    u8 attack_3[0x68];  // +0x14f70
    u8 attack_4[0x8];   // +0x14fd8
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_l5bb568, char_class) == 0x149a0, "layout");
static_assert(__builtin_offsetof(UnkHydrogigaAI_l5bb568, attack_4) == 0x14fd8, "layout");

// An embedded attack, and the part of the class data it is initialised from.
static inline UnkHydrogigaAttack* Attack(u8* storage) {
    return reinterpret_cast<UnkHydrogigaAttack*>(storage);
}

static inline UnkReflObject* Data(u8* part) {
    return reinterpret_cast<UnkReflObject*>(part);
}

static inline UnkManager* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkManager*>(manager);
}

// `arg` reaches the base step untouched.
u32 fn_71005bb568(UnkHydrogigaAI_l5bb568* self, void* arg) {
    u32 result = fn_71003ac8f4(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    UnkReflObject* object = self->object;
    self->char_class = object && fn_710009d50c(object->vf_0x0(), fn_71005b8aac())
                           ? reinterpret_cast<UnkHydrogigaCharClass_l5bb568*>(object)
                           : nullptr;
    self->attacks.Add(Attack(self->attack_0));
    fn_71005b3978(Attack(self->attack_0), Data(self->char_class->field_0x738));
    self->attacks.Add(Attack(self->attack_1));
    CHydrogigaMaelstormAttack_InitAndAddTentacleIds(Attack(self->attack_1), Data(self->char_class->maelstorm));
    self->attacks.Add(Attack(self->attack_2));
    fn_71005b542c(Attack(self->attack_2), Data(self->char_class->field_0x920));
    self->attacks.Add(Attack(self->attack_3));
    CHydrogigaTentacleBashAttack_InitFromCharClass(Attack(self->attack_3), Data(self->char_class->tentacle_bash));
    self->attacks.Add(Attack(self->attack_4));
    CHydrogigaTongueSwirlAttack_InitFromCharClass(Attack(self->attack_4), Data(self->char_class->tongue_swirl));

    {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkGameManagerSub_0x608* scenario = manager->field_0x608;
        CBlackboard* blackboard = &manager->field_0x968;
        UnkStrIdHandle key;
        {
            CStrId id("LUA:HYDROGIGA_DEAD", true);
            fn_7100ae986c(&key, id);
        }
        bool dead = false;
        {
            CStrId section = fn_71011a2fbc(scenario);
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                fn_710006d11c(s, &key.id, &dead);
        }
        if (!dead) {
            UnkManager* sub_areas = SubAreaManager();
            if (self->registration) {
                fn_71005c4828(self->registration, &self->registration);
                self->registration = nullptr;
            }
            self->registration = reinterpret_cast<u8*>(sub_areas) + 0xa60;
            fn_71005c41bc(self->registration, reinterpret_cast<UnkCallback*>(&self->registration));
        }
    }
    return result & 1;
}
