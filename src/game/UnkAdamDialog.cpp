// From unit 2615 of tools/tu_map.py (0x7100fb790c-0x7100fb969c, no reflected class): the dialogs
// with ADAM and the professor ("DIAG_ADAM_", "#TERM_CHARACTER_ADAM", "OnPageOpen", "OnPageClose").
// Names and layouts are placeholders.
// Holds fn_7100fb85cc (plays a dialog's voice and opens its page) and fn_7100fb89b4 (opens a dialog
// page with a localized text).
#include <stdio.h>
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A handle on a sound: the reference and the serial it was taken with; the sound is gone once the
// reference's serial differs (or the handle's is -1).
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkWordPair {
    u64 field_0x0;
    u64 field_0x8;
};

// The dialog: the voice it plays, a word pair it was opened with, its text, the music level it
// lowered (negative until read) and its two timings.
struct UnkAdamDialog {
    u8 unk_0x0[0x8];
    UnkSoundHandle voice;      // +0x8
    UnkWordPair field_0x18;    // +0x18
    CRntString field_0x28;     // +0x28
    u8 unk_0x58[0x30];
    u32 field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    f32 field_0x94;
    bool field_0x98;
};
static_assert(__builtin_offsetof(UnkAdamDialog, field_0x98) == 0x98, "UnkAdamDialog placeholder layout");

// The id table (fn_7100031894 builds it; the same definition as src/game/CGameManager.cpp's and
// src/game/CSubAreaManager.cpp's). Its +0x2050 names the blackboard section the dialog mode is
// read from.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// A property seen as a definition whose qualified id (game/UnkGameBlackboard.h) is built on first
// use (as in src/game/CGameManager.cpp).
struct UnkDialogDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
static inline const UnkQualifiedId& Qid(UnkDialogDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}
extern UnkPropertyEx<> g_unk_7172759b50;  // the dialog mode's property

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);

// The language singleton (created on first use; 0x2068 bytes) and a localized text it gives for a
// key: a 0x20-byte text destroyed by fn_7100066e44.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
void fn_7100066e44(void* text);
struct UnkDialogText {
    u8 unk_0x0[0x20];
    ~UnkDialogText() { fn_7100066e44(this); }
};
UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key);

// A callback: an object and the function called with it (an empty one is two nulls).
struct UnkDialogCallback {
    void* object;
    void (*fn)(void*);
};
void fn_7100fb9564(void* object);

// The dialog target behind the game manager's +0x2260 (its +0x10) and what opens a page on it.
struct UnkDialogHolder {
    u8 unk_0x0[0x10];
    void* target;
};
void fn_7100b01e60(void* target, const UnkDialogText* text, void* arg2, const CStrId& arg3,
                   const UnkDialogCallback* on_close, const CStrId* id, const UnkDialogCallback* on_open);

// Opens a page with the text `key` names on the dialog target, when the blackboard's dialog mode
// (2 unless set) is 2; closing it calls fn_7100fb9564 with `owner`.
void fn_7100fb89b4(UnkAdamDialog* owner, const CStrId& key, void* arg2, const CStrId& arg3) {
    s32 mode = 2;
    CBlackboard* blackboard = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x820);
    const CStrId& section = *reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + 0x2050);
    const UnkQualifiedId& name = Qid(reinterpret_cast<UnkDialogDef&>(g_unk_7172759b50));
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395a30(s, &name, &mode);
    UnkDialogHolder* holder =
        *reinterpret_cast<UnkDialogHolder**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2260);
    if (!holder)
        return;
    void* target = holder->target;
    if (!target || mode != 2)
        return;
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    UnkDialogText text = fn_710119d040(language, key);
    UnkDialogCallback on_close;
    on_close.object = owner;
    on_close.fn = fn_7100fb9564;
    UnkDialogCallback on_open;
    on_open.object = nullptr;
    on_open.fn = nullptr;
    fn_7100b01e60(target, &text, arg2, arg3, &on_close, &g_unk_7101d470e0, &on_open);
}

// The pooled data behind a string id as its text accessor sees it: the text at +0x10 and a count
// of the reads at +0x44 (a union member: the original reloads the id after the increment).
struct UnkIdText {
    u8 unk_0x0[0x10];
    const char* str;
    u8 unk_0x18[0x2c];
    union {
        s32 reads;
    };
};
static inline const char* IdText(const CStrId& id) {
    ++reinterpret_cast<UnkIdText*>(id.data)->reads;
    const char* s = reinterpret_cast<UnkIdText*>(id.data)->str;
    return s ? s : g_unk_710162e2ed;
}

// The sound player (reached through the GOT) and the voice calls.
extern void* g_unk_71729efa48;
UnkSoundHandle fn_7100fcdf00(void* player, f32 volume, const CStrId& name, bool arg3);
void fn_7100105034(void* sound);
void fn_7100105ef4(void* sound, f32 time, f32 step, bool arg3);

// The music manager (game/sound/CMusicManager.h), created on first use, and its level.
extern void* g_unk_71726bc560;
f32 fn_7100fd7b3c(void* manager);
void fn_7100fd7af8(void* manager, f32 level, f32 time);
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

// Starts the dialog `key` names: with `stop`, a voice still playing is faded out and stopped; the
// voice `voice` (or the key's text with ".wav", lower-cased) is played at `volume` and faded in
// over `fade` when that is positive; with `lower` the music is brought down to at most 0.4 (its
// level is read once), else back to the level read before; then the page opens with `text`.
// False when the key is empty or the voice did not start.
bool fn_7100fb85cc(UnkAdamDialog* self, f32 volume, f32 fade, const CStrId& key, void* arg2, const char* voice,
                   bool stop, bool lower, const char* text, const UnkWordPair& pair) {
    if (key.data == &g_unk_7101d47108->empty)
        return false;
    if (stop) {
        if (UnkSound* sound = self->voice.Get()) {
            fn_7100105a80(sound, 0.2f, 0.0f);
            fn_7100105034(self->voice.Get());
            self->voice = g_unk_7108b6c460;
        }
    }
    char name[0x200];
    if (voice && voice[0]) {
        memcpy(name, voice, strlen(voice) + 1);
    } else {
        sprintf(name, "%s%s", IdText(key), ".wav");
        s32 length = strlen(name);
        for (s32 i = 0; i < length; ++i)
            if (name[i] >= 'A' && name[i] <= 'Z')
                name[i] += 'a' - 'A';
    }
    {
        void* player = g_unk_71729efa48;
        CStrId id(name, true);
        self->voice = fn_7100fcdf00(player, volume, id, true);
    }
    UnkSound* sound = self->voice.Get();
    if (!sound)
        return false;
    if (fade > 0.0f)
        fn_7100105ef4(sound, fade, 0.2f, true);
    if (lower) {
        if (self->field_0x8c < 0.0f)
            self->field_0x8c = fn_7100fd7b3c(MusicManager());
        if (self->field_0x8c > 0.4f)
            fn_7100fd7af8(MusicManager(), 0.4f, 0.2f);
    } else if (self->field_0x8c >= 0.0f) {
        fn_7100fd7af8(MusicManager(), self->field_0x8c, 0.2f);
    }
    {
        CStrId id(text, true);
        fn_7100fb89b4(self, key, arg2, id);
    }
    self->field_0x98 = lower;
    self->field_0x88 = 0;
    fn_710007e77c(&self->field_0x28, text);
    self->field_0x18 = pair;
    self->field_0x90 = volume;
    self->field_0x94 = fade;
    return true;
}
