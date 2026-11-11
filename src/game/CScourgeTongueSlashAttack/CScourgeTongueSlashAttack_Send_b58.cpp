// CScourgeTongueSlashAttack unit (tools/tu_map.py unit 1031). fn_710069ba34: when the owner's (+0x8) AI
// component (+0x240) is a CScourgeAIComponent, it gets its value at +0x14740 and the distance from the
// owner's position to the given point (fn_710069f604); then, unless the owner's character class (+0x220)
// already holds one of three ids under the usual track kind (fn_7101177ae8), it is sent the id of the
// attack's side (+0x38, from a three-entry table; -1 beyond it) with mode 0 (as src/game_s2/UnkSpitSend.h).
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CScourgeAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

base::global::UnkStringData** fn_7101177ae8(UnkAnimationComponent* self, u64 kind);
extern const u64 g_unk_710164a618[] __attribute__((visibility("hidden")));

struct UnkScourgeAI_b58a4 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14740 - 0x8];
    f32 field_0x14740;
};

struct UnkScourgeTongueSlashAttack_b58a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x28];
    s32 side;  // 0x38
};

struct UnkPoint_b58a4 {
    f32 x;
    f32 y;
    f32 z;
};

void fn_710069ba34(UnkScourgeTongueSlashAttack_b58a4* self, const UnkPoint_b58a4* point) {
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    UnkScourgeAI_b58a4* ai = static_cast<UnkScourgeAI_b58a4*>(owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710069ef44())) {
        UnkActor* actor = self->owner;
        UnkPoint_b58a4 pos = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
        f32 dx = point->x - pos.x;
        f32 dy = point->y - pos.y;
        f32 dz = point->z - pos.z;
        f32 distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        fn_710069f604(ai, ai->field_0x14740, distance);
    }
    owner = self->owner;
    if (!owner)
        return;
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    if (!anim)
        return;
    u64 current = *reinterpret_cast<u64*>(reinterpret_cast<char*>(*fn_7101177ae8(anim, 0xf4061ff25a2163d6)) + 0x30);
    if (current == 0xe89c493d56876bf8 || current == 0xeae407a9663038b1 || current == 0x5be4e30ac47ed5c2)
        return;
    u32 side = self->side;
    u64 hash = side <= 2 ? g_unk_710164a618[(s32)side] : (u64)-1;
    UnkSpitSend(anim, hash, 0xf4061ff25a2163d6);
}
