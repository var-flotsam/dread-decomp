// CMultiLockOnBlockComponent unit (tools/tu_map.py): fn_7100882bac reads the block's flag from the
// blackboard section of the current scenario (the empty id without a scenario; fn_71010472e0 creates the
// section when missing): the property's name is looked up in the key list at +0x88 (count +0x90, values at
// +0x80) as in CEventScenarioComponent_b46.cpp. False when it is not found. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_717272b638;

struct UnkMultiLockOnValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkMultiLockOnBlock_b48 {
    u8 pad_0x0[0x80];
    UnkMultiLockOnValue_b48* values;
    u64* keys;
    u32 count;
};

bool fn_7100882bac(UnkMultiLockOnBlock_b48* self) {
    bool value = false;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkGameManagerSub_0x608* scenario = manager->field_0x608;
    CBlackboard::CSection* section =
        fn_71010472e0(&manager->field_0x968, scenario ? fn_71011a2fbc(scenario) : CStrId(g_unk_7101d470e0));
    if (section) {
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)g_unk_717272b638.name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        fn_710006d11c(section, &self->values[index].id, &value);
    }
    return value;
}
