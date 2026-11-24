// CScorpiusAIComponent unit: the damage handler. A hit on the mask (the collider named by a fixed hash) is
// counted and may start the mask attack before the base handler answers; any other hit costs health, then
// the owner's timeline plays the ids for the part hit. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/CGrabComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1370.h"
#include "game/UnkTimelineCalls.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;

struct UnkScorpiusAI_l692a98;
void CScorpiusAIComponent_SetPartColorFromCubedAmount(UnkScorpiusAI_l692a98* self, f32 amount);
struct UnkScorpiusAI_b59a4;
void fn_7100696b48(UnkScorpiusAI_b59a4* self);

extern CStrId g_unk_71727041f0 __attribute__((visibility("hidden")));  // the body's collider name
extern const u64 g_unk_710164a570;  // reached through the GOT
extern const u64 g_unk_710164a578;  // reached through the GOT
extern const u64 g_unk_710164a580;  // reached through the GOT
extern const u64 g_unk_710164a588;  // reached through the GOT
extern const u64 g_unk_710164a590;  // reached through the GOT
extern const u64 g_unk_710164a598;  // reached through the GOT
extern const u64 g_unk_710164a5a0;  // reached through the GOT
extern const u64 g_unk_710164a5a8;  // reached through the GOT
extern const u64 g_unk_710164a5b0;  // reached through the GOT

// The mask collider's name hash.
static const u64 kMaskHash = 0xf12fca483f92ea27;

struct UnkScorpiusTunables_l696f04 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkScorpiusHealthTunables_l696f04 {
    u8 pad_0x0[0x1d4];
    f32 max_health_a;  // 0x1d4: mode 1
    f32 max_health_b;  // 0x1d8
};

struct UnkScorpiusEvent_l696f04 {
    u8 pad_0x0[0x28];
    bool field_0x28;
    u8 pad_0x29[0x60 - 0x29];
    void* collider;  // 0x60
    u8 pad_0x68[0x4];
    f32 x, y, z;  // 0x6c
    u8 pad_0x78[0xb4 - 0x78];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0xf8 - 0xb8];
    f32 damage;  // 0xf8
};

struct UnkScorpiusAttack_l696f04;
struct UnkScorpiusAttackVtbl_l696f04 {
    const base::reflection::CType* (*vf_0x0)(UnkScorpiusAttack_l696f04* self);
    void* slots[0xe8 / 8 - 1];
    bool (*vf_0xe8)(UnkScorpiusAttack_l696f04* self);
};

struct UnkScorpiusAttack_l696f04 {
    UnkScorpiusAttackVtbl_l696f04* vtable;
    u8 pad_0x8[0x90];
};

struct UnkScorpiusAttacks_l696f04 {
    u8 pad_0x0[0x1a0];
    UnkScorpiusAttack_l696f04* current;  // 0x1a0
};

struct UnkScorpiusTarget_l696f04 {
    u8 pad_0x0[0x54];
    f32 distance;  // 0x54
};

struct UnkScorpiusOwner_l696f04 {
    u8 pad_0x0[0x1b8];
    UnkScorpiusTarget_l696f04* target;  // 0x1b8
    u8 pad_0x1c0[0x220 - 0x1c0];
    void* animation;  // 0x220
    u8 pad_0x228[0x258 - 0x228];
    void* attack;  // 0x258
    void* grab;    // 0x260
    u8 pad_0x268[0x278 - 0x268];
    UnkScorpiusAttacks_l696f04* attacks;  // 0x278
    u8 pad_0x280[0x2a0 - 0x280];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkScorpiusData_l696f04 {
    u8 pad_0x0[0x5b8];
    UnkRobotSoldierTable_A2_b69* damage_factors;  // 0x5b8: an object, used by address
};

struct UnkScorpiusAI_l696f04;
struct UnkScorpiusAIVtbl_l696f04 {
    void* slots[0x208 / 8];
    void (*vf_0x208)(UnkScorpiusAI_l696f04* self, s32 state, bool flag);
};

struct UnkScorpiusAI_l696f04 {
    UnkScorpiusAIVtbl_l696f04* vtable;
    u8 pad_0x8[0x28];
    UnkScorpiusOwner_l696f04* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    s32 mode;  // 0x1140
    u8 pad_0x1144[0x149a0 - 0x1144];
    UnkScorpiusData_l696f04* data;  // 0x149a0
    u8 pad_0x149a8[0x14b58 - 0x149a8];
    UnkScorpiusAttack_l696f04 mask_attack;  // 0x14b58
    u8 pad_0x14bf0[0x14c29 - 0x14bf0];
    bool field_0x14c29;
    u8 pad_0x14c2a[0x14cb8 - 0x14c2a];
    f32 mask_damage;    // 0x14cb8
    f32 health;         // 0x14cbc
    u8 pad_0x14cc0[0x4];
    f32 field_0x14cc4;
    f32 field_0x14cc8;
    f32 field_0x14ccc;
    u8 pad_0x14cd0[0x14cfa - 0x14cd0];
    bool field_0x14cfa;
    u8 pad_0x14cfb[0x14d88 - 0x14cfb];
    s32 phase;  // 0x14d88
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l696f04, data) == 0x149a0, "layout");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696f04, mask_attack) == 0x14b58, "layout");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696f04, health) == 0x14cbc, "layout");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696f04, phase) == 0x14d88, "layout");

void fn_71006935a4(UnkScorpiusAI_l696f04* self, bool a, bool b);

static inline bool IsMask(const CStrId* name) { return name->data->Key() == kMaskHash; }

u32 fn_7100696f04(UnkScorpiusAI_l696f04* self, UnkScorpiusEvent_l696f04* event) {
    UnkRequest* request = reinterpret_cast<UnkRequest*>(event);
    if (void* grab = self->owner->grab) {
        if (fn_710113d83c(grab)) {
            f32 factor = fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(&self->data->damage_factors),
                                       event->flags);
            fn_71008c1538(request, factor * event->damage);
        }
    }
    if (!event->collider || !self->owner->collision || !self->owner->animation)
        return 2;
    const CStrId* name = fn_7100864018(self->owner->collision, event->collider);
    u32 result;
    bool body_hit;
    if (IsMask(name)) {
        if (!(event->flags & 0x2000041e))
            return 2;
        self->mask_damage = event->damage + self->mask_damage;
        if (self->mode == 2) {
            void* grab = self->owner->grab;
            if (!grab || fn_710113d83c(grab)) {
                self->field_0x14ccc = event->damage + self->field_0x14ccc;
                self->field_0x14cc8 = event->damage + self->field_0x14cc8;
            } else {
                self->field_0x14cc4 = event->damage + self->field_0x14cc4;
                UnkScorpiusOwner_l696f04* owner = self->owner;
                if (owner && owner->attack) {
                    bool busy = false;
                    if (fn_7101136308(owner->attack)) {
                        UnkScorpiusAttack_l696f04* current = self->owner->attacks->current;
                        busy = current && fn_710009d50c(current->vtable->vf_0x0(current), fn_710068629c()) &&
                               !current->vtable->vf_0xe8(current);
                    }
                    if (!busy && !event->field_0x28) {
                        grab = self->owner->grab;
                        if (grab && !fn_710113d83c(grab)) {
                            owner = self->owner;
                            f32 distance = owner->target->distance;
                            bool start = false;
                            if (self->phase == 1) {
                                if (self->field_0x14cc8 > 250.0f)
                                    start = distance <= 750.0f;
                                else
                                    start = distance <= 1900.0f;
                            } else if (self->phase == 2) {
                                if (self->field_0x14cc4 >= 400.0f) {
                                    if (self->field_0x14cc8 > 250.0f)
                                        start = distance <= 650.0f && distance >= 300.0f;
                                    else
                                        start = distance <= 750.0f;
                                }
                            }
                            if (start && (self->field_0x14cfa || &self->mask_attack != owner->attacks->current))
                                fn_71006935a4(self, true, true);
                        }
                    }
                }
            }
        }
        result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self),
                               reinterpret_cast<UnkQuetzoaXMessage_b58*>(event));
        if (result == 2)
            return 2;
        body_hit = false;
    } else {
        s32 mode = self->mode;
        if (mode == 0 || !self->field_0x14c29)
            return 2;
        if (!(*name == g_unk_71727041f0)) {
            UnkCollisionObject* body = fn_7100864194(self->owner->collision, &g_unk_71727041f0);
            UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(body));
            f32 dx = event->x - transform.m[3];
            f32 dy = event->y - transform.m[7];
            f32 dz = event->z - transform.m[11];
            if (!(dx * dx + dy * dy + dz * dz <= 19600.0f)) {
                const auto* tunables = reinterpret_cast<const UnkScorpiusTunables_l696f04*>(&g_unk_7172703e88);
                fn_71008c1538(request, event->damage * tunables->field_0x54);
                return 2;
            }
            mode = self->mode;
        }
        self->health -= event->damage;
        const auto* health = reinterpret_cast<const UnkScorpiusHealthTunables_l696f04*>(&g_unk_717272fd38);
        const f32* max_health = mode == 1 ? &health->max_health_a : &health->max_health_b;
        CScorpiusAIComponent_SetPartColorFromCubedAmount(reinterpret_cast<UnkScorpiusAI_l692a98*>(self), self->health / *max_health);
        if (self->health <= 0.0f && !fn_710113d83c(self->owner->grab)) {
            fn_7100696b48(reinterpret_cast<UnkScorpiusAI_b59a4*>(self));
            if (self->mode == 1)
                self->vtable->vf_0x208(self, 2, true);
        }
        result = 1;
        body_hit = true;
    }
    UnkTimelineComponent* timeline = self->owner->timeline;
    if (!timeline)
        return result;
    auto play = [timeline](const u64* id) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
    };
    if (IsMask(fn_7100864018(self->owner->collision, event->collider))) {
        void* grab = self->owner->grab;
        if (grab && fn_710113d83c(grab)) {
            play(self->field_0x14c29 ? &g_unk_710164a570 : &g_unk_710164a578);
            play(self->field_0x14c29 ? &g_unk_710164a580 : &g_unk_710164a588);
        } else {
            play(self->field_0x14c29 ? &g_unk_710164a590 : &g_unk_710164a598);
            play(self->field_0x14c29 ? &g_unk_710164a5a0 : &g_unk_710164a5a8);
        }
    } else {
        play(body_hit ? &g_unk_710164a5b0 : self->field_0x14c29 ? &g_unk_710164a5a0 : &g_unk_710164a5a8);
    }
    return result;
}
