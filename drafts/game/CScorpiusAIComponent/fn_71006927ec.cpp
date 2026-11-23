// CScorpiusAIComponent unit: the start step. Resets the timer, tints the owner's glow part, makes two named
// husks unbreakable, enters the tuned mode, clears the attack state and tells the owner's animation about
// the start. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkColor_l6927ec {
    f32 r, g, b, a;
    UnkColor_l6927ec(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkGlowPart_l6927ec {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkScorpiusOwner_l6927ec {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* world;  // 0x280
};

// The tunables blocks as this function reads them.
struct UnkScorpiusTimes_l6927ec {
    u8 pad_0x0[0x1d4];
    f32 start_time;  // 0x1d4
};

struct UnkScorpiusTunables_l6927ec {
    u8 pad_0x0[0x24];
    s32 mode;  // 0x24
};

struct UnkScorpiusAI_l6927ec;
struct UnkScorpiusAIVtable_l6927ec {
    u8 pad_0x0[0x208];
    void (*vf_0x208)(UnkScorpiusAI_l6927ec* self, s32 mode, bool flag);
};

struct UnkScorpiusAI_l6927ec {
    UnkScorpiusAIVtable_l6927ec* vtable;
    u8 pad_0x8[0x28];
    UnkScorpiusOwner_l6927ec* owner;  // 0x30
    u8 pad_0x38[0x14c84];
    f32 timer;  // 0x14cbc
    u8 pad_0x14cc0[8];
    f32 field_0x14cc8;
    f32 field_0x14ccc;
    u8 pad_0x14cd0[0x34];
    f32 field_0x14d04;
    u8 pad_0x14d08[0x78];
    bool field_0x14d80;
    u8 pad_0x14d81[7];
    s32 attack_count;  // 0x14d88
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l6927ec, timer) == 0x14cbc, "timer");
static_assert(__builtin_offsetof(UnkScorpiusAI_l6927ec, field_0x14d04) == 0x14d04, "field_0x14d04");
static_assert(__builtin_offsetof(UnkScorpiusAI_l6927ec, attack_count) == 0x14d88, "attack_count");

// The part of the string data read here: a 64-bit key at 0x30.
struct UnkStringKey_l6927ec {
    u8 pad_0x0[0x30];
    u64 key;
};

inline u64 KeyOf(const CStrId& id) { return reinterpret_cast<const UnkStringKey_l6927ec*>(id.data)->key; }

struct UnkMissilePart;
struct UnkMagnetSurfaceHusk_b48;
void fn_71003aff64(UnkCoreXAI_A2_b73* self, u32 a, u32 b);
void* fn_7100692bb0(void* owner);
void fn_7100925348(UnkMagnetSurfaceHusk_b48* self, bool breakable);
void fn_7100692c8c(UnkScorpiusAI_l6927ec* self, s32 a, bool b);

__attribute__((visibility("hidden"))) extern CStrId g_unk_7172704208[2];  // the two husks' names
__attribute__((visibility("hidden"))) extern CStrId g_unk_7172704220;

// Tints the owner's glow part, when its world accepts the part.
inline void TintGlow(UnkScorpiusAI_l6927ec* self) {
    UnkWorld* world = self->owner->world;
    UnkGlowPart_l6927ec* part = reinterpret_cast<UnkGlowPart_l6927ec*>(fn_7101152b90(world, 0x7eced1a36c46400full));
    if (!part)
        return;
    if (!fn_710114e668(reinterpret_cast<UnkObject*>(world), part->field_0xb))
        return;
    UnkColor_l6927ec color(1.0f, 0.66f, 0.12f, 1.0f);
    fn_7100146c50(reinterpret_cast<UnkMissilePart*>(part), 0, &color, 1);
}

// The husk the game manager knows by `name` stops being breakable.
inline void MakeUnbreakable(const CStrId& name) {
    u64 key = KeyOf(name);
    UnkGameManager* manager = g_unk_7101d47310;
    u32 kind = g_unk_71727bebb8;
    UnkWeakTarget* target = fn_7100debbc8(manager, &key, &kind);
    fn_7100925348(reinterpret_cast<UnkMagnetSurfaceHusk_b48*>(fn_7100692bb0(target)), false);
}

void fn_71006927ec(UnkScorpiusAI_l6927ec* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    self->timer = reinterpret_cast<const UnkScorpiusTimes_l6927ec&>(g_unk_717272fd38).start_time;
    TintGlow(self);
    fn_71003aff64(reinterpret_cast<UnkCoreXAI_A2_b73*>(self), 0, 1);
    MakeUnbreakable(g_unk_7172704208[0]);
    MakeUnbreakable(g_unk_7172704208[1]);
    fn_7100692c8c(self, 1, true);
    switch (reinterpret_cast<const UnkScorpiusTunables_l6927ec&>(g_unk_7172703e88).mode) {
    case 1:
        self->vtable->vf_0x208(self, 1, true);
        break;
    case 2:
        self->vtable->vf_0x208(self, 2, true);
        break;
    }
    self->attack_count = 0;
    self->field_0x14cc8 = 0.0f;
    self->field_0x14ccc = 0.0f;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0x82;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, 0xea4b14555468f96aull, 0x722b01c64fe0ffe0ull, &mode, &g_unk_710164b968,
                      &g_unk_7172769740, &mask);
        unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(self->owner->animation->field_0x80),
                                    g_unk_7172704220, self->field_0x14d04);
    }
    self->field_0x14d80 = false;
}
