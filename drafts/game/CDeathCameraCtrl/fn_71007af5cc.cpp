// CDeathCameraCtrl unit (tools/tu_map.py): places the death camera along -z from the player, at most the
// tuning's distance (+0x40 of its tuning) away, shifted by the target's offset when the target is a
// "METROID" or "BOSS"; runs the base step and then follows the game manager's focus object through its
// weak handle. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

u64 fn_71010e383c(char* p0);
char* fn_71010e217c(char* p0);

// A string id's data as the text reads see it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_l7af5cc {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[4];
    union {
        s32 field_0x44;
    };
};

// Whether the id names the same text as `s` (by key); counts the use.
static inline bool NameIs(const CStrId& id, const CRntString& s) {
    ++reinterpret_cast<UnkStrIdText_l7af5cc*>(id.data)->field_0x44;
    return reinterpret_cast<UnkStrIdText_l7af5cc*>(id.data)->str.key == s.key;
}

struct UnkDeathCamPlayer_l7af5cc {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkDeathCamTarget_l7af5cc {
    u8 pad_0x0[0x20];
    CStrId kind;  // 0x20
    u8 pad_0x28[0x284 - 0x28];
    f32 offset_x;  // 0x284
    f32 offset_y;  // 0x288
};

struct UnkDeathCamTuning_l7af5cc {
    u8 pad_0x0[0x40];
    f32 distance;  // 0x40
};

struct UnkDeathCamTuningRef_l7af5cc {
    u8 pad_0x0[0x10];
    UnkDeathCamTuning_l7af5cc* tuning;
};

struct UnkDeathCamArg_l7af5cc {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
};

struct UnkDeathCamera_l7af5cc {
    u8 pad_0x0[0x68];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0xb0 - 0x80];
    UnkWeakHandle* target;               // 0xb0
    UnkDeathCamTuningRef_l7af5cc* tuning;  // 0xb8
};

void fn_71007af5cc(UnkDeathCamera_l7af5cc* self, UnkDeathCamArg_l7af5cc* arg) {
    UnkGameManager* manager = g_unk_7101d47310;
    auto* player = reinterpret_cast<UnkDeathCamPlayer_l7af5cc*>(fn_7100dec788(manager));
    self->dir.x = 0.0f;
    self->dir.y = 0.0f;
    self->dir.z = -1.0f;
    if (arg && player) {
        f32 px = player->x;
        f32 py = player->y;
        f32 pz = player->z;
        UnkVec3f node = fn_7100206154(&arg->node);
        f32 dx = px - node.x;
        f32 dy = py - node.y;
        f32 dz = pz - node.z;
        f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
        f32 limit = self->tuning && self->tuning->tuning ? self->tuning->tuning->distance : 1665.8074f;
        f32 d = dist < limit ? dist : limit;
        self->pos.x = px - self->dir.x * d;
        self->pos.y = py - d * self->dir.y;
        self->pos.z = pz - d * self->dir.z;
        if (self->target && self->target->target) {
            auto* target = reinterpret_cast<UnkDeathCamTarget_l7af5cc*>(
                fn_71010e383c(reinterpret_cast<char*>(self->target->target)));
            if (target && (NameIs(target->kind, CRntString("METROID")) || NameIs(target->kind, CRntString("BOSS")))) {
                f32 ox = target->offset_x;
                f32 x = self->pos.x;
                f32 y = self->pos.y;
                self->pos.x = ox + x;
                self->pos.y = y + target->offset_y;
            }
        }
    } else {
        self->pos = fn_7100206154(&arg->node);
    }
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
    UnkWeakHandle* handle = GetWeakHandle(static_cast<UnkWeakTarget*>(fn_7100defcec(manager)));
    AssignRef(&self->target, handle);
    fn_71010e217c(reinterpret_cast<char*>(handle ? handle->target : nullptr));
}
