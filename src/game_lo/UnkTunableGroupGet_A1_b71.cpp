// The tunables' registry unit (src/base/UnkTunableRegistry.cpp, tools/tu_map.py unit 112): the group
// stored under a name in the keyed arrays at +0x30 (the lookup of src/game_lo/fn_710013c3c8.cpp), or a
// new one appended for it (fn_710013c2ac). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTunableGroup_A1b71 {
    u8 data[0x80];
};

struct UnkTunableGroupArrays_A1b71 {
    UnkTunableGroup_A1b71* values;
    CStrId* keys;
    u32 count;

    UnkTunableGroup_A1b71* End() { return &values[count]; }
    UnkTunableGroup_A1b71* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

UnkTunableGroup_A1b71* fn_710013c2ac(UnkTunableGroupArrays_A1b71* self, const CStrId* key);

struct UnkTunableRegistry_A1b71 {
    u8 pad_0x0[0x30];
    UnkTunableGroupArrays_A1b71 groups;  // 0x30
};

// The group, returned as a one-pointer handle (a struct in x0).
struct UnkTunableGroupRef_A1b71 {
    UnkTunableGroup_A1b71* group;
};

UnkTunableGroupRef_A1b71 fn_710013c218(UnkTunableRegistry_A1b71* self, const CStrId* name) {
    UnkTunableGroupArrays_A1b71* groups = &self->groups;
    UnkTunableGroup_A1b71* group = groups->Find(*name);
    if (group == groups->End())
        group = fn_710013c2ac(groups, name);
    return UnkTunableGroupRef_A1b71{group};
}
