// Scales a value by 5 when the key's entry in the navmesh manager's current mesh
// (game/UnkNavMeshManager.h; the manager is created on first use) answers true. Skipped when
// the low 16 bits of the key are all ones or the flag at 0x611 of the owner's linked object is
// set.
#include "base/UnkVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkLinked {
    u8 pad_0x0[0x611];
    u8 field_0x611;
};

struct UnkOwner {
    u8 pad_0x0[0x29ee0];
    UnkLinked* field_0x29ee0;
};

void fn_710027a33c(UnkNavMesh* navmesh, u32 key);
bool fn_71002b005c();

f32 fn_7100569f34(UnkOwner* self, void* unused, u32 key, f32 value) {
    if ((~key & 0xffff) == 0)
        return value;
    if (self->field_0x29ee0->field_0x611)
        return value;
    UnkNavMeshManager* global = g_unk_71726a4510;
    if (!global) {
        global = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(global);
        g_unk_71726a4510 = global;
    }
    fn_710027a33c(global->navmesh, key);
    if (fn_71002b005c())
        value = value * 5.0f;
    return value;
}
