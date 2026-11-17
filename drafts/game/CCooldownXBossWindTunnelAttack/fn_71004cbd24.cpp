// CCooldownXBossWindTunnelAttack's unit: advances to the next step of the picked pattern (pool at +0x68),
// starts the wind and sends the owner's character class the pattern's request when the step asks for it,
// then spawns up to six fans of the step from the last, each linked to the one spawned before it.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPool;
struct UnkEntry;
struct UnkWindTunnel_b59;
struct UnkLinked;
struct UnkWeakTarget;

UnkEntry* fn_71004ba10c(UnkPool* pool, u32 index, u32 flag);
void fn_7100f0e238(void* target, bool on);
void fn_71004cc644(UnkWindTunnel_b59* self);
void* fn_71004cc860(void* owner);
void fn_71004d7980(UnkLinked* self, UnkWeakTarget* other);

extern CStrId g_unk_71726d80f8[2] __attribute__((visibility("hidden")));

struct UnkFanSlot_l4cbd24 {
    u8 pad_0x0[0x8];
    bool enabled;
    u8 pad_0x9[3];
    u32 field_0xc;
    bool field_0x10;
    u8 pad_0x11[7];
};

struct UnkStep_l4cbd24 {
    u8 pad_0x0[0x8];
    UnkFanSlot_l4cbd24* slots;
    u32 count;
    u8 pad_0x14[0x1c];
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
    bool field_0x39;
    bool field_0x3a;
};

struct UnkPatternId_l4cbd24 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkPattern_l4cbd24 {
    u8 pad_0x0[0x110];
    UnkPatternId_l4cbd24* id;
};

struct UnkPosition_l4cbd24 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkOwner_l4cbd24 {
    UnkPosition_l4cbd24 Position() const {
        UnkPosition_l4cbd24 position;
        position.x = x;
        position.y = y;
        position.z = z;
        return position;
    }
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x220 - 0x68];
    void* field_0x220;
};

struct UnkFan_l4cbd24 {
    void** vtable;
    void vf_0xd0(const UnkPosition_l4cbd24* position, bool snap) {
        reinterpret_cast<void (*)(UnkFan_l4cbd24*, const UnkPosition_l4cbd24*, bool)>(vtable[0xd0 / 8])(
            this, position, snap);
    }
};

struct UnkFanMovement_l4cbd24 {
    void** vtable;
    void vf_0x290(UnkOwner_l4cbd24* owner) {
        reinterpret_cast<void (*)(UnkFanMovement_l4cbd24*, UnkOwner_l4cbd24*)>(vtable[0x290 / 8])(this, owner);
    }
    u8 pad_0x8[0x5e0 - 0x8];
    u32 field_0x5e0;
    bool field_0x5e4;
    bool field_0x5e5;
};

struct alignas(8) UnkSpawnKind_l4cbd24 {
    u32 id;
};

struct UnkWindTunnel_l4cbd24 {
    u8 pad_0x0[0x8];
    UnkOwner_l4cbd24* owner;
    u8 pad_0x10[0x58];
    UnkPattern_l4cbd24* pattern;
    u8 pad_0x70[0x4];
    f32 field_0x74;
    u8 pad_0x78[0x4];
    u32 index;
    u32 step;
    u8 pad_0x84[0x8];
    bool field_0x8c;
};

static UnkStep_l4cbd24* StepAt(UnkPattern_l4cbd24* pattern, u32 index, u32 step) {
    return reinterpret_cast<UnkStep_l4cbd24*>(fn_71004ba10c(reinterpret_cast<UnkPool*>(pattern), index, step));
}

void fn_71004cbd24(UnkWindTunnel_l4cbd24* self) {
    UnkStep_l4cbd24* entry = StepAt(self->pattern, self->index, self->step);
    if (!entry)
        return;
    self->step++;
    UnkStep_l4cbd24* next = StepAt(self->pattern, self->index, self->step);
    self->field_0x74 = next ? next->field_0x30 : 0.0f;

    if (entry->field_0x39) {
        fn_7100f0e238(g_unk_7101d47310->field_0x38, true);
        fn_71004cc644(reinterpret_cast<UnkWindTunnel_b59*>(self));
    }
    if (entry->field_0x3a) {
        if (void* target = self->owner->field_0x220) {
            UnkSpitSend(target, self->pattern->id->hash, 0xf4061ff25a2163d6);
            self->field_0x8c = fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        }
    }

    UnkFanMovement_l4cbd24* previous = nullptr;
    for (u32 i = entry->count - 1, spawned = 0; i < entry->count; i--, spawned++) {
        UnkFanSlot_l4cbd24* slot = &entry->slots[i];
        if (slot->enabled) {
            CStrId* id = slot->field_0x10 ? &g_unk_71726d80f8[1] : &g_unk_71726d80f8[0];
            UnkFan_l4cbd24* fan;
            {
                UnkSpawnKind_l4cbd24 kind;
                UnkActorDefFactory* factory = g_unk_71726a0220;
                UnkStringData* data = id->data;
                ++data->reads;
                kind.id = g_unk_7172a0ba48;
                fan = reinterpret_cast<UnkFan_l4cbd24*>(
                    fn_710119438c(factory, reinterpret_cast<const char*>(&data->String()), id,
                                  reinterpret_cast<const UnkPowerUpLifeType_b52&>(kind), nullptr));
            }
            if (fan) {
                UnkPosition_l4cbd24 position = self->owner->Position();
                position.x = position.x + 700.0f + entry->field_0x34;
                position.y = spawned * 200.0f + 100.0f + position.y;
                fan->vf_0xd0(&position, true);
                UnkFanMovement_l4cbd24* movement = static_cast<UnkFanMovement_l4cbd24*>(fn_71004cc860(fan));
                if (movement) {
                    movement->vf_0x290(self->owner);
                    movement->field_0x5e0 = slot->field_0xc;
                    movement->field_0x5e4 = slot->field_0x10;
                    movement->field_0x5e5 = entry->field_0x38;
                    fn_71004d7980(reinterpret_cast<UnkLinked*>(movement), reinterpret_cast<UnkWeakTarget*>(previous));
                    previous = movement;
                }
            }
        }
        if (spawned > 4)
            break;
    }
}
