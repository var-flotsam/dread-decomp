// CPickableItemComponent's unit: the pickup jingle. Sound group 5 and the groups in mask 0x20 are switched
// on, the music manager plays the powerup, pickup or unknown pickup jingle and the matching rumble file is
// played. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;
using base::math::CVector3D;

void fn_7100108b34(UnkSoundMgr* self, u32 group, bool on, f32 fade);

struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};

// What the music manager calls when the jingle ends.
__attribute__((visibility("hidden"))) void fn_71009d3f54();

// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget_b0086 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_b0086* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

// A file-path id (pool g_unk_7101d47200).
struct UnkPickablePathId_l9d16c4 {
    UnkStringData* data;

    explicit UnkPickablePathId_l9d16c4(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkPickablePathId_l9d16c4& operator=(const base::global::UnkFilePathStrIdBase& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkPickablePathId_l9d16c4() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from.
struct UnkPickableRumbleParams_l9d16c4 {
    UnkPickablePathId_l9d16c4 path;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkPickableRumbleParams_l9d16c4()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};
static_assert(sizeof(UnkPickableRumbleParams_l9d16c4) == 0x48, "");

struct UnkPickableItem_l9d16c4 {
    u8 pad_0x0[0x128];
    bool powerup;  // 0x128
};

// The music manager, created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

// The rumble manager, created on first use.
static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

void fn_71009d16c4(UnkPickableItem_l9d16c4* self, bool known) {
    static CRntString s_powerupJingle("powerup_jingle");
    static CRntString s_pickupJingle("pickup_jingle");
    static CRntString s_unknownJingle("unknown_pickup_jingle");
    fn_7100108b34(g_unk_7101d4d820, 5, true, 0.2f);
    alignas(8) u32 groups = 0x20;
    fn_7100108464(reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820), true, &groups);
    void* music = MusicManager();
    const CRntString* jingle = self->powerup ? &s_powerupJingle : known ? &s_pickupJingle : &s_unknownJingle;
    UnkMusicMode mode;
    mode.kind = 0x62;
    fn_7100fd90d8(-1.0f, -1.0f, music, jingle, 4, &mode, 0, 0, reinterpret_cast<void*>(fn_71009d3f54));

    static CFilePathStrId s_powerupRumble("streams/music/s_jingle_powerupget.bnvib", true);
    static CFilePathStrId s_pickupRumble("streams/music/s_jingle_itemget.bnvib", true);
    static CFilePathStrId s_unknownRumble("streams/music/s_jingle_itemget_unknown.bnvib", true);
    UnkPickableRumbleParams_l9d16c4 params;
    params.path = self->powerup ? s_powerupRumble : known ? s_pickupRumble : s_unknownRumble;
    params.field_0x10 = 4;
    params.field_0x20[2] = 1.0f;
    params.field_0x34 = 0.1f;
    fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
}
