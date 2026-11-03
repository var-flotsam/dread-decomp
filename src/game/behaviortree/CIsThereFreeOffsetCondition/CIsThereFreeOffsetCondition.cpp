// behaviortree::CIsThereFreeOffsetCondition: fn_71006360b8 is false while the condition has no
// owner (+0x18), else asks fn_710063c0bc about the part at +0x80 of the manager g_unk_71726af468
// (created on first use). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

bool fn_710063c0bc(u8* part);

struct UnkFreeOffsetCondition {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

bool fn_71006360b8(UnkFreeOffsetCondition* self) {
    if (!self->field_0x18)
        return false;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return fn_710063c0bc(manager->pad_0x0 + 0x80);
}
