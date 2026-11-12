// behaviortree::CAttackAction's unit (tools/tu_map.py, unit 456): the node's start. After the base's
// fn_7100394bd0 the started flag (+0x69) is cleared; the attack is the owner's AI component's attack named by
// the string at +0x30 (overridden by the blackboard value the id at +0x60 names, fn_7100395044), or the
// component's current one (+0x1148) without a name. The flag is set when the owner's attack player
// (fn_71003729a8) is busy (fn_7101136308) or starts that attack (fn_7101134420). Names and layouts are
// placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAIAttackComponent.h"
#include "game/CAttackComponent.h"
#include "game/UnkTU2702.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

using UnkAttackName_ovb0128 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkAttackAI_ovb0128 {
    u8 pad_0x0[0x1148];
    void* attack;  // 0x1148
};

struct UnkAttackActor_ovb0128 {
    u8 pad_0x0[0x240];
    UnkAttackAI_ovb0128* ai;  // 0x240
};

struct UnkAttackAction_ovb0128 {
    u8 pad_0x0[0x18];
    UnkAttackActor_ovb0128* actor;  // 0x18
    u8 pad_0x20[0x10];
    CRntString name;  // 0x30
    CStrId name_property;  // 0x60
    u8 pad_0x68;
    bool started;  // 0x69
};

// Signatures as other files declare them.
struct UnkStruct_0x68;
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;
struct UnkScorpiusAI_b69a4;
struct UnkCommanderPlayer_b67a2;

// The owner's attack player.
void* fn_71003729a8(UnkAttackActor_ovb0128* actor);

void fn_7100372864(UnkAttackAction_ovb0128* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->started = false;
    // A copy of the string at +0x30 (only its string part is read).
    UnkAttackName_ovb0128 name(*reinterpret_cast<const UnkAttackName_ovb0128*>(&self->name));
    if (self->name_property.data != &g_unk_7101d47108->empty)
        fn_7100395044(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->name_property,
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name));
    UnkAttackAI_ovb0128* ai = self->actor ? self->actor->ai : nullptr;
    void* attack;
    const char* text = name.Str()->data;
    if (text && text[0])
        attack = fn_7101109df4(reinterpret_cast<UnkScorpiusAI_b69a4*>(ai), &name);
    else
        attack = ai ? ai->attack : nullptr;
    if (!self->actor)
        return;
    void* player = fn_71003729a8(self->actor);
    if (!player)
        return;
    if (fn_7101136308(player) ||
        (attack && reinterpret_cast<bool (*)(UnkCommanderPlayer_b67a2*, void*, u32, u32, void*, f32)>(
                       fn_7101134420)(reinterpret_cast<UnkCommanderPlayer_b67a2*>(player), attack, 0, 0, nullptr,
                                      -1.0f)))
        self->started = true;
}
