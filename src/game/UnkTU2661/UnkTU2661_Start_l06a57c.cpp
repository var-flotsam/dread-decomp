// CCutscene::CTunableCutscene unit (tools/tu_map.py unit 2661): fn_710106a57c starts a cutscene: it registers
// it with the cutscene director, runs its script function when the game allows it, steps it to its first frame,
// stops its sound and clears the two fades the game's player keeps for the director's current cutscene.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU2677.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkObj_710119770c;
struct UnkActor_710119770c;
struct UnkCutscene;

void fn_710119770c(UnkObj_710119770c* self, UnkActor_710119770c* actor);
void fn_7101197d1c(UnkObj_710119770c* self);
u64 fn_71010e4a98(void* self);
u64 fn_71010e4aa0(void* self);
void fn_71010e4aa8(void* self);
void fn_71010e4ab0(void* self);
void fn_71010e1b40(void* self);
void fn_71010e1b78(void* self);

struct UnkCutsceneEntry_l06a57c {
    u8 pad_0x0[0x18];
    CStrId id;
};

// The cutscene director (g_unk_7101d47330).
struct UnkCutsceneDirector_l06a57c {
    u8 pad_0x0[0x58];
    UnkCutsceneEntry_l06a57c* current;
    u8 pad_0x60[0xb14 - 0x60];
    bool field_0xb14;

    bool IsCurrent(const CStrId& id) const { return current && current->id == id; }
};

struct UnkCutsceneDef_l06a57c {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
};

struct UnkSoundRef_l06a57c {
    UnkSound* sound;
    s64 serial;
};

struct UnkCutscene_l06a57c {
    u8 pad_0x0[0x50];
    CStrId script;
    u8 pad_0x58[0x118 - 0x58];
    f32 field_0x118;
    u8 pad_0x11c[0x2f8 - 0x11c];
    UnkCutsceneDef_l06a57c* def;
    UnkSoundHandle sound;
    u8 pad_0x310[0x4b0 - 0x310];
    s32 state;
    u32 field_0x4b4;
    u8 pad_0x4b8[4];
    f32 frame;
    u8 pad_0x4c0[0x523 - 0x4c0];
    bool field_0x523;
    bool field_0x524;
};
static_assert(__builtin_offsetof(UnkCutscene_l06a57c, def) == 0x2f8, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l06a57c, state) == 0x4b0, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l06a57c, field_0x524) == 0x524, "layout");

// The id's text, counting the read.
static inline const char* ReadText(const CStrId& id) {
    ++id.data->reads;
    return id.data->text;
}

static inline UnkCutsceneDirector_l06a57c* Director() {
    return static_cast<UnkCutsceneDirector_l06a57c*>(g_unk_7101d47330);
}

void fn_710106a57c(UnkCutscene_l06a57c* self) {
    if (self->state != 6)
        self->state = 4;
    fn_710119770c(static_cast<UnkObj_710119770c*>(g_unk_7101d47330), reinterpret_cast<UnkActor_710119770c*>(self));
    if (self->script.data != &g_unk_7101d47108->empty) {
        CStrId script = self->script;
        if (!self->field_0x523 && !fn_7100deac04(g_unk_7101d47310) && !fn_7100deabe4(g_unk_7101d47310))
            fn_71010c0e70(ReadText(script), g_unk_7101d58d10.data, 0);
    }
    if (self->def->field_0x1f8 || Director()->field_0xb14)
        self->field_0x524 = false;
    CStrId current = Director()->current->id;
    if (!self->field_0x4b4 || self->state == 6) {
        fn_710106838c(self);
        self->frame = self->field_0x118 + self->frame;
        fn_710106b2bc(reinterpret_cast<UnkCutscene*>(self), self->frame);
    }
    if (self->field_0x524)
        fn_7101197d1c(static_cast<UnkObj_710119770c*>(g_unk_7101d47330));
    UnkSoundRef_l06a57c* ref = reinterpret_cast<UnkSoundRef_l06a57c*>(self->sound.ref);
    if (ref && ref->serial == self->sound.serial && self->sound.serial != -1 && ref->sound) {
        fn_7100105a80(ref->sound, 0.0f, 0.0f);
        self->sound = g_unk_7108b6c460;
    }
    if (void* player = fn_7100defcec(g_unk_7101d47310)) {
        if (fn_71010e4a98(player) >= 2 && Director()->IsCurrent(current)) {
            fn_71010e4aa8(player);
            fn_71010e1b40(player);
        }
        if (fn_71010e4aa0(player) >= 2 && Director()->IsCurrent(current)) {
            fn_71010e4ab0(player);
            fn_71010e1b78(player);
        }
    }
}
