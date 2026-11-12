// Unit 2615 of tools/tu_map.py (the dialogs with ADAM, src/game/UnkAdamDialog.cpp): the close callback of a dialog
// page (fn_7100fb9564: fires the pending callback once nothing is queued, fades the voice out and marks the dialog
// done) and the dialog's end (fn_7100fb927c: fades the voice, releases the queued string ids, sends "OnExit" to the
// dialog target and restores the music level). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/GUI/CDialogComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkSoundRef_ovb0215 {
    UnkSound* object;
    s64 serial;
};
struct UnkSoundHandle_ovb0215 {
    UnkSoundRef_ovb0215* ref;
    s64 serial;
};

// Tells the playing sound (if any) to fade out.
static inline void FadeOut(UnkSoundHandle_ovb0215* handle) {
    UnkSoundRef_ovb0215* ref = handle->ref;
    if (ref && ref->serial == handle->serial && handle->serial != -1 && ref->object)
        fn_7100105a80(ref->object, 0.2f, 0.0f);
}

struct UnkCallback_ovb0215 {
    void* object;
    void (*fn)(void*);
};

struct UnkDialogState_ovb0215 {
    u8 pad_0x0[8];
    UnkSoundHandle_ovb0215 voice;  // 0x8
    UnkCallback_ovb0215 on_close;  // 0x18
    u8 pad_0x28[0x38];
    CStrId* ids;  // 0x60
    u32 id_count; // 0x68
    u8 pad_0x6c[0x20];
    f32 music_level;  // 0x8c
    u8 pad_0x90[9];
    bool done;  // 0x99
};
static_assert(__builtin_offsetof(UnkDialogState_ovb0215, music_level) == 0x8c, "layout");
static_assert(__builtin_offsetof(UnkDialogState_ovb0215, done) == 0x99, "layout");

void fn_7100fb9564(void* object) {
    UnkDialogState_ovb0215* self = static_cast<UnkDialogState_ovb0215*>(object);
    if (self->on_close.fn && self->id_count == 0) {
        self->on_close.fn(self->on_close.object);
        self->on_close = UnkCallback_ovb0215();
    }
    FadeOut(&self->voice);
    self->done = true;
}

struct UnkMissionLogTarget_ovb0079 {
    u8 unk_0x0[0x260];
    CStrId field_0x260;
    u8 field_0x268[0x8];
};
void fn_7100b02384(UnkMissionLogTarget_ovb0079* self);

// The target's id slot taking a temporary's id (the move assignment, with the temporary's release folded away).
struct UnkIdSlot_ovb0215 : base::global::UnkStrIdBase {
    void Take(base::global::UnkStrId& other) { MoveFrom(other); }
};

struct UnkDialogHolder {
    u8 unk_0x0[0x10];
    UnkMissionLogTarget_ovb0079* target;
};

extern UnkMgr_0xf18* g_unk_71726bc560;
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

void fn_7100fb927c(UnkDialogState_ovb0215* self) {
    FadeOut(&self->voice);
    for (u64 i = 0; i < self->id_count; ++i)
        self->ids[i].Reset();
    self->id_count = 0;
    UnkDialogHolder* holder =
        *reinterpret_cast<UnkDialogHolder**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2260);
    if (holder) {
        if (UnkMissionLogTarget_ovb0079* target = holder->target) {
            UnkGuiEvent event;
            {
                s32 kind = 0x12;
                fn_710120235c(&event, reinterpret_cast<UnkGuiDisplayObject*>(target), &kind, nullptr);
            }
            {
                base::global::UnkStrId exit_id("OnExit", true);
                reinterpret_cast<UnkIdSlot_ovb0215*>(&target->field_0x260)->Take(exit_id);
            }
            reinterpret_cast<void (*)(UnkMissionLogTarget_ovb0079*, s32)>(
                (*reinterpret_cast<void***>(target))[0x1e0 / 8])(target, 0);
            fn_7100b018fc(target, &event);
            fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(target), false);
            fn_7100b02384(target);
            fn_7101202488(&event);
        }
    }
    if (self->music_level >= 0.0f)
        fn_7100fd7af8(MusicManager(), self->music_level, 0.2f);
}
