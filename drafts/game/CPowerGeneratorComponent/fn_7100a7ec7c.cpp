// CPowerGeneratorComponent unit (tools/tu_map.py unit 1588): stores a value at +0x7c, calls virtual slot 17
// unless the flag at +0x78 is set, then sends the owner's sub-object at +0x220 one of two fixed hashes:
// the "already used" one when the "Used" flag (+0x138) is set, else the first-use one, after which it sets
// the flag and stores it in the blackboard section of the current scenario. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CPowerGeneratorComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkPowerGeneratorOwner_la7ec7c {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkPowerGeneratorValue_la7ec7c {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkPowerGenerator_la7ec7c {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    u8 pad_0x8[0x28];
    UnkPowerGeneratorOwner_la7ec7c* owner;  // 0x30
    u8 pad_0x38[0x40];
    bool field_0x78;
    f32 field_0x7c;
    u8 pad_0x80[0x10];
    UnkPowerGeneratorValue_la7ec7c* values;  // 0x90
    u64* keys;                               // 0x98
    u32 count;                               // 0xa0
    u8 pad_0xa4[0x138 - 0xa4];
    bool used;  // 0x138
};
static_assert(__builtin_offsetof(UnkPowerGenerator_la7ec7c, used) == 0x138, "layout");

// Stores a bool under `name` in the blackboard section `section` names (unless the blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7100a7ec7c(UnkPowerGenerator_la7ec7c* self, f32 value) {
    self->field_0x7c = value;
    if (!self->field_0x78)
        self->vf_17();
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    if (self->used) {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0xa2eb97914549d415, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
        return;
    }
    self->used = true;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x290249f84e3a58fd, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_71727502f8.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkPowerGeneratorValue_la7ec7c* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, true);
}
