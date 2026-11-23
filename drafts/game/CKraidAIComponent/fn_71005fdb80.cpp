// The CKraidAIComponent unit. fn_71005fdb80, Kraid's per-frame belly-bomb and belly-damage step: counts down
// the bomb timers (dropping each one that runs out, outside a CCharClassIgnoreKraidBellyBombAnimationTrack),
// reacts to the bombs now in the world, and while in a CCharClassKraidBellyDamageTrack sends the linked
// actor a damage request each time the damage timer runs out. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBombMovement.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkFlagListS7.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/animtree/CLayerNode.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::UnkReflObject;
using base::reflection::UnkTunable;
using unk_flag_list::UnkFlagListKey;

struct UnkPlayer;
struct UnkInputComponent;
struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

extern __attribute__((visibility("hidden"))) u64 g_unk_71726f3798;  // the layer node's key
extern UnkFlagListKey g_unk_71726f37a0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f3790 __attribute__((visibility("hidden")));
extern u64 g_unk_710164a220;  // the timeline id set when a bomb timer runs out
extern u64 g_unk_710164a228;  // the timeline id set when bombs appear
extern UnkTunable g_unk_7172753ad0;

void fn_710116a168(void* target, void* key);

struct UnkFanBombEntry_ovb0122 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
};

struct UnkFanBombList_ovb0122 {
    UnkFanBombEntry_ovb0122** data;
    u32 count;
};

// What a bomb's movement points at (+0x18); its vf_0xe0 is called for each bomb.
struct UnkKraidBombOwner_l5fdb80 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
};

struct UnkKraidBombMovement_l5fdb80 {
    u8 pad_0x0[0x18];
    UnkKraidBombOwner_l5fdb80* owner;  // 0x18
};

struct UnkKraidNodeMap_l5fdb80 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88

    UnkReflObject** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkKraidTree_l5fdb80 {
    u8 pad_0x0[0x40];
    UnkKraidNodeMap_l5fdb80* map;  // 0x40
};

struct UnkKraidDamageRequest_l5fdb80 {
    u8 pad_0x0[0x21];
    bool field_0x21;
    u8 pad_0x22[0x7];
    bool field_0x29;
    u8 pad_0x2a[0x2];
    f32 amount;  // 0x2c
    u8 pad_0x30[0x84];
    u32 flags;       // 0xb4
    u8 id_raw[0x8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x40];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkKraidDamageRequest_l5fdb80() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkKraidDamageRequest_l5fdb80() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkKraidDamageRequest_l5fdb80) == 0x100, "request layout");

struct UnkKraidDamageTarget_l5fdb80 {
    void** vtable;
    void vf_0x188(UnkKraidDamageRequest_l5fdb80* request) {
        reinterpret_cast<void (*)(UnkKraidDamageTarget_l5fdb80*, UnkKraidDamageRequest_l5fdb80*)>(
            vtable[0x188 / 8])(this, request);
    }
};

// The parts of an actor this function reads that UnkActor leaves as padding.
struct UnkKraidActor_l5fdb80 {
    u8 pad_0x0[0x30];
    u8 key[0x8];  // 0x30
    u8 pad_0x38[0x1c0 - 0x38];
    UnkKraidDamageTarget_l5fdb80* damage;  // 0x1c0
};

struct UnkKraidPlayer_l5fdb80 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

struct UnkKraidTuning_l5fdb80 {
    u8 pad_0x0[0x54c];
    f32 damageInterval;  // 0x54c
};

struct UnkKraidBombTunable_l5fdb80 {
    u8 pad_0x0[0x24];
    f32 bombTime;  // 0x24
};

struct UnkKraidDamageTunable_l5fdb80 {
    u8 pad_0x0[0x294];
    f32 damage;  // 0x294
};

// What a timer derives from (nothing of it is read here).
struct UnkKraidTimerBase_l5fdb80 {};

// The time left on one bomb in Kraid's belly.
struct UnkKraidBombTimer_l5fdb80 : UnkKraidTimerBase_l5fdb80 {
    f32 time;
    explicit UnkKraidBombTimer_l5fdb80(f32 t) : time(t) {}
    ~UnkKraidBombTimer_l5fdb80() {}
};

struct UnkKraidAI_l5fdb80 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x149e4 - 0x38];
    u32 bombsLeft;                                // 0x149e4
    CRntVector<UnkKraidBombTimer_l5fdb80> bombs;  // 0x149e8
    f32 damageTimer;                              // 0x14a10
    u8 pad_0x14a14[0x14a30 - 0x14a14];
    UnkKraidTuning_l5fdb80* tuning;  // 0x14a30
};
static_assert(__builtin_offsetof(UnkKraidAI_l5fdb80, damageTimer) == 0x14a10, "layout");

// The layer node under g_unk_71726f3798 in the tree, when it is a CLayerNode.
static inline UnkLayerNode* FindLayer_l5fdb80(UnkKraidTree_l5fdb80* tree) {
    UnkKraidNodeMap_l5fdb80* map = tree->map;
    UnkReflObject** it = map->Find(g_unk_71726f3798);
    UnkReflObject* layer = nullptr;
    if (it != map->values + map->count) {
        if (UnkReflObject* node = *it)
            layer = fn_710009d50c(node->vf_0x0(), fn_71002e43a4()) ? node : nullptr;
    }
    return reinterpret_cast<UnkLayerNode*>(layer);
}

// Sends a request under the two ids with the given mode (as UnkSpitSend does with mode 0).
inline void SendWithMode_l5fdb80(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

// Sets `id` on a timeline to the all-flags mask.
inline void SetTimeline_l5fdb80(void* timeline, const u64* id) {
    u32 mask = UnkAll_SpitSend().value;
    fn_7100a2cb7c(static_cast<UnkTimelineComponent*>(timeline), id, &mask, false, -1.0f);
}

// Sets `id` on the actor's timeline and on the linked actor's, when it has one.
static inline void SetTimelines_l5fdb80(UnkKraidAI_l5fdb80* self, const u64* id) {
    SetTimeline_l5fdb80(self->actor->field_0x2e8, id);
    if (fn_7100d7c990(self->actor)->field_0x2e8)
        SetTimeline_l5fdb80(fn_7100d7c990(self->actor)->field_0x2e8, id);
}

void fn_71005fdb80(UnkKraidAI_l5fdb80* self, f32 dt) {
    void* linked = fn_7100d7c990(self->actor)->field_0x220;
    UnkKraidTree_l5fdb80* tree = static_cast<UnkKraidTree_l5fdb80*>(self->actor->field_0x220);
    UnkFanBombList_ovb0122* list = fn_7100aa9ecc();
    if (!tree)
        return;
    UnkAnimationComponent* anim = reinterpret_cast<UnkAnimationComponent*>(tree);
    u32 newBombs = list->count;

    if (self->bombs.count != 0) {
        UnkKraidBombTimer_l5fdb80* it = self->bombs.data;
        while (it != self->bombs.data + self->bombs.count) {
            it->time -= dt;
            if (!(it->time < 0.0f) || fn_710117b348(anim, fn_7100c30db8())) {
                ++it;
                continue;
            }
            u32 index = it - self->bombs.data;
            self->bombs.EraseAt(index);
            it = self->bombs.data + index;
            SetTimelines_l5fdb80(self, &g_unk_710164a220);
            fn_71002e465c(FindLayer_l5fdb80(tree), 1.0f);
            SendWithMode_l5fdb80(tree, 0xf34062645faece9d, 0xb2a7539239c3aad5, 2);
        }
        if (self->bombsLeft == 0 && self->bombs.count == 0) {
            SendWithMode_l5fdb80(linked, 0xec9227e3237518ff, 0xf4061ff25a2163d6, 2);
            SendWithMode_l5fdb80(tree, 0xec9227e3237518ff, 0xf4061ff25a2163d6, 2);
        }
    }

    if (newBombs != 0) {
        for (UnkFanBombEntry_ovb0122 **p = list->data, **e = p + list->count; p != e; ++p)
            reinterpret_cast<UnkKraidBombMovement_l5fdb80*>((*p)->movement)->owner->vf_0xe0();
        u32 left = self->bombsLeft;
        self->bombsLeft = left < newBombs ? 0 : left - newBombs;
        self->bombs.EmplaceBack(
            UnkKraidBombTimer_l5fdb80(reinterpret_cast<UnkKraidBombTunable_l5fdb80*>(&g_unk_7172753ad0)->bombTime));
        SetTimelines_l5fdb80(self, &g_unk_710164a228);
        if (self->bombsLeft == 0) {
            UnkKraidPlayer_l5fdb80* player =
                reinterpret_cast<UnkKraidPlayer_l5fdb80*>(fn_7100dec788(g_unk_7101d47310));
            if (player && player->input)
                fn_7100872460(player->input, true, false, &g_unk_71726f37a0);
        }
    }

    if (!fn_710117b348(anim, fn_7100c38738()))
        return;
    if (self->damageTimer > 0.0f) {
        self->damageTimer -= dt;
        if (self->damageTimer < 0.0f)
            self->damageTimer = 0.0f;
    }
    if (self->damageTimer != 0.0f)
        return;
    self->damageTimer = self->tuning->damageInterval;
    UnkKraidDamageRequest_l5fdb80 request;
    request.Id() = g_unk_71726f3790;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                  reinterpret_cast<UnkKraidDamageTunable_l5fdb80*>(&g_unk_7172716160)->damage);
    request.flags = 0x10000000;
    request.field_0x21 = true;
    request.field_0x29 = true;
    request.amount = self->damageTimer;
    reinterpret_cast<UnkKraidActor_l5fdb80*>(fn_7100d7c990(self->actor))->damage->vf_0x188(&request);
    fn_710116a168(reinterpret_cast<UnkKraidActor_l5fdb80*>(fn_7100d7c990(self->actor))->damage,
                  reinterpret_cast<UnkKraidActor_l5fdb80*>(self->actor)->key);
}
