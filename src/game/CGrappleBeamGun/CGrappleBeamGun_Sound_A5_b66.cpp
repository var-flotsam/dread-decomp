// CGrappleBeamGun's sound plays (kind 5); two identical copies.
#include "../CGun/UnkGunSound_A5_b66.h"

void fn_7100833bfc(UnkGunSoundOwner_A5_b66* self, void*, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    void* player = g_unk_71726af0b0;
    asm("" : "+r"(self));  // the original reads the id after the player
    PlayGunSound_A5_b66(player, self, sound, 5, 1, params);
}

void fn_7100833c78(UnkGunSoundOwner_A5_b66* self, void*, void* sound, void*, UnkGunSoundParams_A5_b66* params) {
    void* player = g_unk_71726af0b0;
    asm("" : "+r"(self));  // the original reads the id after the player
    PlayGunSound_A5_b66(player, self, sound, 5, 1, params);
}
