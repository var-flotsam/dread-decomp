// CPositionalSoundComponent unit (tools/tu_map.py unit 2711): its update. While the listener is within the
// component's range of the owner the playing sound follows the owner (or a model joint) and is stopped when
// it drifts out; otherwise it is started after the repeat delay. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/snd/CSoundSystem.h"
#include "base/snd/UnkSound.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSoundRef.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/UnkXformNode.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkPositionalSound_ovb56;
struct UnkSoundObject_b50;
struct UnkSoundMgr;
const char* CPositionalSoundComponent_PickRandomSoundPath(UnkPositionalSound_ovb56* self);

struct UnkPositionalUpdate_l14c068;
void fn_7100108084(UnkSoundMgr* mgr, base::math::CVector3D* a, base::math::CVector3D* b, base::math::CVector3D* c);
void fn_7100105ebc(UnkSound* sound, const UnkVec3f* pos);
UnkSoundHandle fn_710114c618(UnkPositionalUpdate_l14c068* self, const CFilePathStrId* id, bool first);

struct UnkSoundOwner_l14c068 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkSoundModelHost_l14c068 {
    u8 pad_0x0[0x228];
    void* model;  // 0x228
};

struct UnkSoundHolder_l14c068 {
    u8 pad_0x0[0x10];
    UnkSoundModelHost_l14c068* host;  // 0x10
};

struct UnkPositionalUpdate_l14c068 {
    u8 pad_0x0[0x30];
    UnkSoundOwner_l14c068* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 field_0x40[0x30];
    f32 near;      // 0x70
    f32 range;     // 0x74
    f32 delay;     // 0x78
    u8 pad_0x7c[8];
    f32 timer;     // 0x84
    UnkSoundHandle handles[2];  // 0x88
    bool active;   // 0xa8
    u8 pad_0xa9[7];
    CStrId joint;  // 0xb0
    u8 pad_0xb8[4];
    f32 fade;      // 0xbc
    f32 scaleX;    // 0xc0
    f32 scaleY;    // 0xc4
    UnkSoundHolder_l14c068* holder;  // 0xc8
};
static_assert(__builtin_offsetof(UnkPositionalUpdate_l14c068, holder) == 0xc8, "");

static inline UnkSound* GetSound_l14c068(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkPathId_l14c068 {
    union {
        CFilePathStrId id;
    };
    explicit UnkPathId_l14c068(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkPathId_l14c068() { id.~CFilePathStrId(); }
};

struct UnkPathRecord_l14c068 {
    u8 unk_0x0[0x10];
    char text[0x34];
    union {
        s32 uses;  // 0x44
    };
};

static inline const char* Use_l14c068(const CFilePathStrId* path) {
    ++reinterpret_cast<UnkPathRecord_l14c068*>(path->data)->uses;
    return reinterpret_cast<UnkPathRecord_l14c068*>(path->data)->text;
}

// Stops the sound of a handle (fading by the component's time) and empties the handle.
inline void Stop_l14c068(UnkPositionalUpdate_l14c068* self, UnkSoundHandle* handle) {
    if (UnkSound* sound = GetSound_l14c068(handle)) {
        fn_7100105a80(sound, self->fade, 0.0f);
        *handle = g_unk_7108b6c460;
    }
}

// Starts a new sound from one of the component's paths into the handle of `first` (0x88) or the other one.
inline void Start_l14c068(UnkPositionalUpdate_l14c068* self, bool first) {
    const char* picked = CPositionalSoundComponent_PickRandomSoundPath(reinterpret_cast<UnkPositionalSound_ovb56*>(self));
    if (!CRntString_KeysDiffer(reinterpret_cast<const CRntString*>(picked),
                               reinterpret_cast<const CRntString*>(Use_l14c068(&GameManagerIdTable().field_0x5f8))))
        return;
    UnkPathId_l14c068 id(*reinterpret_cast<const CRntString*>(picked));
    UnkSoundHandle* handle = &self->handles[first ? 0 : 1];
    *handle = fn_710114c618(self, &id.id, first);
    if (UnkSound* sound = GetSound_l14c068(handle))
        fn_7100106024(reinterpret_cast<UnkSoundObject_b50*>(sound), self->field_0x40);
}

void fn_710114c068(UnkPositionalUpdate_l14c068* self, f32 dt) {
    if (!self->owner)
        return;
    base::math::CVector3D listener = {0.0f, 0.0f, 0.0f};
    base::math::CVector3D aux = {0.0f, 0.0f, 0.0f};
    fn_7100108084(g_unk_7101d4d820, &listener, &aux, &aux);

    UnkSoundOwner_l14c068* owner = self->owner;
    f32 range = self->range;
    f32 dist = 0.0f;
    bool inside = false;
    if (owner && !(fabsf(listener.x - owner->x) > range * self->scaleX) &&
        !(fabsf(listener.y - owner->y) > range * self->scaleY)) {
        f32 dx = owner->x - listener.x;
        f32 dy = owner->y - listener.y;
        f32 dz = owner->z - listener.z;
        f32 d2 = dx * dx + dy * dy + dz * dz;
        if (!(d2 > range * range)) {
            dist = sqrtf(d2);
            inside = true;
        }
    }

    if (!inside) {
        Stop_l14c068(self, &self->handles[0]);
        return;
    }

    UnkSoundHandle* handle = &self->handles[0];
    UnkSound* sound = GetSound_l14c068(handle);
    if (self->active) {
        if (!sound) {
            if (self->range < self->near) {
                self->timer -= self->delay;
                Start_l14c068(self, true);
            } else if (dist < self->range) {
                Start_l14c068(self, true);
            }
            return;
        }
        if (self->joint.data == &g_unk_7101d47108->empty) {
            UnkVec3f pos(self->owner->x, self->owner->y, self->owner->z);
            if (UnkSound* s = GetSound_l14c068(handle))
                fn_7100105ebc(s, &pos);
        } else if (self->holder && self->holder->host && self->holder->host->model) {
            u64 key = self->joint.data->Key();
            UnkVec3f pos(0.0f, 0.0f, 0.0f);
            fn_710117364c(self->holder->host->model, &key, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
            if (UnkSound* s = GetSound_l14c068(handle))
                fn_7100105ebc(s, &pos);
        }
        if (dist > self->range)
            Stop_l14c068(self, handle);
    } else {
        if (sound)
            Stop_l14c068(self, handle);
        if (self->delay > 0.0f) {
            self->timer += dt;
            if (self->timer >= self->delay) {
                if (self->range < self->near || !(dist > self->range)) {
                    self->timer -= self->delay;
                    Start_l14c068(self, false);
                }
            }
        }
    }
}
