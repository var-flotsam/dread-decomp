// CChozoCommanderAIComponent's unit: a step that runs the base node routine fn_71003ace0c, releases the handle
// at +0x15ce0 through fn_71004567dc, calls fn_7100ed78c0 on the sound player g_unk_71726af0b0 and then hands the
// lazily created manager g_unk_71726af468 to fn_7100da928c (as fn_710048e38c). Names are placeholders.
#include "base/UnkVector.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBossAIComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

struct UnkNode;

struct UnkChozoCommanderAIComponent_A2_b69 {
    u8 pad_0x0[0x15ce0];
    void* handle;  // +0x15ce0
};

void fn_710044fff8(UnkChozoCommanderAIComponent_A2_b69* self) {
    fn_71003ace0c(reinterpret_cast<UnkNode*>(self));
    if (self->handle) {
        fn_71004567dc(self->handle, &self->handle);
        self->handle = nullptr;
    }
    fn_7100ed78c0(g_unk_71726af0b0);
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_7100da928c(manager);
}
