// CCharClassEventPropComponent unit (tools/tu_map.py): fn_71007dc10c reloads the event prop's three
// flags (+0x78 under g_unk_717271ebc0, "EventLaunched" +0x79, "EventFinished" +0x7a) from the current
// scenario's blackboard section (keys at +0x58, count +0x60, values at +0x50; lookup as in
// CEventScenarioComponent_b46.cpp), then resumes: a finished event calls vtable slot 0x178, a started one
// not yet launched slot 0x168. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassEventPropComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkEventPropValue_b49 {
    CStrId id;
    u64 field_0x8;
};

struct UnkEventProp_b49 {
    void** vtable;
    u8 pad_0x8[0x48];
    UnkEventPropValue_b49* values;
    u64* keys;
    u32 count;
    u8 pad_0x64[0x14];
    bool started;
    bool launched;
    bool finished;

    __attribute__((always_inline)) void GetFlag(CBlackboard* blackboard, const CStrId& section_id,
                                                const UnkProperty& property, bool* out) {
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
        UnkEventPropValue_b49* v = values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_710006d11c(section, &v[index].id, out);
    }
    void vf_0x168() { reinterpret_cast<void (*)(UnkEventProp_b49*)>(vtable[0x168 / 8])(this); }
    void vf_0x178() { reinterpret_cast<void (*)(UnkEventProp_b49*)>(vtable[0x178 / 8])(this); }
};

void fn_71007dc10c(UnkEventProp_b49* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    self->GetFlag(blackboard, section_id, g_unk_717271ebc0, &self->started);
    self->GetFlag(blackboard, section_id, g_unk_717271ebe8, &self->launched);
    self->GetFlag(blackboard, section_id, g_unk_717271ec10, &self->finished);
    if (self->finished)
        self->vf_0x178();
    else if (self->started && !self->launched)
        self->vf_0x168();
}
