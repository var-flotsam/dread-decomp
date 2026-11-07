// CShootActivatorHidrogigaComponent (unit of CCharClassShootActivatorHidrogigaComponent, tools/tu_map.py):
// fn_7100897344 runs the base activator's step (fn_7100890ea4), then reads its flag from the blackboard
// section of the current scenario (the property's name looked up in the key list at +0x490, count
// +0x498, values at +0x488, as in CEventScenarioComponent_b46.cpp); when it is not set, fn_7100897458
// runs; the flag ends up in +0x603. fn_7100897be8 runs the base's other step (fn_7100891b08), reads the
// same flag and, when it is set, gives the owner (+0x30) the key g_unk_710164aef8 (fn_7100d7c828). Names
// and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_717272c420;

struct UnkHidrogigaActivator_b44a2;
void fn_7100897458(UnkHidrogigaActivator_b44a2* self);

struct UnkHidrogigaValue_b48 {
    CStrId id;
    u64 field_0x8;
};

extern u64 g_unk_710164aef8;

struct UnkHidrogigaActivator_b48 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x488 - 0x38];
    UnkHidrogigaValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x49c[0x603 - 0x49c];
    bool activated;
};

void fn_7100890ea4(UnkHidrogigaActivator_b48* self, void* arg);
void fn_7100891b08(UnkHidrogigaActivator_b48* self);

// `arg` (x1) reaches the base activator's step untouched in the original (tools/forwarded_args.py).
void fn_7100897344(UnkHidrogigaActivator_b48* self, void* arg) {
    fn_7100890ea4(self, arg);
    bool activated = false;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272c420.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkHidrogigaValue_b48* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_710006d11c(section, &values[index].id, &activated);
    if (!activated)
        fn_7100897458(reinterpret_cast<UnkHidrogigaActivator_b44a2*>(self));
    self->activated = activated;
}

void fn_7100897be8(UnkHidrogigaActivator_b48* self) {
    fn_7100891b08(self);
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, fn_71011a2fbc(manager->field_0x608));
    if (!section)
        return;
    bool activated = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272c420.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    bool found = fn_710006d11c(section, &self->values[index].id, &activated);
    if (activated && found)
        fn_7100d7c828(self->owner, &g_unk_710164aef8, false);
}
