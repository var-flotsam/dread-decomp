// CGunComponent unit (tools/tu_map.py unit 1304): switches the gun to the weapon named by `id`. With
// `primary`, the first group whose front weapon answers to the id becomes the alternative weapon (the
// old one is remembered when `remember` is set); otherwise the first weapon of the main groups that
// answers to it (or has exactly that id, with `exact`) becomes the main weapon. Returns whether the
// weapon changed. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// Animation flags of the weapon switch.
extern const u64 g_unk_710164ac70;
extern const u64 g_unk_710164ac78;
extern const u64 g_unk_710164ac80;
extern const u64 g_unk_710164ac88;

// A weapon of a group. Only the slots this function calls are named.
struct UnkGunWeapon_l83f32c {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual bool IsSelectable();  // 0x30
    u8 pad_0x8[0x18];
    CStrId id;  // 0x20
};

using UnkGunWeaponGroup_l83f32c = CRntVector<UnkGunWeapon_l83f32c*>;

struct UnkGunComponent_l83f32c {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8();
    virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138(); virtual void vf_0x140(); virtual void vf_0x148();
    virtual void vf_0x150(); virtual void vf_0x158(); virtual void vf_0x160(); virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178();
    virtual void vf_0x180(); virtual void vf_0x188(); virtual void vf_0x190(); virtual void vf_0x198(); virtual void vf_0x1a0();
    virtual void SetCurrent(UnkGunWeapon_l83f32c* weapon, bool a, bool b);    // 0x1a8
    virtual void ReleaseCurrent(UnkGunWeapon_l83f32c* weapon, bool a);         // 0x1b0
    virtual bool Answers(const CStrId* weapon_id, const CStrId* id);           // 0x1b8

    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x90];
    CRntVector<UnkGunWeaponGroup_l83f32c> primary;    // 0xc8
    CRntVector<UnkGunWeaponGroup_l83f32c> secondary;  // 0xf0
    UnkGunWeapon_l83f32c* alt;   // 0x118
    UnkGunWeapon_l83f32c* main;  // 0x120
    bool use_alt;                // 0x128
    u8 pad_0x129[0x17];
    base::UnkStrIdVec previous;  // 0x140; the alternative weapons switched away from
    CStrId id;                   // 0x168
    u8 pad_0x170[0x26];
    bool alt_flag;   // 0x196
    bool main_flag;  // 0x197
};
static_assert(__builtin_offsetof(UnkGunComponent_l83f32c, previous) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l83f32c, main_flag) == 0x197, "layout");

// Whether `ids` holds `id`.
static inline bool Contains(const base::UnkStrIdVec& ids, const CStrId& id) {
    for (u32 i = 0; i < ids.count; ++i)
        if (ids.data[i].data == id.data)
            return true;
    return false;
}

struct UnkSamusGunComponent;

bool fn_710083f32c(UnkSamusGunComponent* gun, const CStrId* id, bool primary, bool remember, bool exact) {
    UnkGunComponent_l83f32c* self = reinterpret_cast<UnkGunComponent_l83f32c*>(gun);
    if (primary) {
        UnkGunWeaponGroup_l83f32c* group = self->primary.data;
        for (; group != self->primary.data + self->primary.count; ++group) {
            if (self->Answers(&group->data[0]->id, id) && group->data[0]->IsSelectable())
                break;
        }
        self->id = g_unk_7101d470e0;
        if (group == self->primary.data + self->primary.count || self->alt == group->data[0])
            return false;

        if (self->alt && remember) {
            base::UnkStrIdVec& previous = self->previous;
            if (!Contains(previous, self->alt->id))
                previous.PushBack(self->alt->id);
        }
        self->ReleaseCurrent(self->alt, false);
        self->alt = group->data[0];
        if (self->use_alt) {
            self->SetCurrent(self->alt, false, true);
            if (self->actor && !fn_7100ded930(g_unk_7101d47310)) {
                if (self->alt_flag) {
                    fn_7100d7c828(self->actor, &g_unk_710164ac80, false);
                    self->alt_flag = false;
                } else {
                    fn_7100d7c828(self->actor, &g_unk_710164ac70, false);
                    self->alt_flag = true;
                }
            }
        }
        return true;
    }

    for (UnkGunWeaponGroup_l83f32c* group = self->secondary.data;
         group != self->secondary.data + self->secondary.count; ++group) {
        for (UnkGunWeapon_l83f32c** it = group->data; it != group->data + group->count; ++it) {
            if ((exact && (*it)->id.data == id->data) || (self->Answers(&(*it)->id, id) && (*it)->IsSelectable())) {
                if (self->main != group->data[0]) {
                    self->ReleaseCurrent(self->main, false);
                    self->main = *it;
                    self->SetCurrent(self->main, false, true);
                    if (self->actor && !fn_7100ded930(g_unk_7101d47310)) {
                        if (self->main_flag) {
                            fn_7100d7c828(self->actor, &g_unk_710164ac88, false);
                            self->main_flag = false;
                        } else {
                            fn_7100d7c828(self->actor, &g_unk_710164ac78, false);
                            self->main_flag = true;
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}
