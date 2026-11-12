// CMagnetSurfaceHuskComponent unit (tools/tu_map.py): fn_7100925244 reads the husk's "Breakable" flag
// (g_unk_7172737e70, CPowerBombBlockLifeComponent_TUInit.cpp) from the given blackboard's section of the
// current scenario (created when missing), its name looked up in the key list at +0x70 (count +0x78, values
// at +0x68) as in CEventScenarioComponent_b46.cpp, and hands it to fn_7100925348 (false when it is not
// found). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CMagnetSurfaceHuskComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkMagnetSurfaceHuskValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkMagnetSurfaceHusk_b48 {
    u8 pad_0x0[0x68];
    UnkMagnetSurfaceHuskValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_7100925348(UnkMagnetSurfaceHusk_b48* self, bool breakable);

void fn_7100925244(UnkMagnetSurfaceHusk_b48* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool breakable = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172737e70.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    fn_710006d11c(section, &self->values[index].id, &breakable);
    fn_7100925348(self, breakable);
}
