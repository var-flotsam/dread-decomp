// CCooldownXBossWindTunnelAttack's unit (tools/tu_map.py unit 751): ending the attack. After the base steps the game
// manager's value (+0x38) is cleared, the AI component (+0x240) is told off (fn_71004d432c) and the collider
// g_unk_71726d8108 of the owner's container is switched off; the timeline id is reset (fn_71004cbc28) and the
// component is told to finish (fn_71004d1fb0); finally the entity named by g_unk_71726d8110 is sent a request. Names
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/CCooldownXBossAttack.h"
#include "game/CCooldownXBossWindTunnelAttack.h"
#include "game/UnkActor.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkWindTunnelEnd_ovb0265;
struct UnkCooldownXBossAttack_b62a2;
void fn_71004cbaa8(UnkWindTunnelEnd_ovb0265* self);
void fn_71004cbc28(UnkWindTunnelEnd_ovb0265* self);
void fn_7100f0e238(void* target, u32 value);
struct UnkCooldownAI_ovb0265;
void fn_71004d432c(UnkCooldownAI_ovb0265* self, u32 on);
void fn_71004d1fb0(UnkCooldownAI_ovb0265* self);

struct UnkWindTunnelFlag {
    u8 pad_0x0[0x40];
    f32 field_0x40;
};
struct UnkGlobal2;
extern const base::global::CStrId g_unk_71726d8108 __attribute__((visibility("hidden")));

struct UnkWeakTarget;

struct UnkWindTunnelEnd_ovb0265 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

void fn_71004cc42c(UnkWindTunnelEnd_ovb0265* self) {
    fn_71004c196c(reinterpret_cast<UnkCooldownXBossAttack_b62a2*>(self));
    fn_7100f0e238(reinterpret_cast<char*>(g_unk_7101d47310) + 0x38, 0);
    fn_71004cbaa8(self);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318()))
        fn_71004d432c(reinterpret_cast<UnkCooldownAI_ovb0265*>(ai), 0);
    if (auto* container = *reinterpret_cast<UnkCollisionContainer**>(reinterpret_cast<char*>(self->owner) + 0x2a0)) {
        if (UnkCollisionObject* object = fn_71008640a4(container, &g_unk_71726d8108))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
    }
    fn_71004cbc28(self);
    ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318())) {
        fn_71004d1fb0(reinterpret_cast<UnkCooldownAI_ovb0265*>(ai));
        reinterpret_cast<UnkWindTunnelFlag*>(&g_unk_7172790a08)->field_0x40 = 1.0f;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(g_unk_71726d8110.data) + 0x30);
    kind = kind_value;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &key, &kind);
    if (!entity)
        return;
    void* anim = *reinterpret_cast<void**>(reinterpret_cast<char*>(entity) + 0x220);
    if (!anim)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7101176690(anim, 0x3d73722bcf353a54, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
