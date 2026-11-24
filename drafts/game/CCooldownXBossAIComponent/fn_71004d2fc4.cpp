// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): what the component does when the boss is
// defeated: the "#MLOG_DEFEAT_BOSS_COOLDOWN_X" message goes to the game manager's callbacks, the base handler
// runs and the damage at +0x15838 is updated; then the death track's id is sent to the character class, and
// when that fails (or there is none) fn_71004d3318 runs. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

// The message's arguments, freed with it.
struct UnkMlogArgs_l4d2fc4 : base::global::CRntVector<base::UnkStrIdMember> {
    UnkMlogArgs_l4d2fc4() : base::global::CRntVector<base::UnkStrIdMember>(0xc) {}
    ~UnkMlogArgs_l4d2fc4() { Free(); }
};

struct UnkMlogMessage_l4d2fc4 {
    u32 kind;
    base::global::CStrId text;
    UnkMlogArgs_l4d2fc4 args;

    UnkMlogMessage_l4d2fc4(const CStrId& t) : kind(0), text(t) {}
};

struct UnkCooldownXDefeatOwner_l4d2fc4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCooldownXDefeatAI_l4d2fc4 {
    u8 pad_0x0[0x30];
    UnkCooldownXDefeatOwner_l4d2fc4* owner;  // 0x30
    u8 pad_0x38[0x157ff];
    bool skip_death_track;  // 0x15837
    u8 damage[0x10];        // 0x15838
};
static_assert(__builtin_offsetof(UnkCooldownXDefeatAI_l4d2fc4, skip_death_track) == 0x15837, "layout");

void fn_71004d3318(UnkCooldownXDefeatAI_l4d2fc4* self);

void fn_71004d2fc4(UnkCooldownXDefeatAI_l4d2fc4* self, void* arg) {
    UnkMlogMessage_l4d2fc4 message(CStrId("#MLOG_DEFEAT_BOSS_COOLDOWN_X", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(self->damage),
                  static_cast<const UnkWeaponDamage_ovb0054*>(arg));
    if (self->owner->animation && !self->skip_death_track) {
        u64 hash = 0xd99849d4ae9273c3;
        UnkTrackQuery query;
        query.cls = fn_7100bf92d8();
        query.result = nullptr;
        query.value = -1.0f;
        fn_710117b08c(self->owner->animation, &query);
        if (query.result && query.result->sources.data != &g_unk_7101d47108->empty)
            hash = query.result->sources.data->Key();
        // Sends the hash with mode 0 to the character class; done when it was taken.
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        if (CAnimationComponent_SendActionToTrack(self->owner->animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask))
            return;
    }
    fn_71004d3318(self);
}
