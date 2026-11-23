// Unit 2666 Lua binding: plays a music stream by path ("s"). The sound system's group 1 fades over
// 0.2 s (fn_7100108b0c), then a sound description (group 1) takes the path, the music manager's
// volume for it (fn_7100fdc468) and its two words (fn_7100fdc3c4), and goes to the sound system
// (fn_7100108620). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBossAIComponent.h"
#include "game/UnkPlayingHandle_ovb0074.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2677.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringEntry;

struct lua_State;

using UnkPathMember_l092720 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkTuneMgrView_b0019;
void fn_7100fdc3c4(UnkTuneMgrView_b0019* manager, const u64* path, s32* first, s32* second);

// The file path of a string id's text (its use count at +0x44 goes up first).
struct UnkStreamPath_l092720 {
    base::global::UnkStringData* data;

    explicit UnkStreamPath_l092720(const CStrId& id) {
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
        ++entry->tail.field_0x44;
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), entry->buffer, true);
    }
    ~UnkStreamPath_l092720() { reinterpret_cast<UnkPathMember_l092720*>(this)->~UnkPathMember_l092720(); }
    void operator=(const CFilePathStrId& other) { *reinterpret_cast<CFilePathStrId*>(this) = other; }
};

// A string id copied from another (retained) and released in two steps.
struct UnkStreamName_l092720 {
    base::global::UnkStringData* data;

    explicit UnkStreamName_l092720(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkStreamName_l092720() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// What the sound system plays (the shape of UnkSoundDesc_ovb0074).
struct UnkMusicDesc_l092720 {
    s32 group;
    UnkStreamPath_l092720 path;  // 0x8
    UnkStreamName_l092720 name;  // 0x10
    s32 direction;               // 0x18
    s32 field_0x1c;
    s32 field_0x20;
    u32 variant;                 // 0x24
    u32 field_0x28;              // unset in the original
    f32 volume;                  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkMusicDesc_l092720()
        : group(0), path(base::global::CStrId::Empty()), name(base::global::CStrId::Empty()),
          direction(1), field_0x1c(0), field_0x20(-1), variant(0), volume(1.0f), field_0x30(0.0f),
          field_0x34(0.0f), field_0x38(-1.0f) {}
};

struct UnkSoundSystemPlayView_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkPlayingRef_ovb0074;

// The music manager, created on first use.
static inline UnkTuneMgrView_b0019* MusicManager_l092720() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return static_cast<UnkTuneMgrView_b0019*>(manager);
}

s32 fn_7101092720(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        CFilePathStrId path(name, true);
        fn_7100108b0c(g_unk_7101d4d820, 1, 0.2f);
        UnkMusicDesc_l092720 desc;
        desc.volume = fn_7100fdc468(MusicManager_l092720(), reinterpret_cast<const u64*>(&path));
        fn_7100fdc3c4(MusicManager_l092720(), reinterpret_cast<const u64*>(&path), &desc.field_0x1c, &desc.field_0x20);
        desc.direction = 1;
        desc.path = path;
        desc.group = 1;
        fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                      reinterpret_cast<UnkSoundDesc_ovb0074*>(&desc));
    }
    return 0;
}
