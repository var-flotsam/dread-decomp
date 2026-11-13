// CDoorCentralUnitLifeComponent's unit (tools/tu_map.py), fn_71008ca2d0: opens the door (fn_71008d0ef4), stores
// `true` as "StayOpened" (g_unk_7172731110) in the door's own blackboard section (fn_71008d2670) of the game
// manager's blackboard (unless it is locked), and sets +0x418. Keys at +0x678 (count +0x680, values at +0x670),
// looked up as in src/game/CDoorLifeComponent/CDoorLifeComponent_b49.cpp. fn_71008c99c8: after fn_71008d1604,
// rewrites "StayOpened" as `true` when the section already has it (fn_71010474f4). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkDoorCentralValue_A4_b76 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkDoorCentral_A4_b76 {
    u8 pad_0x0[0x418];
    bool field_0x418;
    u8 pad_0x419[0x670 - 0x419];
    UnkDoorCentralValue_A4_b76* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkDoorCentralValue_A4_b76* Find(const UnkProperty& property) {
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

CStrId fn_71008d2670(UnkDoorCentral_A4_b76* self);
void fn_71008d1604(UnkDoorCentral_A4_b76* self);
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);

void fn_71008ca2d0(UnkDoorCentral_A4_b76* self) {
    fn_71008d0ef4(self);
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71008d2670(self);
        UnkDoorCentralValue_A4_b76* name = self->Find(g_unk_7172731110);
        UnkTypedValue v;
        bool value = true;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, &name->name, &v);
            }
        }
    }
    self->field_0x418 = true;
}

void fn_71008c99c8(UnkDoorCentral_A4_b76* self) {
    fn_71008d1604(self);
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    bool known;
    {
        CStrId section_id = fn_71008d2670(self);
        known = fn_71010474f4(blackboard, section_id, self->Find(g_unk_7172731110)->id);
    }
    if (!known)
        return;
    CStrId section_id = fn_71008d2670(self);
    UnkDoorCentralValue_A4_b76* name = self->Find(g_unk_7172731110);
    bool value = true;
    SetSectionBool(blackboard, section_id, &name->name, &value);
}
