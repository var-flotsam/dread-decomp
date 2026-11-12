// CAreaSoundComponent's unit (tools/tu_map.py): the three sound paths at +0xb08..+0xb18, handed to
// the sound manager after the base step (fn_7100a46e00) and back from it before the base
// teardown (fn_710024f910), as CPositionalSoundComponent does with its four
// (src/game/CPositionalSoundComponent/CPositionalSoundComponent_Sounds_ovb0056.cpp). Names and
// layouts are placeholders. The component also forgets a finished sound: the first of its three
// sound handles (+0xad8..+0xaf8) that still refers to it is emptied (fn_71011815a8, and
// fn_7101181664 for the listener base at +0xad0).
#include "base/global/CFilePathStrId.h"
#include "base/snd/CSoundSystem.h"
#include "base/snd/UnkSound.h"
#include "game/CActorComponent.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringPool;

struct UnkSoundMgr;
struct UnkPathNode;
struct UnkLightingComponent3;
struct UnkSteamJetComponent;
struct UnkSteamJetArg;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkAreaSoundComponent_ovb0062 {
    u8 unk_0x0[0xb08];
    CFilePathStrId paths[3];  // 0xb08
};

bool fn_71011806b4(UnkAreaSoundComponent_ovb0062* self, UnkSteamJetArg* arg) {
    bool result = fn_7100a46e00(reinterpret_cast<UnkSteamJetComponent*>(self), arg);
    for (int i = 0; i < 3; ++i) {
        if (self->paths[i].data != &g_unk_7101d47200->empty)
            fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(&self->paths[i]));
    }
    return result;
}

void fn_7101180754(UnkAreaSoundComponent_ovb0062* self) {
    for (int i = 0; i < 3; ++i) {
        if (self->paths[i].data != &g_unk_7101d47200->empty)
            fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(&self->paths[i]));
    }
    fn_710024f910(reinterpret_cast<UnkLightingComponent3*>(self));
}

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};

static inline UnkSound* GetSound_ovb0062(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkAreaSoundHandles_ovb0062 {
    u8 unk_0x0[0xad8];
    UnkSoundHandle handles[3];  // 0xad8
};

__attribute__((always_inline)) static inline void Forget_ovb0062(UnkAreaSoundHandles_ovb0062* self, UnkSound* sound) {
    if (!sound)
        return;
    for (int i = 0; i < 3; ++i) {
        UnkSound* s = GetSound_ovb0062(&self->handles[i]);
        if (s && s == sound) {
            self->handles[i] = g_unk_7108b6c460;
            return;
        }
    }
}

void fn_71011815a8(UnkAreaSoundHandles_ovb0062* self, UnkSound* sound) {
    Forget_ovb0062(self, sound);
}

// Through the listener base at +0xad0.
void fn_7101181664(u8* listener, UnkSound* sound) {
    Forget_ovb0062(reinterpret_cast<UnkAreaSoundHandles_ovb0062*>(reinterpret_cast<u64>(listener) - 0xad0), sound);
}

struct UnkSoundObject_b50;
void fn_71007181b8(void* self);
extern char g_unk_7101cb48a0[];  // CAreaSoundComponent's vtables

struct UnkAreaSoundDtor_ovb0062 {
    void* vtable;
    u8 unk_0x8[0xac8];
    void* listener_vtable;      // 0xad0
    UnkSoundHandle handles[3];  // 0xad8
    CFilePathStrId paths[3];    // 0xb08
};

static inline void Release_ovb0062(CFilePathStrId* path) {
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (path->data && path->data != &pool->empty) {
            --path->data->ref_count;
            if (path->data->ref_count == 0) {
                fn_71000803a4(pool, path->data);
                pool = g_unk_7101d47200;
            }
        }
        path->data = &pool->empty;
    }
}

// The component's destructor body: the vtables (the primary and the listener's at +0xad0), the
// first handle with a sound is stopped (fn_7100106024) and emptied (for the third handle the
// second handle's sound is the one passed, as in the original), the paths are released in
// reverse, then the base destructor (fn_71007181b8) runs.
void fn_71011804b0(UnkAreaSoundDtor_ovb0062* self) {
    self->vtable = g_unk_7101cb48a0 + 0x10;
    self->listener_vtable = g_unk_7101cb48a0 + 0x220;
    UnkSoundHandle* handle = nullptr;
    UnkSound* sound = nullptr;
    if (GetSound_ovb0062(&self->handles[0])) {
        handle = &self->handles[0];
        sound = GetSound_ovb0062(&self->handles[0]);
    } else if (GetSound_ovb0062(&self->handles[1])) {
        handle = &self->handles[1];
        sound = GetSound_ovb0062(&self->handles[1]);
    } else if (GetSound_ovb0062(&self->handles[2])) {
        handle = &self->handles[2];
        sound = GetSound_ovb0062(&self->handles[1]);
    }
    if (handle) {
        fn_7100106024(reinterpret_cast<UnkSoundObject_b50*>(sound), nullptr);
        *handle = g_unk_7108b6c460;
    }
    Release_ovb0062(&self->paths[2]);
    Release_ovb0062(&self->paths[1]);
    Release_ovb0062(&self->paths[0]);
    fn_71007181b8(self);
}
