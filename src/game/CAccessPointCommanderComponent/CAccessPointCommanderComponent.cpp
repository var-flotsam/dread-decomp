// CAccessPointCommanderComponent's static initialiser: the unit header, a default pair of -1.0
// values and a flag in the unit's data, the class's registration and three string ids built from
// literals with compile-time hashes.
#include "base/reflection/UnkClassGetters.h"
#include "UnkStrIdHashedInit.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkAccessPointCommanderTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId ids[3];
    float range[2];
    bool flag;
    UnkReflType type;
};

extern UnkAccessPointCommanderTU g_unk_717274ea88 __attribute__((visibility("hidden")));
extern char g_unk_717274ea78;

void fn_7100a6b440() {
    UnkAccessPointCommanderTU& tu = g_unk_717274ea88;
    InitHeader(tu.header);
    tu.range[0] = -1.0f;
    tu.range[1] = -1.0f;
    tu.flag = true;
    fn_7100a69610();
    fn_710009ec00(&g_unk_717274ea78, &tu.type);
    INIT_HASHED_STRID(&tu.ids[0], "collision_camera_005");
    fn_7100000250(fn_710000051c, &tu.ids[0], &g_unk_7101a19000);
    INIT_HASHED_STRID(&tu.ids[1], "collision_camera_004");
    fn_7100000250(fn_710000051c, &tu.ids[1], &g_unk_7101a19000);
    INIT_HASHED_STRID(&tu.ids[2], "AccessPointCommanderComponent");
    fn_7100000250(fn_710000051c, &tu.ids[2], &g_unk_7101a19000);
}
