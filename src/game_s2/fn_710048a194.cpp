// Per-frame update of a timer pair (+0x138 runs up, +0x13c counts down) for a player action: advances
// the first by the frame time, lets fn_710048a070 run when the flag at +0x133 is clear and the
// owner's attack is not a melee damage track, then, while the player's attack is a fade-to-black
// death track and the float at +0x54 of the player object (+0x1c0) is not positive, either starts
// the countdown (0.33 s, after a player call, unless the effect object at +0x1040 of the manager is
// busy) or runs it down and calls vtable slot 0x180 of the player object when it expires.
#include "types.h"

struct UnkPlayerObject {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 field_0x54;
    void vf_0x180(u32 a, u32 b) { reinterpret_cast<void (*)(UnkPlayerObject*, u32, u32)>(vtable[0x30])(this, a, b); }
};

struct UnkAttack;

struct UnkPlayer {
    u8 pad_0x0[0x1c0];
    UnkPlayerObject* field_0x1c0;
    u8 pad_0x1c8[0x58];
    UnkAttack* field_0x220;
};

struct UnkOwner {
    u8 pad_0x0[0x220];
    UnkAttack* field_0x220;
};

struct UnkSelf {
    u8 pad_0x0[0x133];
    u8 field_0x133;
    u8 pad_0x134[4];
    f32 field_0x138;
    f32 field_0x13c;
};

struct UnkVec {
    u64 xy;
    u32 z;
};

struct UnkManager;
extern UnkManager* g_unk_7101d47310;
extern const char g_unk_7101649988[] __attribute__((visibility("hidden")));
void* fn_7100d19258();  // CCharClassMeleeDamageTrack::refl_GetClass
void* fn_7100c19578();  // CCharClassFadeToBlackDeathTrack::refl_GetClass
bool fn_710117b348(UnkAttack* object, void* class_ptr);
UnkOwner* fn_7100defcd4(UnkManager* manager);
UnkPlayer* fn_7100dec788(UnkManager* manager);
void fn_710048a070(UnkSelf* self);
void fn_7100d7c828(UnkPlayer* player, const char* name, u32 flag);
bool fn_7101053a7c(void* effects);
bool fn_7101053a0c(void* effects);
void fn_7101053850(void* effects, UnkVec* position, u32 flag, f32 duration, f32 scale);

void fn_710048a194(UnkSelf* self, f32 delta) {
    self->field_0x138 = self->field_0x138 + delta;
    if (!self->field_0x133) {
        if (!fn_710117b348(fn_7100defcd4(g_unk_7101d47310)->field_0x220, fn_7100d19258()))
            fn_710048a070(self);
    }
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player) {
        UnkPlayerObject* object = player->field_0x1c0;
        if (object) {
            UnkAttack* attack = player->field_0x220;
            if (attack && fn_710117b348(attack, fn_7100c19578()) && object->field_0x54 <= 0.0f) {
                f32 timer = self->field_0x13c;
                if (timer < 0.0f) {
                    player = fn_7100dec788(g_unk_7101d47310);
                    if (player)
                        fn_7100d7c828(player, g_unk_7101649988, 0);
                    char* effects = reinterpret_cast<char*>(g_unk_7101d47310) + 0x1040;
                    if (!fn_7101053a7c(effects) && !fn_7101053a0c(effects)) {
                        self->field_0x13c = 0.33f;
                        UnkVec position;
                        position.xy = 0;
                        position.z = 0;
                        fn_7101053850(effects, &position, 1, 0.33f, 1.0f);
                    }
                } else {
                    timer = timer - delta;
                    self->field_0x13c = timer;
                    if (timer < 0.0f)
                        object->vf_0x180(0, 0);
                }
            }
        }
    }
}
