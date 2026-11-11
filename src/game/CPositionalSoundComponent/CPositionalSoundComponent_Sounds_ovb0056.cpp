// CPositionalSoundComponent unit (tools/tu_map.py, the name from reflection): the four sound paths at
// +0x50..+0x68 (handed to and back from the sound manager, or one picked at random) and the two sound
// handles at +0x88/+0x98 (stopped or forgotten). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/snd/CSoundSystem.h"
#include "base/snd/UnkSound.h"
#include "game/CActorComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU101.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkSoundMgr;
struct UnkPathNode;
struct UnkLightingComponent3;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

static inline UnkSound* GetSound_ovb56(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkPositionalSound_ovb56 {
    u8 unk_0x0[0x48];
    s32 path_count;  // 0x48
    u8 unk_0x4c[4];
    CFilePathStrId paths[4];  // 0x50
    u8 unk_0x70[0x18];
    UnkSoundHandle handles[2];  // 0x88
    bool field_0xa8;
    u8 unk_0xa9[0x13];
    f32 field_0xbc;
};
static_assert(__builtin_offsetof(UnkPositionalSound_ovb56, field_0xbc) == 0xbc, "layout");

// The base update (fn_71010c41bc), then every non-empty path is handed to the sound manager.
bool fn_710114bbd4(UnkPositionalSound_ovb56* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    for (int i = 0; i < 4; ++i) {
        if (self->paths[i].data != &g_unk_7101d47200->empty)
            fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(&self->paths[i]));
    }
    return result;
}

// Hands every non-empty path back to the sound manager, then runs the base (fn_710024f910).
void fn_710114bc94(UnkPositionalSound_ovb56* self) {
    for (int i = 0; i < 4; ++i) {
        if (self->paths[i].data != &g_unk_7101d47200->empty)
            fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(&self->paths[i]));
    }
    fn_710024f910(reinterpret_cast<UnkLightingComponent3*>(self));
}

// Stops the sound of the active handle (the first with the flag at +0xa8 set, fading by +0xbc; the
// second otherwise) and empties that handle.
void fn_710114bd84(UnkPositionalSound_ovb56* self) {
    if (self->field_0xa8) {
        if (UnkSound* sound = GetSound_ovb56(&self->handles[0])) {
            fn_7100105a80(sound, self->field_0xbc, 0.0f);
            self->handles[0] = g_unk_7108b6c460;
        }
    } else {
        if (UnkSound* sound = GetSound_ovb56(&self->handles[1])) {
            fn_7100105034(sound);
            self->handles[1] = g_unk_7108b6c460;
        }
    }
}

// Empties the handle whose sound is `sound`.
static inline void Forget_ovb56(UnkPositionalSound_ovb56* self, UnkSound* sound) {
    if (!sound)
        return;
    for (int i = 0; i < 2; ++i) {
        UnkSound* current = GetSound_ovb56(&self->handles[i]);
        if (current && current == sound) {
            self->handles[i] = g_unk_7108b6c460;
            return;
        }
    }
}

void fn_710114cc50(UnkPositionalSound_ovb56* self, UnkSound* sound) {
    Forget_ovb56(self, sound);
}

extern "C" int rand();

// The id table (fn_7100031894 builds it; the definition of src/game/CScenario.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x5f8];
    CFilePathStrId field_0x5f8;
    u8 pad_0x600[0x2578 - 0x600];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// The string record behind a path id: its text at +0x10 and a use count at +0x44.
struct UnkPathRecord_ovb56 {
    u8 unk_0x0[0x10];
    char text[0x34];
    // A union member: Clang gives union accesses no type-based alias info, so the id's record is
    // read again after the count is raised.
    union {
        s32 uses;  // 0x44
    };
};

static inline const char* Use_ovb56(const CFilePathStrId* path) {
    ++reinterpret_cast<UnkPathRecord_ovb56*>(path->data)->uses;
    return reinterpret_cast<UnkPathRecord_ovb56*>(path->data)->text;
}

// A random one of the non-empty paths (the table's default path when there is none), counted as used.
const char* fn_710114be28(UnkPositionalSound_ovb56* self) {
    s32 found = 0;
    for (s32 i = 0; i < self->path_count; ++i) {
        if (self->paths[i].data != &g_unk_7101d47200->empty)
            ++found;
    }
    if (found) {
        s32 pick = rand() % found;
        s32 seen = 0;
        for (s32 i = 0; i < self->path_count; ++i) {
            if (self->paths[i].data == &g_unk_7101d47200->empty)
                continue;
            if (++seen >= pick)
                return Use_ovb56(&self->paths[pick]);
        }
    }
    return Use_ovb56(&GameManagerIdTable().field_0x5f8);
}
