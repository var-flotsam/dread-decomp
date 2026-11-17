// CEmmyGrabSamusAttack (unit 800): the grab's hit feedback. Plays the two hit sounds through the
// entity (one shared sound description, its path switched between them) and the hit-voice rumble.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector3D;

struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkEntityWithSound;
struct UnkRumbleParams;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);
UnkHandle fn_7100d7c650(UnkEntityWithSound* self, const UnkRumbleParams* params, bool a, bool b, bool c);

// The unit's two hit sounds (file-path ids built by its static initialiser).
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_71726de580;
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_71726de588;

// A vibration file's path, kept for the program's life.
struct UnkVibrationPath_l50d150 : CFilePathStrId {
    explicit UnkVibrationPath_l50d150(const char* str) : CFilePathStrId(str, true) {}
    ~UnkVibrationPath_l50d150();  // fn_7100000c7c
};

// A file-path id (pool g_unk_7101d47200) whose count is released in one step.
struct UnkRumblePath_l50d150 {
    UnkStringData* data;

    explicit UnkRumblePath_l50d150(const UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkRumblePath_l50d150& operator=(const UnkFilePathStrIdBase& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkRumblePath_l50d150() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from (UnkRumbleParams), with its defaults.
struct UnkRumbleDesc_l50d150 {
    UnkRumblePath_l50d150 path;
    u64 field_0x8;
    u32 kind;          // 0x10
    CVector3D offset;  // 0x14
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleDesc_l50d150()
        : path(g_unk_7101d470e8), field_0x8(0), kind(0x7fffffff), offset(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

struct UnkGrabOwner_l50d150 {
    void** vtable;
    UnkEntityWithSound* entity;
};

void fn_710050d150(UnkGrabAttack_A3_b68* self) {
    UnkGrabOwner_l50d150* owner = reinterpret_cast<UnkGrabOwner_l50d150*>(self);

    static UnkSoundDesc s_sound;
    s_sound.kind.field_0x0 = 0;
    s_sound.path = g_unk_71726de580;
    s_sound.field_0x38 = 0.0f;
    s_sound.field_0x6d = true;
    s_sound.field_0x4c = 0.0f;
    fn_7100d7c374(owner->entity, &s_sound, &g_unk_7101d470e0, false, true, true);
    s_sound.path = g_unk_71726de588;
    fn_7100d7c374(owner->entity, &s_sound, &g_unk_7101d470e0, false, true, true);

    static const UnkVibrationPath_l50d150 s_hitvoice("actors/emmy/emmy_hitvoice_01.bnvib");
    UnkRumbleDesc_l50d150 rumble;
    rumble.kind = 3;
    rumble.path = s_hitvoice;
    rumble.field_0x20[2] = 2.0f;
    fn_7100d7c650(owner->entity, reinterpret_cast<const UnkRumbleParams*>(&rumble), false, true, true);
}
