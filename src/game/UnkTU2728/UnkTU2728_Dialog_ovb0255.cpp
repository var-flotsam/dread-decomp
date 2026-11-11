// Unit 2728: starts a voice for a caption id and, for the languages whose setting asks for it, shows its
// text on the dialog target (fn_7101192664: a localized text or a clear, by the blackboard dialog mode).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CBlackboard.h"
#include "game/GUI/CSubtitleComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkTU2615.h"
#include "game/UnkTU2733.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

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

struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
struct UnkDialogText : UnkGuiText {
    ~UnkDialogText() { fn_7100066e44(this); }
};
UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key);

static inline UnkLanguage* Language() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

using UnkGuiObj = UnkGuiDisplayObject;
bool fn_7100b8524c(UnkGuiObj* self, const UnkGuiText& text);
struct UnkUnit2728Target;

struct UnkUnit2728Holder {
    u8 pad_0x0[0x10];
    UnkUnit2728Target* target;  // 0x10
};
struct UnkUnit2728 {
    u8 pad_0x0[0x38];
    bool flag;  // 0x38
};

__attribute__((noinline)) void fn_7101192664(UnkUnit2728* self, const CStrId* key) {
    s32 mode = 2;
    CBlackboard* blackboard = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x820);
    const CStrId& section = *reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + 0x2050);
    const UnkQualifiedId& name = Qid(reinterpret_cast<UnkDialogDef&>(g_unk_7172759b50));
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395a30(s, &name, &mode);
    bool flag = mode == 2 ? self->flag : false;
    UnkUnit2728Holder* holder = *reinterpret_cast<UnkUnit2728Holder**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2278);
    if (!holder)
        return;
    UnkUnit2728Target* target = holder->target;
    if (!target)
        return;
    if (flag) {
        UnkDialogText text = fn_710119d040(Language(), *key);
        fn_7100b8524c(reinterpret_cast<UnkGuiObj*>(target), text);
    } else {
        fn_7100b8538c(target);
    }
}


struct UnkSound;
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

UnkSoundHandle fn_7100fcdf00(void* player, f32 volume, const CStrId& name, bool arg3);


struct UnkVoice_b0255 {
    UnkSoundHandle handle;  // 0x0
    u64 field_0x10;
    u64 field_0x18;
    u8 unk_0x20[8];
    u64 field_0x28;
    u32 field_0x30;
};

struct UnkCaptionId;

bool fn_7101192838(void* player, UnkCaptionId* caption, bool on_text, bool on_alt, bool loop, f32 volume) {
    auto* self = static_cast<UnkVoice_b0255*>(player);
    const CStrId& key = *reinterpret_cast<const CStrId*>(caption);
    if (key.data == &g_unk_7101d47108->empty)
        return false;
    self->handle = fn_7100fcdf00(g_unk_71729efa48, volume, key, loop);
    if (!self->handle.Get())
        return false;
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    switch (fn_710119c9f4(language)) {
    case 10:
    case 11:
    case 12:
    case 13:
        if (on_alt)
            fn_7101192664(reinterpret_cast<UnkUnit2728*>(self), &key);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 14:
        if (on_text)
            fn_7101192664(reinterpret_cast<UnkUnit2728*>(self), &key);
        break;
    case 0:
        if (on_text)
            fn_7101192664(reinterpret_cast<UnkUnit2728*>(self), &key);
        break;
    default:
        break;
    }
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    return true;
}
