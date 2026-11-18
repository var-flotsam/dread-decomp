// CDizzeanSwarmControllerComponent's unit (tools/tu_map.py unit 770): the update. After the base
// step, every active group sums its live members' offsets from the group's centre; when the sum is
// not near zero and points against the group's heading, the group is flagged to turn. Names and
// layouts are placeholders.
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkSwarmHold_b66a4;
// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Hold_b66a4.cpp declares it.
void fn_7100741aa8(UnkSwarmHold_b66a4* self, f32 dt);

struct UnkDizzeanMember_l4e3884 {
    u8 pad_0x0[0x25];
    bool field_0x25;
};

// A group of members (0x148 bytes).
struct UnkDizzeanGroup_l4e3884 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x6b - 0x25];
    bool field_0x6b;
    bool turn;  // 0x6c
    u8 pad_0x6d[0x70 - 0x6d];
    f32 heading[3];  // 0x70
    u8 pad_0x7c[0xb8 - 0x7c];
    UnkDizzeanMember_l4e3884** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];

    UnkDizzeanMember_l4e3884** begin() { return members; }
    UnkDizzeanMember_l4e3884** end() { return members + memberCount; }
};

struct UnkDizzeanGroupList_l4e3884 {
    u8 pad_0x0[0x50];
    UnkDizzeanGroup_l4e3884* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkDizzeanGroup_l4e3884* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkDizzeanData_l4e3884 {
    u8 pad_0x0[0x10];
    UnkDizzeanGroupList_l4e3884* groupList;  // 0x10
};

struct UnkDizzeanSwarm_l4e3884 {
    u8 pad_0x0[0xe8];
    UnkDizzeanData_l4e3884* data;  // 0xe8
};

// Within a small tolerance of zero.
static inline bool NearZero(f32 v) { return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f; }

static inline UnkVec3Result operator-(const UnkVec3Result& a, const UnkVec3Result& b) {
    return UnkVec3Result(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct UnkDizzeanSum_l4e3884 {
    f32 x;
    f32 y;
    f32 z;
    UnkDizzeanSum_l4e3884() : x(0.0f), y(0.0f), z(0.0f) {}
    void operator+=(const UnkVec3Result& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
};

void fn_71004e3884(UnkDizzeanSwarm_l4e3884* self, f32 dt) {
    fn_7100741aa8(reinterpret_cast<UnkSwarmHold_b66a4*>(self), dt);
    s32 count = self->data->groupList->count;
    for (s32 i = 0; i < count; i++) {
        UnkDizzeanGroup_l4e3884* group = self->data->groupList->At(i);
        if (!group->active)
            continue;
        UnkVec3Result centre = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
        UnkDizzeanSum_l4e3884 sum;
        for (UnkDizzeanMember_l4e3884* member : *group) {
            if (!fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)) || !member->field_0x25)
                continue;
            sum += fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member)) - centre;
        }
        bool moved = !(NearZero(sum.x) && NearZero(sum.y) && NearZero(sum.z));
        if (moved && sum.x * group->heading[0] + sum.y * group->heading[1] + sum.z * group->heading[2] < 0.0f) {
            group->field_0x6b = false;
            group->turn = true;
        }
    }
}
