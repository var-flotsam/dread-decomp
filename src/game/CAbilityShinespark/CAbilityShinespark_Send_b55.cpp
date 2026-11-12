// CAbilityShinespark's unit (tools/tu_map.py): when the actor (+0x30 of the object at +0xc0) has
// its +0x210 set, sends the sub-object at its +0x220 the id at +0x30 of the id table's entry at
// +0x1d78 (fn_7100031894) with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h); unless `kind` is
// 4, then removes one timeline id from the actor (fn_7100d7c94c). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkIdHolder_b55sh {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b55sh {
    UnkIdTable_b55sh();  // fn_7100031894
    ~UnkIdTable_b55sh();
    u8 pad_0x0[0x1d78];
    UnkIdHolder_b55sh* field_0x1d78;
};

inline UnkIdTable_b55sh& IdTable_b55sh() {
    static UnkIdTable_b55sh s_table;
    return s_table;
}

extern const u64 g_unk_7101648e88;

struct UnkShinesparkActor_b55 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[0x8];
    void* field_0x220;
};

struct UnkShinesparkLink_b55 {
    u8 pad_0x0[0x30];
    UnkShinesparkActor_b55* actor;
};

struct UnkShinespark_b55 {
    u8 pad_0x0[0xc0];
    UnkShinesparkLink_b55* link;
};

static inline __attribute__((always_inline)) void Send_b55sh(void* target, u64 hash) {
    UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
}

void fn_710032f9b0(UnkShinespark_b55* self, s32 kind) {
    UnkShinesparkActor_b55* actor = self->link->actor;
    if (actor->field_0x210)
        Send_b55sh(actor->field_0x220, IdTable_b55sh().field_0x1d78->id);
    if (kind != 4)
        fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648e88);
}

struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
struct UnkOwner;
extern const u64 g_unk_7101648ed8 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101648ef8 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101648ec0;

struct UnkShinesparkTunable_b55 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
};
struct UnkTunable;

struct UnkShinesparkFxHolder_b55 {
    u8 pad_0x0[0x228];
    base::reflection::UnkReflObject* comp;
};

struct UnkShinesparkFxLink_b55 {
    u8 pad_0x0[0x10];
    UnkShinesparkFxHolder_b55* holder;
};

struct UnkShinesparkActor2_b55 {
    u8 pad_0x0[0x1f0];
    UnkGroundPlayer_b52* field_0x1f0;
    u8 pad_0x1f8[0x28];
    void* field_0x220;
};

struct UnkShinesparkLink2_b55 {
    u8 pad_0x0[0x30];
    UnkShinesparkActor2_b55* actor;
};

struct UnkShinespark2_b55 {
    u8 pad_0x0[0xc0];
    UnkShinesparkLink2_b55* link;
    u8 pad_0xc8[0x390 - 0xc8];
    f32 field_0x390;
    u8 pad_0x394[0x750 - 0x394];
    u32 field_0x750;
};

static inline void Fade_b55sh(base::reflection::UnkReflObject* comp) {
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        fn_710093adc0(comp, &g_unk_7101648ed8, &g_unk_7101648ef8, 0.2f);
}

void fn_710032dfdc(UnkShinespark2_b55* self) {
    UnkShinesparkActor2_b55* actor = self->link->actor;
    if (actor && actor->field_0x220) {
        u64 hash = 0xc906c3bd30e0562f;
        if (actor->field_0x1f0 && fn_71010d1598(actor->field_0x1f0))
            hash = 0xccbcbb5050c1a714;
        Send_b55sh(actor->field_0x220, hash);
    }
    self->field_0x390 = reinterpret_cast<UnkShinesparkTunable_b55*>(&g_unk_71726b0378)->field_0x24;
    self->field_0x750 = 0;
    Fade_b55sh(reinterpret_cast<UnkShinesparkFxHolder_b55*>(fn_7100defcd4(g_unk_7101d47310))->comp);
    Fade_b55sh((*reinterpret_cast<UnkShinesparkFxLink_b55**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0))
                   ->holder->comp);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648ec0, false);
}
