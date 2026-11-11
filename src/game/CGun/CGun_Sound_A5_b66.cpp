// CGun's sound plays: the kind comes from the shot flags (0x8000000 -> 7, else 0x400 -> 6, else 5), mode 3.
#include "UnkGunSound_A5_b66.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU811.h"

__attribute__((always_inline)) static inline u32 GunSoundKind_A5_b66(u32 flags) {
    u32 kind = (flags & 0x400) ? 6 : 5;
    if (flags & 0x8000000)
        kind = 7;
    return kind;
}

void fn_71008364e4(UnkGunSoundOwner_A5_b66* self, u32 flags, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    asm("" : "+r"(self));  // pins the order: kind, player, then the id
    u32 kind = GunSoundKind_A5_b66(flags);
    void* player = g_unk_71726af0b0;
    PlayGunSound_A5_b66(player, self, sound, kind, 3, params);
}

void fn_7100836578(UnkGunSoundOwner_A5_b66* self, u32 flags, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    asm("" : "+r"(self));  // pins the order: kind, player, then the id
    u32 kind = GunSoundKind_A5_b66(flags);
    void* player = g_unk_71726af0b0;
    PlayGunSound_A5_b66(player, self, sound, kind, 3, params);
}

// Same with the mode passed in, then the game manager's deferred set at +0x20a8 is told about the sound.
void fn_7100836340(UnkGunSoundOwner_A5_b66* self, u32 flags, void* sound, u32 mode, UnkGunSoundParams_A5_b66* params) {
    asm("" : "+r"(self));
    u32 kind = GunSoundKind_A5_b66(flags);
    void* player = g_unk_71726af0b0;
    PlayGunSound_A5_b66(player, self, sound, kind, mode, params);
    fn_710056a010(reinterpret_cast<char*>(g_unk_7101d47310) + 0x20a8, sound);
}

// With the mode from the parameters (2 without them): first copies the game manager's +0x2444 to +0x48.
void fn_71008363e4(UnkGunSoundOwner_A5_b66* self, u32 flags, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    self->field_0x48 = *reinterpret_cast<s32*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2444);
    u32 kind = GunSoundKind_A5_b66(flags);
    u32 mode = GunSoundMode_A5_b66(params, 2);
    PlayGunSound_A5_b66(g_unk_71726af0b0, self, sound, kind, mode, params);
    fn_710056a010(reinterpret_cast<char*>(g_unk_7101d47310) + 0x20a8, sound);
}

// Same without the notification, mode 3 without parameters.
void fn_710083660c(UnkGunSoundOwner_A5_b66* self, u32 flags, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    u32 kind = GunSoundKind_A5_b66(flags);
    void* player = g_unk_71726af0b0;
    u32 id = self->sound_id;
    u32 mode = GunSoundMode_A5_b66(params, 3);
    PlayGunSoundId_A5_b66(player, sound, id, kind, mode, params);
}
