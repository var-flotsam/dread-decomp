// CSaveStationUsableComponent's unit (tools/tu_map.py). fn_7100a8a068: stops the sound at +0x540 (as
// CShipRechargeComponent's fn_7100a8bf18, with a 2.5 fade), stops the char class's effect on the
// entity's world and, with a user still alive (+0xe8), plays two timeline ids on it. fn_7100a89bf8: keeps
// a weak handle to the user and asks the save-station question (as CMapAcquisitionComponent does) with
// the callbacks fn_7100a8ad50 and fn_7100a8ad5c. fn_7100a8a438: counts the time (+0x554); unless the
// music manager is still playing the char class's music (fn_7100fd6f74) and the tunable wait is set and
// not yet over, has the object g_unk_7101d47368 run fn_7100d8e0a0, plays the user's timeline id and
// calls its own slot 0x2b8. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkSaveStation_ovb0118;
void fn_7100a8ad50(UnkSaveStation_ovb0118* self);
void fn_7100a8ad5c(UnkSaveStation_ovb0118* self);

// A callback: an object and a function to call on it.
struct UnkSaveCallback_ovb0118 {
    UnkSaveStation_ovb0118* object;
    void (*function)(UnkSaveStation_ovb0118*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

struct UnkGuiRef_ovb0118 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

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
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkShipRechargeEntity_b49a4;

struct UnkWorld;
void fn_710114fe8c(UnkWorld*, u64, u32, u32);
void fn_7100d7c94c(UnkActor*, const u64*);

struct UnkSaveStationEffect_ovb0118 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkSaveStationClass_ovb0118 {
    u8 pad_0x0[0x1b8];
    UnkSaveStationEffect_ovb0118* effect;  // 0x1b8
};

struct UnkSaveStationEntity_ovb0118 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkSaveStation_ovb0118 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x18];
    UnkSaveStationEntity_ovb0118* entity;  // 0x30
    u8 pad_0x38[0xb0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x450];
    UnkSoundHandle sound;  // 0x540
};

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
extern const u64 g_unk_710164b5c0;
extern const u64 g_unk_710164b5c8;

void fn_7100a8a068(UnkSaveStation_ovb0118* self) {
    if (self->sound.Get()) {
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->entity), &self->sound, 2.5f, 0.0f);
        self->sound = g_unk_7108b6c460;
    }
    UnkWorld* world = self->entity->world;
    base::reflection::UnkReflObject* object = self->char_class;
    UnkSaveStationClass_ovb0118* char_class =
        fn_710009d50c(object->vf_0x0(), fn_7100a8886c()) ? reinterpret_cast<UnkSaveStationClass_ovb0118*>(object)
                                                        : nullptr;
    fn_710114fe8c(world, char_class->effect->id, 0, 0);
    if (!self->user || !self->user->target)
        return;
    fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5c0);
    UnkWeakTarget* target = self->user ? self->user->target : nullptr;
    fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &g_unk_710164b5c8, false);
}

bool fn_7100a89bf8(UnkSaveStation_ovb0118* self, UnkWeakTarget* user) {
    if (!user)
        return false;
    AssignRef(&self->user, fn_7100d78378(user));
    UnkGuiRef_ovb0118* ref = static_cast<UnkGuiRef_ovb0118*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return true;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return true;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text("#GUI_SAVESTATION_QUESTION", true);
    UnkSaveCallback_ovb0118 accept_callback = {self, fn_7100a8ad50};
    UnkSaveCallback_ovb0118 back_callback = {self, fn_7100a8ad5c};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
    return true;
}

extern void* g_unk_71726bc560;  // the music manager
extern void* g_unk_7101d47368;

struct UnkSaveStationName_ovb0118 {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct UnkSaveStationClass2_ovb0118 {
    u8 pad_0x0[0x190];
    UnkSaveStationName_ovb0118* music;  // 0x190
};

// The save station's tunables: the time to wait for the music at +0x24.
struct UnkSaveStationTunables_ovb0118 {
    u8 pad_0x0[0x24];
    f32 music_wait;  // 0x24
};
extern UnkSaveStationTunables_ovb0118 g_unk_71727531f0;

struct UnkSaveStation2_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x8];
    base::reflection::UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0xd0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x464];
    f32 time;  // 0x554

    void vf_0x2b8(bool flag) {
        reinterpret_cast<void (*)(UnkSaveStation2_ovb0118*, bool)>(vtable[0x2b8 / 8])(this, flag);
    }
};

typedef u32 __attribute__((may_alias)) UnkAliasU32_ovb0118;

bool fn_7100fd6f74(void* music, const char* name, const u32* mask);

inline const u32& AllMusicLayers_ovb0118() {
    static u32 s_value = ~base::reflection::refl_GetEnumZero<0x71726c4090>() & 0x1ff;
    return s_value;
}

static inline void* MusicManager() {
    void* music = g_unk_71726bc560;
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        g_unk_71726bc560 = music;
    }
    return music;
}

void fn_7100a8a438(UnkSaveStation2_ovb0118* self, f32 dt) {
    self->time += dt;
    base::reflection::UnkReflObject* object = self->char_class;
    UnkSaveStationClass2_ovb0118* char_class =
        object && fn_710009d50c(object->vf_0x0(), fn_7100a8886c()) ? reinterpret_cast<UnkSaveStationClass2_ovb0118*>(object)
                                                                  : nullptr;
    void* music = MusicManager();
    // Counted through an aliasing type: the original reloads the name after it.
    ++*reinterpret_cast<UnkAliasU32_ovb0118*>(&char_class->music->use_count);
    const char* name = char_class->music->text;
    alignas(8) u32 mask = AllMusicLayers_ovb0118();
    if (fn_7100fd6f74(music, name, &mask)) {
        f32 wait = g_unk_71727531f0.music_wait;
        if (wait + 0.001f >= 0.0f && wait - 0.001f <= 0.0f)
            return;
        if (!(self->time >= wait))
            return;
    }
    void* lua = g_unk_7101d47368;
    if (!lua) {
        lua = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(lua);
        g_unk_7101d47368 = lua;
    }
    fn_7100d8e0a0(lua);
    if (self->user && self->user->target) {
        fn_7100d7c94c(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5c0);
        UnkWeakTarget* target = self->user ? self->user->target : nullptr;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &g_unk_710164b5c8, false);
    }
    self->vf_0x2b8(false);
}
