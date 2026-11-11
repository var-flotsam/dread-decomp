// CThermalDeviceComponent unit (tools/tu_map.py, the name from reflection): unless the game manager's
// byte at +0x2441 is set, counts a use on the owner's record and hands the record to the manager when the
// component is armed (+0x59c) and the manager agrees, then runs the next step. Names and layouts are
// placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

bool fn_7100dfbe3c(UnkGameManager* manager);
bool fn_7100dfbe78(UnkGameManager* manager, void* record);

struct UnkThermalRecord {
    u8 pad_0x0[0x10];
    u8 field_0x10[0x34];
    u32 field_0x44;
};

struct UnkThermalOwner {
    u8 pad_0x0[0x30];
    UnkThermalRecord* field_0x30;
};

struct UnkThermalDeviceComponent {
    u8 pad_0x0[0x18];
    UnkThermalOwner* field_0x18;
    u8 pad_0x20[0x59c - 0x20];
    bool field_0x59c;
};

bool fn_7100aa122c(UnkThermalDeviceComponent* self, void* arg);

bool fn_7100a92834(UnkThermalDeviceComponent* self, void* arg) {
    if (reinterpret_cast<const u8*>(g_unk_7101d47310)[0x2441])
        return false;
    if (self->field_0x59c && fn_7100dfbe3c(g_unk_7101d47310)) {
        UnkThermalOwner* owner = self->field_0x18;
        UnkGameManager* manager = g_unk_7101d47310;
        ++*reinterpret_cast<a_u32*>(&owner->field_0x30->field_0x44);
        fn_7100dfbe78(manager, owner->field_0x30->field_0x10);
    }
    return fn_7100aa122c(self, arg);
}
