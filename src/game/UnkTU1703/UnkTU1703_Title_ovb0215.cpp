// Unit 1703 (tools/tu_map.py, no name hint; the title screen of src/game/UnkUnit1703/UnkUnit1703_TitleLabels_b49.cpp
// and the main menu's member at +0x2f8): the destructor of the member that holds a sound handle (+0x30) and fades
// the sound out, and the title screen's phases that fade their sound (0.2 and 0), tell the scene-state singleton
// (g_unk_7101d47320, created on first use) the music modes 0x48, 0x49 (and 0x4a), and step the screen on.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2578.h"
#include "types.h"

extern char g_unk_7101a9d190[];  // the member's vtable

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

// Tells the playing sound to fade out and forgets it.
static inline void FadeOut(UnkSoundHandle* handle) {
    UnkSoundRef* ref = handle->ref;
    if (ref && ref->serial == handle->serial && handle->serial != -1 && ref->object) {
        fn_7100105a80(ref->object, 0.2f, 0.0f);
        *handle = g_unk_7108b6c460;
    }
}

struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100f011c4(UnkStruct_0x128* self, u32 mode);
bool fn_7100f00778(UnkStruct_0x128* self);

static inline UnkStruct_0x128* Music() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (!m) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

struct UnkMainMenuMember2f8_ovb0126 {
    char* vtable;
    u64 field_0x8;
    u8 pad_0x10[0x20];
    UnkSoundHandle sound;  // 0x30
};

void fn_7100b52460(UnkMainMenuMember2f8_ovb0126* self) {
    self->vtable = g_unk_7101a9d190 + 0x10;
    FadeOut(&self->sound);
    self->vtable = g_unk_7101a9c360 + 0x10;
    self->field_0x8 = 0;
}

struct UnkTitlePhase_ovb0215 {
    u8 pad_0x0[0x30];
    UnkSoundHandle sound;  // 0x30
};

void fn_7100b54418(UnkTitlePhase_ovb0215* self) {
    if (!fn_7100f00778(Music()))
        return;
    FadeOut(&self->sound);
    fn_7100f011c4(Music(), 0x48);
    fn_7100f011c4(Music(), 0x49);
}

struct UnkMusicState_ovb0215 {
    u8 pad_0x0[0x11c];
    f32 volume;  // 0x11c
};

struct UnkTitleScreenB {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkSoundHandle sound;  // 0x30
};

void fn_7100b541d0(UnkTitleScreenB* self) {
    if (!fn_7100f00778(Music()))
        return;
    FadeOut(&self->sound);
    fn_7100f011c4(Music(), 0x48);
    fn_7100f011c4(Music(), 0x49);
    fn_7100f01838(Music(), 0x4a);
    fn_7100f00714(Music());
    fn_7100f003e8(Music(), 0x32);
    reinterpret_cast<UnkMusicState_ovb0215*>(Music())->volume = 0.75f;
}

using base::global::CRntString;
using base::global::CStrId;

void* fn_7100deab70(UnkGameManager* self, const CRntString& name);
struct UnkMenuSoundOwner_l191198;
void fn_7101191198(UnkMenuSoundOwner_l191198* self, f32 value);

struct UnkTitleScreenA {
    void* vtable;
    u8 pad_0x8[8];
    s32 state;  // 0x10
    u8 pad_0x14[0x1c];
    UnkSoundHandle sound;  // 0x30
};

void fn_7100b53fa0(UnkTitleScreenA* self) {
    if (!fn_7100f00778(Music()))
        return;
    FadeOut(&self->sound);
    void* menu = fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU"));
    if (menu)
        fn_7101191198(static_cast<UnkMenuSoundOwner_l191198*>(menu), 0.0f);
    fn_7100f011c4(Music(), 0x48);
    fn_7100f011c4(Music(), 0x4a);
    fn_7100f01838(Music(), 0x49);
    fn_7100f00714(Music());
    if (fn_7100f00778(Music()))
        fn_7100f003e8(Music(), -0x87);
    self->state = 1;
}

struct UnkTitleTarget;
void fn_7100b53448(UnkTitleScreenA* self);
void fn_7100b538b0(UnkTitleScreenA* self, UnkTitleTarget* target);
void fn_7100b53b44(UnkTitleScreenA* self, UnkTitleTarget* target, f32 dt);
struct UnkTitleScreenB;
void fn_7100b53d58(UnkTitleScreenB* self, f32 dt);

struct UnkTitleScreenSteps_ovb0215 {
    void* vtable;
    UnkGuiDisplayObject* root;  // 0x8
    s32 state;                  // 0x10
    u8 pad_0x14[4];
    f32 delay;  // 0x18
    u8 pad_0x1c[0xc];
    f32 timer;  // 0x28
};

void fn_7100b532a0(UnkTitleScreenSteps_ovb0215* self, f32 dt, UnkTitleTarget* target) {
    if (!fn_7100f00778(Music())) {
        self->delay = 0.001f;
        self->state = 3;
    }
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f) {
            UnkGuiDisplayObject* press = fn_71011d94d4(self->root, CStrId("PressALabel", true));
            reinterpret_cast<void (*)(UnkGuiDisplayObject*, const char*, s32)>(
                (*reinterpret_cast<void***>(press))[0x1e8 / 8])(press, "Loop", 0);
        }
    }
    switch (self->state) {
    case 0:
        fn_7100b53448(reinterpret_cast<UnkTitleScreenA*>(self));
        break;
    case 1:
        fn_7100b538b0(reinterpret_cast<UnkTitleScreenA*>(self), target);
        break;
    case 2:
        fn_7100b53b44(reinterpret_cast<UnkTitleScreenA*>(self), target, dt);
        break;
    case 3:
        fn_7100b53d58(reinterpret_cast<UnkTitleScreenB*>(self), dt);
        break;
    }
}
