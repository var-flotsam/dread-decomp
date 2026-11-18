// CFlockingSwarmControllerComponent's unit (tools/tu_map.py 1158): adds a member to the swarm through the
// base, then records the member's position and the swarm's current value in two growable arrays.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSwarmMember_l738edc {
    u8 pad_0x0[0x6c];
    UnkVec3f position;  // 0x6c
};

struct UnkFlockingSwarm_l738edc {
    u8 pad_0x0[0x90];
    base::global::CRntVector<UnkVec3f> positions;  // 0x90
    base::global::CRntVector<f32> values;          // 0xb8
    f32 value;                                     // 0xe0
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l738edc, value) == 0xe0, "layout");

s32 fn_710074c7dc(UnkFlockingSwarm_l738edc* self, void* arg, UnkSwarmMember_l738edc* member);

s32 fn_7100738edc(UnkFlockingSwarm_l738edc* self, void* arg, UnkSwarmMember_l738edc* member) {
    s32 result = fn_710074c7dc(self, arg, member);
    self->positions.PushBackCopy(member->position);
    self->values.PushBack(self->value);
    return result;
}
