// Unit 2702 (hint CAIComponent): an AI component's area box: the player's height as both corners'
// depth and, around the owner's position, the owner's half extents. Zero when there is no player or owner.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkVec3_l1063a4 {
    f32 x, y, z;
};

struct UnkBox_l1063a4 {
    UnkVec3f min;
    UnkVec3f max;
};

struct UnkBoundsOwner_l1063a4 {
    u8 pad_0x0[0x388];
    f32 field_0x388;
    f32 field_0x38c;
    f32 field_0x390_unused_pad;
    f32 field_0x394;
};

struct UnkBoundsSelf_l1063a4 {
    u8 pad_0x0[0x30];
    UnkBoundsOwner_l1063a4* owner;  // 0x30
};

void fn_71011063a4(UnkBoundsSelf_l1063a4* self, UnkBox_l1063a4* out) {
    out->min = UnkVec3f(0.0f, 0.0f, 0.0f);
    out->max = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if ((player == nullptr) | (self->owner == nullptr))
        return;
    UnkVec3f playerPos = *reinterpret_cast<const UnkVec3f*>(fn_7100d7c9f4(player));
    f32 z = playerPos.z;
    const UnkVec3_l1063a4* pos = reinterpret_cast<const UnkVec3_l1063a4*>(
        fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(self->owner)));
    f32 px = pos->x;
    f32 py = pos->y;
    UnkBoundsOwner_l1063a4* owner = self->owner;
    f32 halfX = owner->field_0x388 * 0.5f;
    f32 halfY = owner->field_0x38c * 0.5f;
    f32 y = py + owner->field_0x394;
    out->min = UnkVec3f(px - halfX, y - halfY, z);
    out->max = UnkVec3f(px + halfX, y + halfY, z + 0.0f);
}
