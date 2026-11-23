// CGunComponent unit (tools/tu_map.py unit 1304): advances the beam charge by `dt` (signalling the weapon when
// it reaches the start and the gun when it is full), then, while the secondary charge is on, advances the
// charge of the secondary weapon (+0x4c8 of the id table) and fires it when full. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

extern const u64 g_unk_710164ac60;
extern const u64 g_unk_710164ac68;
extern base::reflection::UnkTunable g_unk_7172726228;

// The gun tunable's charge threshold.
struct UnkGunTunable_l83e4fc {
    u8 pad_0x0[0x24];
    f32 threshold;  // 0x24
};

struct UnkGunWeapon_l83e4fc {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual bool IsSelectable();  // 0x30
    virtual void vf_0x38(); virtual void vf_0x40();
    virtual bool IsCharging();  // 0x48
    virtual void vf_0x50(); virtual void vf_0x58(); virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70();
    virtual void Cancel();  // 0x78
    virtual void vf_0x80(); virtual void vf_0x88(); virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0();
    virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8(); virtual void vf_0xc0();
    virtual void StartCharge(s32 mode, bool flag);  // 0xc8
    u8 pad_0x8[0x18];
    CStrId id;  // 0x20
};

using UnkGunWeaponGroup_l83e4fc = base::global::CRntVector<UnkGunWeapon_l83e4fc*>;

struct UnkGunActor_l83e4fc {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkGunComponent_l83e4fc {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8();
    virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138(); virtual void vf_0x140(); virtual void vf_0x148();
    virtual void vf_0x150(); virtual void vf_0x158(); virtual void vf_0x160(); virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178();
    virtual void vf_0x180(); virtual void vf_0x188();
    virtual void OnChargeFull();  // 0x190

    u8 pad_0x8[0x28];
    UnkGunActor_l83e4fc* actor;  // 0x30
    u8 pad_0x38[0xb8];
    base::global::CRntVector<UnkGunWeaponGroup_l83e4fc> secondary;  // 0xf0
    UnkGunWeapon_l83e4fc* alt;   // 0x118
    UnkGunWeapon_l83e4fc* main;  // 0x120
    bool use_alt;                // 0x128
    u8 pad_0x129[0x8];
    bool cancel_charge;  // 0x131
    u8 pad_0x132[0x2];
    s32 mode;  // 0x134
    u8 pad_0x138[0x38];
    f32 charge_start;  // 0x170
    f32 charge_full;   // 0x174
    bool charging;     // 0x178
    u8 pad_0x179[0x3];
    f32 charge;            // 0x17c
    bool charge_started;   // 0x180
    u8 pad_0x181[0x9];
    bool secondary_on;     // 0x18a
    u8 pad_0x18b[0x1];
    f32 secondary_charge;  // 0x18c
    f32 secondary_full;    // 0x190
};
static_assert(__builtin_offsetof(UnkGunComponent_l83e4fc, secondary) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l83e4fc, secondary_full) == 0x190, "layout");

// `value` kept within 0 and `max`.
static inline f32 ClampCharge(f32 value, const f32& max) {
    if (value <= 0.0f)
        return 0.0f;
    return value >= max ? max : value;
}

// The first weapon of the groups with the given id that is selectable; null if none.
static inline UnkGunWeapon_l83e4fc* FindWeapon(base::global::CRntVector<UnkGunWeaponGroup_l83e4fc>& groups,
                                                const CStrId& id) {
    for (UnkGunWeaponGroup_l83e4fc* g = groups.data; g != groups.data + groups.count; ++g) {
        for (UnkGunWeapon_l83e4fc** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id == id && (*it)->IsSelectable())
                return *it;
        }
    }
    return nullptr;
}

// Gives the timeline the key `id` with every flag set.
static inline void SetKey(UnkTimelineComponent* timeline, const u64* id, base::reflection::UnkReflTypeIdArg& mask) {
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

void fn_710083e4fc(UnkGunComponent_l83e4fc* self, f32 dt) {
    if (self->charging) {
        if (self->cancel_charge) {
            fn_7100839b00(reinterpret_cast<char*>(self), true);
        } else {
            f32 previous = self->charge;
            self->charge += dt;
            f32 charge = ClampCharge(self->charge, self->charge_full);
            self->charge = charge;
            if (UnkGunWeapon_l83e4fc* weapon = self->use_alt ? self->alt : self->main) {
                f32 start = self->charge_start;
                if (charge >= start && ((start == 0.0f) | (previous < start))) {
                    weapon->StartCharge(self->mode, false);
                    self->charge_started = true;
                }
                if (self->charging && self->charge >= self->charge_full && previous < self->charge_full)
                    self->OnChargeFull();
            }
        }
    }

    if (!self->secondary_on)
        return;
    UnkTimelineComponent* timeline = self->actor ? self->actor->timeline : nullptr;
    UnkGunWeapon_l83e4fc* weapon = FindWeapon(self->secondary, GameManagerIdTable().field_0x4c8);
    if (!weapon)
        return;
    base::reflection::UnkReflTypeIdArg charged_mask;
    base::reflection::UnkReflTypeIdArg fired_mask;
    bool charging = weapon->IsCharging();
    f32 previous = self->secondary_charge;
    if (charging)
        self->secondary_charge = ClampCharge(previous + dt, self->secondary_full);
    f32 threshold = reinterpret_cast<UnkGunTunable_l83e4fc*>(&g_unk_7172726228)->threshold;
    if (previous < threshold && self->secondary_charge >= threshold) {
        if (charging)
            SetKey(timeline, &g_unk_710164ac60, charged_mask);
        else {
            weapon->Cancel();
            return;
        }
    }
    if (charging && self->secondary_charge >= self->secondary_full && previous < self->secondary_full) {
        fn_710083f32c(reinterpret_cast<UnkSamusGunComponent*>(self), &GameManagerIdTable().field_0x4c8, false,
                      false, false);
        if (timeline) {
            fn_7100a2b7b8(timeline, &g_unk_710164ac60);
            SetKey(timeline, &g_unk_710164ac68, fired_mask);
        }
    }
}
