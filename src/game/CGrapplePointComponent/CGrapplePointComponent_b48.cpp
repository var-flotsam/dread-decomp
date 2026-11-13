// CGrapplePointComponent unit (tools/tu_map.py): fn_7100822b4c stores the point's "Position" (a
// base::math::CVector3D) in the given blackboard's section of the current scenario, unless the blackboard is
// locked: the property's name looked up in the key list at +0x60 (count +0x68, values at +0x58) as in
// CEventScenarioComponent_b46.cpp, the setter as in CDoorLifeComponent.cpp. Names and layouts are
// placeholders. fn_7100822a30 stores the point's flag (g_unk_71727235d0) the same way, as a bool.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CGrapplePointComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// Stores the vector at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionVector(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                    base::math::CVector3D* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_71000246e4();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
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

struct UnkGrapplePointValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkGrapplePoint_b48 {
    u8 pad_0x0[0x58];
    UnkGrapplePointValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_7100822b4c(UnkGrapplePoint_b48* self, CBlackboard* blackboard, base::math::CVector3D* position) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_71727235f8.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkGrapplePointValue_b48* values = self->values;
    SetSectionVector(blackboard, section_id, &values[index].name, position);
}

void fn_7100822a30(UnkGrapplePoint_b48* self, CBlackboard* blackboard, u32 value) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_71727235d0.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkGrapplePointValue_b48* values = self->values;
    bool flag = value != 0;
    SetSectionBool(blackboard, section_id, &values[index].name, &flag);
}
