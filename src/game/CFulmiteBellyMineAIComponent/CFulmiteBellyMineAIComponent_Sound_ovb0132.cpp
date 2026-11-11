// CFulmiteBellyMineAIComponent's unit (tools/tu_map.py unit 826): the mine's sound (fn_710057dadc).
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkInterp_7100105d34;
void fn_71001059f0(UnkInterp_7100105d34* self, f32 time);

struct UnkSoundHandle;
struct UnkEntityWithSound;

struct UnkStrIdPathRecord_b48;

// A string id released in one step (decrement and test together).
struct UnkMineSoundId_ovb0132 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkMineSoundId_ovb0132() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A file-path id released in two steps (the original reloads the id after the decrement).
struct UnkMineSoundPath_ovb0132 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkMineSoundPath_ovb0132() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMineSoundResource_ovb0132 {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) ~UnkMineSoundResource_ovb0132() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// A sound's name and path with the resource the path resolves to.
struct UnkMineSoundRecord_ovb0132 {
    UnkMineSoundId_ovb0132 id;
    UnkMineSoundPath_ovb0132 path;
    UnkMineSoundResource_ovb0132 resource;

    explicit UnkMineSoundRecord_ovb0132(const UnkStrIdPathRecord_b48* other) {
        fn_7100027464(reinterpret_cast<UnkStrIdPathRecord_b48*>(this), other);
    }
    __attribute__((always_inline)) ~UnkMineSoundRecord_ovb0132() {
        fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(this));
    }
};

extern UnkStrIdPathRecord_b48 g_unk_71726e4750 __attribute__((visibility("hidden")));

struct UnkMineSoundParams_ovb0132 {
    f32 volume;
    f32 pitch;
    bool flag;
};

// A sound handle (as src/game/CCharClassShootActivatorComponent/CShootActivatorComponent_Sound_ovb0126.cpp).
struct UnkMineSoundRef_ovb0132 {
    UnkInterp_7100105d34* sound;
    s64 serial;
};
struct UnkMineSoundHandle_ovb0132 {
    UnkMineSoundRef_ovb0132* ref;
    s64 serial;

    bool Playing() const { return ref && ref->serial == serial && serial != -1 && ref->sound; }
    UnkInterp_7100105d34* Get() const { return ref && ref->serial == serial && serial != -1 ? ref->sound : nullptr; }
};

struct UnkMineAI3_ovb0132 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // +0x30
    u8 pad_0x38[0x146a0];
    UnkMineSoundHandle_ovb0132 sound;  // +0x146d8
};
static_assert(__builtin_offsetof(UnkMineAI3_ovb0132, sound) == 0x146d8, "layout");

// Moves the playing sound's volume and pitch to the given values over 0.2 (and its third value over
// 0.2); starts the sound (g_unk_71726e4750) with them when it isn't playing.
void fn_710057dadc(UnkMineAI3_ovb0132* self, f32 volume, f32 pitch) {
    UnkMineSoundHandle_ovb0132* sound = &self->sound;
    if (sound->Playing()) {
        fn_7100105d34(sound->Get(), volume, false, 0.2f);
        fn_7100105da8(sound->Get(), pitch, 0.2f);
        fn_71001059f0(sound->Get(), 0.2f);
        return;
    }
    UnkMineSoundParams_ovb0132 params;
    params.volume = volume;
    params.pitch = pitch;
    params.flag = true;
    UnkMineSoundRecord_ovb0132 record(&g_unk_71726e4750);
    using Play = UnkMineSoundHandle_ovb0132 (*)(UnkEntityWithSound*, void*, void*, bool);
    *sound = reinterpret_cast<Play>(&fn_7100d7c45c)(self->owner, &record, &params, false);
}
