// CCharClassSetSubAreaSetupEvent's unit (tools/tu_map.py unit 2291): fn_7100cd9814 applies the event. Unless
// its setup id (+0x10) is empty it goes to the sub-area manager g_unk_7101d47360 (created on first use, as
// src/game/CCharClassPopSetupEvent/CCharClassSetupEvent_Apply_A5_b74.cpp): with no sub-area id (+0x18)
// through fn_7100eea3fc, otherwise through fn_7100eee1f8 with a copy of the sub-area id (the manager's
// current one at +0x3f0 when the event names the placeholder id g_unk_717279de58). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CStrId;

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
extern CStrId g_unk_717279de58 __attribute__((visibility("hidden")));

struct UnkSubAreaManager_ovb0136 {
    u8 pad_0x0[0x3f0];
    CStrId* field_0x3f0;
};

struct UnkSetSubAreaSetupEvent_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId setup;    // 0x10
    CStrId subArea;  // 0x18
    bool field_0x20;
};

static inline void* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

void fn_7100cd9814(UnkSetSubAreaSetupEvent_ovb0136* self) {
    if (self->setup.data == &g_unk_7101d47108->empty)
        return;
    if (self->subArea.data == &g_unk_7101d47108->empty) {
        fn_7100eea3fc(SubAreaManager(), &self->setup, self->field_0x20, false);
        return;
    }
    const CStrId* source = &self->subArea;
    if (self->subArea.data == g_unk_717279de58.data) {
        CStrId* current = static_cast<UnkSubAreaManager_ovb0136*>(SubAreaManager())->field_0x3f0;
        source = current ? current : &g_unk_7101d470e0;
    }
    CStrId subArea(*source);
    fn_7100eee1f8(SubAreaManager(), &subArea, &self->setup, self->field_0x20, false);
}
