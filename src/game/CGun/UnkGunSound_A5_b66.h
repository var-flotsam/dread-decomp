// Gun sound play shared by CGrappleBeamGun and CGun: plays a sound on the global sound player
// (fn_7100ed659c) and, when the handle is still live and the parameters ask for it (+0x14b), sets the
// sound's level (+0x14c) at once (fn_7100105f94). Names are placeholders.
#pragma once
#include "game/UnkTU101.h"
#include "types.h"

struct UnkInterp_7100105f94;

struct UnkGunSoundRef_A5_b66 {
    UnkInterp_7100105f94* sound;
    s64 serial;
};

struct UnkGunSoundHandle_A5_b66 {
    UnkGunSoundRef_A5_b66* ref;
    s64 serial;
};

struct UnkGunSoundModeSource_A5_b66 {
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
    virtual u32 vf_0x90(s32 index);  // the sound mode
};

struct UnkGunSoundModeHolder_A5_b66 {
    u8 pad_0x0[0x158];
    UnkGunSoundModeSource_A5_b66* source;  // 0x158
};

struct UnkGunSoundModeLink_A5_b66 {
    u8 pad_0x0[0x10];
    UnkGunSoundModeHolder_A5_b66* holder;  // 0x10
};

struct UnkGunSoundParams_A5_b66 {
    u8 pad_0x0[0x30];
    UnkGunSoundModeLink_A5_b66* link;  // 0x30
    u8 pad_0x38[0x113];
    bool has_level;  // 0x14b
    s32 level;       // 0x14c
};

struct UnkGunSoundOwner_A5_b66 {
    u8 pad_0x0[0x48];
    s32 field_0x48;
    u8 pad_0x4c[0x1828];
    u32 sound_id;  // 0x1874
};

// The mode a sound plays in: `none` without parameters, else what the parameters' source says (0 without one).
__attribute__((always_inline)) static inline u32 GunSoundMode_A5_b66(UnkGunSoundParams_A5_b66* params, u32 none) {
    if (!params)
        return none;
    if (params->link && params->link->holder && params->link->holder->source)
        return params->link->holder->source->vf_0x90(-1);
    return 0;
}

extern void* g_unk_71726af0b0;  // what plays the sounds
UnkGunSoundHandle_A5_b66 fn_7100ed659c(void* player, void* sound, u32 id, u32 kind, u32 mode);

__attribute__((always_inline)) static inline void PlayGunSoundId_A5_b66(void* player, void* sound, u32 id, u32 kind,
                                                                        u32 mode, UnkGunSoundParams_A5_b66* params) {
    UnkGunSoundHandle_A5_b66 h = fn_7100ed659c(player, sound, id, kind, mode);
    UnkInterp_7100105f94* sound_obj =
        h.ref && h.ref->serial == h.serial && h.serial != -1 ? h.ref->sound : nullptr;
    if (params && sound_obj && params->has_level)
        fn_7100105f94(sound_obj, params->level, 0.0f);
}

__attribute__((always_inline)) static inline void PlayGunSound_A5_b66(void* player, UnkGunSoundOwner_A5_b66* self,
                                                                      void* sound, u32 kind, u32 mode,
                                                                      UnkGunSoundParams_A5_b66* params) {
    PlayGunSoundId_A5_b66(player, sound, self->sound_id, kind, mode, params);
}
