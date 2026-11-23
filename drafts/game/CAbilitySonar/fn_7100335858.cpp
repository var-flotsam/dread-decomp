// CAbilitySonar's unit (tools/tu_map.py unit 434): starts the sonar pulse. Sets the special energy drain, then
// (except in mode 2) plays the two start sounds, silencing them when they are already playing, and places
// the pulse effect at the owner; records the owner's position as the pulse origin, shows the wave model there
// and clears the pending items. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbility.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "game/UnkTU133.h"
#include "game/UnkTU134.h"
#include "game/UnkTU284.h"
#include "game/UnkTU285.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkActor;
struct UnkVec3f;
struct UnkXformNode;
struct UnkAbilitySonar_ovb0085;
struct UnkSonarEffect_ovb0085;
struct UnkSonarEffectOwner_ovb0085;
void fn_71003356ac(UnkAbilitySonar_ovb0085* self, UnkSonarEffect_ovb0085* effect, UnkSonarEffectOwner_ovb0085* owner);
void fn_7100145ef4(UnkSwitchObject* self);

extern base::reflection::UnkTunable g_unk_7172745770;  // CTunableSpecialEnergyComponent
extern base::reflection::UnkTunable g_unk_71726b0c48;  // CTunableAbilitySonar
extern const u64 g_unk_7101648fc0;
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101648fe0;  // The pulse effect's angle.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101648fec;  // The wave model's scale.

struct UnkSpecialEnergyTunable_l335858 {
    u8 pad_0x0[0x38];
    f32 sonar_drain;  // 0x38
};

struct UnkSonarTunable_l335858 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
};

struct UnkSonarOwner_l335858 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x54 - 0x48];
    f32 y;  // 0x54
    u8 pad_0x58[0x64 - 0x58];
    f32 z;  // 0x64

    CVector3D Position() const { return {x, y, z}; }
};

struct UnkSonarEnergy_l335858 {
    u8 pad_0x0[0x50];
    CVector2D drain;  // 0x50: the rate and its limit
};

struct UnkSonarEnergyLink_l335858 {
    u8 pad_0x0[0x10];
    UnkSonarEnergy_l335858* energy;  // 0x10
};

struct UnkSonarHolder_l335858 {
    u8 pad_0x0[0x30];
    UnkSonarOwner_l335858* owner;  // 0x30
    u8 pad_0x38[0xe8 - 0x38];
    UnkSonarEnergyLink_l335858* energy;  // 0xe8
};

struct UnkSonarSound_l335858 {
    UnkPlayingSound_ovb0074* sound;
    u64 id;
};

struct UnkSonarSoundRef_l335858 {
    UnkSonarSound_l335858* handle;
    u64 handle_id;

    void Silence() const {
        UnkSonarSound_l335858* h = handle;
        if (h && h->id == handle_id && handle_id != ~0ull && h->sound)
            fn_7100105c9c(h->sound, 0.0f);
    }
};

struct UnkSonarItem_l335858 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Release();  // 0x20
};

struct UnkAbilitySonar_l335858 {
    u8 pad_0x0[0xc0];
    UnkSonarHolder_l335858* holder;  // 0xc0
    u8 pad_0xc8[0x390 - 0xc8];
    UnkSwitchObject* wave;   // 0x390
    UnkSwitchObject* pulse;  // 0x398
    u8 pad_0x3a0[0x3c8 - 0x3a0];
    u32 field_0x3c8;
    u32 field_0x3cc;
    u32 field_0x3d0;
    u32 item_count;                    // 0x3d4
    UnkSonarItem_l335858* items[1];    // 0x3d8
    u8 pad_0x3e0[0x6e0 - 0x3e0];
    CVector2D origin;     // 0x6e0
    CVector2D position;   // 0x6e8
    CVector2D offset;     // 0x6f0
    u8 pad_0x6f8[0x758 - 0x6f8];
    bool field_0x758;
    u8 pad_0x759[0x760 - 0x759];
    u8 start_sound[0x18];   // 0x760
    u8 loop_sound[0x18];    // 0x778
    UnkSonarSoundRef_l335858 start_handle;  // 0x790
    UnkSonarSoundRef_l335858 loop_handle;   // 0x7a0
};
static_assert(__builtin_offsetof(UnkAbilitySonar_l335858, loop_handle) == 0x7a0, "layout");

static inline UnkSonarSoundRef_l335858 PlaySound(void* entry, UnkSonarOwner_l335858* owner) {
    using PlayFn = UnkSonarSoundRef_l335858 (*)(UnkDropTarget*, void*, UnkDropPlayer_ovb0208*, void*, u32);
    return reinterpret_cast<PlayFn>(&fn_7101023f04)(g_unk_7101d47318, entry,
                                                    reinterpret_cast<UnkDropPlayer_ovb0208*>(owner), nullptr, 0);
}

void fn_7100335858(UnkAbilitySonar_l335858* self, s32 mode) {
    fn_71002f9f18(reinterpret_cast<UnkSmLockon_A1_b64*>(self), mode);
    if (UnkSonarEnergyLink_l335858* link = self->holder->energy) {
        if (UnkSonarEnergy_l335858* energy = link->energy) {
            f32 drain = reinterpret_cast<const UnkSpecialEnergyTunable_l335858*>(&g_unk_7172745770)->sonar_drain;
            energy->drain = CVector2D{drain, drain};
        }
    }
    if (UnkSonarOwner_l335858* owner = self->holder->owner) {
        CVector3D pos = owner->Position();
        if (mode != 2) {
            self->start_handle = PlaySound(self->start_sound, owner);
            self->loop_handle = PlaySound(self->loop_sound, owner);
            self->start_handle.Silence();
            self->loop_handle.Silence();
            if (UnkSwitchObject* pulse = self->pulse) {
                fn_7100208a40(pulse->field_0x28);
                fn_7100146e60(self->pulse, true);
                CVector3D at = owner->Position();
                fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(self->pulse), reinterpret_cast<const UnkVec3f*>(&at));
                fn_7100146d98(reinterpret_cast<UnkFxObject_ovb0041*>(self->pulse),
                              reinterpret_cast<const UnkVec3f*>(&g_unk_7101648fe0));
                fn_71003356ac(reinterpret_cast<UnkAbilitySonar_ovb0085*>(self),
                              reinterpret_cast<UnkSonarEffect_ovb0085*>(self->pulse),
                              reinterpret_cast<UnkSonarEffectOwner_ovb0085*>(owner));
            }
        }
        fn_7100d7c828(reinterpret_cast<UnkActor*>(owner), &g_unk_7101648fc0, false);
        self->field_0x3d0 = reinterpret_cast<const UnkSonarTunable_l335858*>(&g_unk_71726b0c48)->field_0x28;
        CVector2D origin;
        fn_710007b430(&origin, &pos);
        self->origin.x = origin.x;
        self->origin.y = origin.y;
    }
    self->position.x = self->origin.x;
    self->position.y = self->origin.y;
    self->offset.x = 0.0f;
    self->offset.y = 0.0f;
    self->field_0x3cc = self->field_0x3c8;
    if (UnkSwitchObject* wave = self->wave) {
        fn_7100145e94(wave, true);
        fn_7100145ef4(self->wave);
        fn_7100207be8(reinterpret_cast<UnkXformNode*>(self->wave->field_0x28),
                      reinterpret_cast<const UnkVec3f*>(&g_unk_7101648fec));
        {
            CVector3D at = {self->position.x, self->position.y, 0.0f};
            fn_7100145f28(self->wave, reinterpret_cast<const UnkVec3f*>(&at));
        }
        f32 visible = 1.0f;
        fn_7100219294(g_unk_7101d47300, 0x90e4f789f8288231ull, &visible, 0);
    }
    UnkSonarItem_l335858** end = self->items + self->item_count;
    for (UnkSonarItem_l335858** item = self->items; item != end; ++item)
        (*item)->Release();
    self->item_count = 0;
    self->field_0x758 = false;
}
