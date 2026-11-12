// CUnlockAreaSmartObjectComponent's unit (tools/tu_map.py): whether the owner's animation is idle
// (fn_7100a07a90), the grab request handed to the player (fn_7100a076b0) and the use request
// (fn_7100a0746c), which either runs the component's own use (vf_0x1a8) or shows the tunable's text.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CGrabComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2733.h"
#include "types.h"

struct UnkIdHolder_b0086ua {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b0086ua {
    UnkIdTable_b0086ua();  // fn_7100031894
    ~UnkIdTable_b0086ua();
    u8 pad_0x0[0x1d78];
    UnkIdHolder_b0086ua* field_0x1d78;
    u8 pad_0x1d80[0x2438 - 0x1d80];
    struct UnkIdData_b0086ua* field_0x2438;
};

inline UnkIdTable_b0086ua& IdTable_b0086ua() {
    static UnkIdTable_b0086ua s_table;
    return s_table;
}

struct UnkUnlockAreaOwner_b0086 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkUnlockAreaSmartObject_b0086 {
    u8 pad_0x0[0x30];
    UnkUnlockAreaOwner_b0086* owner;
};

// Whether the owner's animation is not playing the id table's +0x1d78 track (false without one).
bool fn_7100a07a90(UnkUnlockAreaSmartObject_b0086* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    return !fn_710117ae38(animation, IdTable_b0086ua().field_0x1d78->id, false);
}

using base::global::CStrId;

struct UnkPlayer;
struct UnkKraidGrab_b57a4;

struct UnkUnlockAreaPlayer_b0086 {
    u8 pad_0x0[0x260];
    void* grabber;  // 0x260
};

// The grab request handed to the player's +0x260 (fn_710113d940): a name and a word.
struct UnkUnlockAreaGrab_b0086 {
    u8 pad_0x0[0x10];
    CStrId name;  // 0xd0 of the component
    u8 pad_0x18[0xc];
    u32 field_0x24;
    u8 pad_0x28[3];
    bool field_0x2b;
};

struct UnkUnlockAreaSmartObject2_b0086 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[8];
    u32 field_0x40;
    u8 pad_0x44[0x24];
    CStrId field_0x68;
    u8 pad_0x70[0x50];
    UnkUnlockAreaGrab_b0086 grab;  // 0xc0
    u8 pad_0xf0[0x58];
    CStrId field_0x148;
};
static_assert(__builtin_offsetof(UnkUnlockAreaSmartObject2_b0086, field_0x148) == 0x148, "layout");

void fn_7100a076b0(UnkUnlockAreaSmartObject2_b0086* self) {
    auto* player = reinterpret_cast<UnkUnlockAreaPlayer_b0086*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->grabber) {
        if (self->field_0x148.data != &g_unk_7101d47108->empty)
            self->grab.name = self->field_0x148;
        else if (self->field_0x68.data != &g_unk_7101d47108->empty)
            self->grab.name = self->field_0x68;
        else
            return;
        self->grab.field_0x24 = self->field_0x40;
        self->grab.field_0x2b = false;
        fn_710113d940(player->grabber, reinterpret_cast<UnkKraidGrab_b57a4*>(&self->grab), self->owner);
    }
    self->field_0x148 = g_unk_7101d470e0;
}

// The pooled data behind a string id as its hash accessor sees it: the hash at +0x30 and a count of
// the reads at +0x44 (a union member: the original reloads the id after the increment).
struct UnkIdData_b0086ua {
    u8 pad_0x0[0x30];
    u64 hash;
    u8 pad_0x38[0xc];
    union {
        u32 use_count;
    };
};

struct UnkInputComponent;

// The language singleton (as src/game/UnkAdamDialog.cpp declares it): created on first use, it
// gives localized texts.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
struct UnkDialogText {
    u8 unk_0x0[0x20];
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

// As src/reflection/CUnlockAreaSmartObjectComponent.cpp declares it (the unit's tunable).
extern base::reflection::UnkTunable g_unk_7172743078;
struct UnkUnlockAreaTunable_b0086 {
    u8 pad_0x0[0x28];
    base::global::CRntString text_key;  // 0x28
};

// A string id built from a run-time string (fn_7100000850).
struct UnkBuiltStrId_b0086 {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_b0086(const base::global::CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltStrId_b0086() { id.~CStrId(); }
};

struct UnkUnlockAreaInputPlayer_b0086 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

struct UnkUnlockAreaOwner3_b0086 {
    u8 pad_0x0[0x1e0];
    void* field_0x1e0;
    u8 pad_0x1e8[0x38];
    void* field_0x220;
};

struct UnkUnlockAreaSmartObject3_b0086 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkUnlockAreaOwner3_b0086* owner;  // 0x30
    u8 pad_0x38[0x110];
    union {
        CStrId field_0x148;
        UnkIdData_b0086ua* field_0x148_data;
    };
    bool field_0x150;

    bool vf_0x188(bool flag) {
        return reinterpret_cast<bool (*)(UnkUnlockAreaSmartObject3_b0086*, bool)>(vtable[0x188 / 8])(this, flag);
    }
    void vf_0x1a8() { reinterpret_cast<void (*)(UnkUnlockAreaSmartObject3_b0086*)>(vtable[0x1a8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkUnlockAreaSmartObject3_b0086, field_0x150) == 0x150, "layout");

void fn_7100a0746c(UnkUnlockAreaSmartObject3_b0086* self, const CStrId* name) {
    auto* player = reinterpret_cast<UnkUnlockAreaInputPlayer_b0086*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkInputComponent* input = player->input;
    if (!input || !self->owner->field_0x220 || self->vf_0x188(false) || self->field_0x150 ||
        fn_710087238c(input))
        return;
    if (name->data != &g_unk_7101d47108->empty) {
        self->field_0x148 = *name;
        UnkIdTable_b0086ua& table = IdTable_b0086ua();
        ++table.field_0x2438->use_count;
        UnkIdData_b0086ua* entry = table.field_0x2438;
        ++self->field_0x148_data->use_count;
        if (self->field_0x148_data->hash == entry->hash) {
            self->vf_0x1a8();
            return;
        }
    }
    if (!self->owner->field_0x1e0)
        return;
    self->field_0x150 = true;
    UnkLanguage* language = Language();
    UnkDialogText text = fn_710119d040(
        language, UnkBuiltStrId_b0086(reinterpret_cast<UnkUnlockAreaTunable_b0086*>(&g_unk_7172743078)->text_key).id);
}
