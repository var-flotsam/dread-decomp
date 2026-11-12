// CCharClassPopSetupEvent (unit 1831, fn_7100bcb03c) and CCharClassPushSetupEvent (unit 1832, fn_7100bcbb30):
// unless the event's setup id (+0x10) is empty, hands the id and the flags at +0x18 and +0x19 to the
// manager g_unk_7101d47360 (created on first use): pop through fn_7100eea334, push through fn_7100eea3fc,
// as in src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_Step_b53.cpp. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkManager;
extern UnkManager* g_unk_7101d47360;

struct UnkSetupEvent_A5_b74 {
    void** vtable;
    u8 pad_0x8[0x8];
    base::global::CStrId id;  // 0x10
    bool field_0x18;
    bool field_0x19;
};

template <void (*Apply)(void*, const base::global::CStrId*, bool, bool)>
static inline __attribute__((always_inline)) void SetupEventApply_A5_b74(UnkSetupEvent_A5_b74* self) {
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    Apply(manager, &self->id, self->field_0x18, self->field_0x19);
}

void fn_7100bcb03c(UnkSetupEvent_A5_b74* self) {
    SetupEventApply_A5_b74<fn_7100eea334>(self);
}

void fn_7100bcbb30(UnkSetupEvent_A5_b74* self) {
    SetupEventApply_A5_b74<fn_7100eea3fc>(self);
}
