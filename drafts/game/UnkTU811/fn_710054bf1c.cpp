// UnkTU811 (CEmmyAIComponent's unit): the E.M.M.I.'s movement speed. The action named on its blackboard picks
// a speed from the tunables (patrol, search, chase; chasing along a path blends between two speeds by the
// path's length and keeps the result); in the first mode a short wait scales it down; the result is scaled
// by the speed factor over the time scale. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyAI_b57a3;
struct UnkNavComponent_b66a4;
struct UnkNavPath_b66a4;
bool fn_710054c504(UnkEmmyAI_b57a3* self, bool flag);
UnkNavPath_b66a4* fn_71006f4e78(UnkNavComponent_b66a4* self);

extern char g_unk_71726df1a0[];
extern char g_unk_71726df038[];
extern char g_unk_71726debe0[];

// The E.M.M.I.'s speed tunables.
struct UnkEmmySpeeds_l54bf1c {
    u8 pad_0x0[0x40];
    f32 patrol;            // 0x40
    f32 searchNear;        // 0x44
    f32 lookNear;          // 0x48
    bool followsPath;      // 0x4c
    f32 chaseFar;          // 0x50
    f32 chaseNear;         // 0x54
    f32 pathFar;           // 0x58
    f32 pathNear;          // 0x5c
    f32 alerted;           // 0x60
    f32 searchFar;         // 0x64
    f32 lookFar;           // 0x68
    f32 chase;             // 0x6c
    f32 idle;              // 0x70
    f32 chaseOther;        // 0x74
    u8 pad_0x78[0x4];
    f32 chaseHunt;         // 0x7c
    f32 chaseHuntClose;    // 0x80
    f32 idleOther;         // 0x84
    u8 pad_0x88[0x9c - 0x88];
    f32 closeDistance;     // 0x9c
    f32 waitFactor;        // 0xa0
    f32 waitTime;          // 0xa4
};

struct UnkEmmyContext_l54bf1c {
    void** vtable;

    UnkEmmySpeeds_l54bf1c* vf_0xd8() {
        return reinterpret_cast<UnkEmmySpeeds_l54bf1c* (*)(UnkEmmyContext_l54bf1c*)>(vtable[0xd8 / 8])(this);
    }
};

struct UnkNavPathView_l54bf1c {
    u8 pad_0x0[0x12a80];
    f32 length;  // 0x12a80
};

struct UnkEmmyAI_l54bf1c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after[0x14c08 - 0x1260 - sizeof(CBlackboard)];
    f32 baseSpeed;  // 0x14c08
    u8 pad_0x14c0c[0xc];
    f32 pathSpeed;  // 0x14c18
    u8 pad_0x14c1c[0x27720 - 0x14c1c];
    bool alerted;  // 0x27720
    u8 pad_0x27721[0x29ee0 - 0x27721];
    UnkEmmyContext_l54bf1c* context;  // 0x29ee0
    u8 pad_0x29ee8[0x4];
    u32 searchKind;  // 0x29eec
    u32 mode;  // 0x29ef0
    u8 pad_0x29ef4[0x4];
    f32 waited;  // 0x29ef8
    u8 pad_0x29efc[0x2a600 - 0x29efc];
    f32 speedFactor;  // 0x2a600
    u8 pad_0x2a604[0x2a6b0 - 0x2a604];
    f32 timeScale;  // 0x2a6b0

    UnkEmmySpeeds_l54bf1c* Speeds() { return context->vf_0xd8(); }
    bool vf_0x7f0() { return reinterpret_cast<bool (*)(UnkEmmyAI_l54bf1c*)>(vtable[0x7f0 / 8])(this); }
    bool vf_0x840() { return reinterpret_cast<bool (*)(UnkEmmyAI_l54bf1c*)>(vtable[0x840 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54bf1c, baseSpeed) == 0x14c08, "UnkEmmyAI_l54bf1c layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l54bf1c, context) == 0x29ee0, "UnkEmmyAI_l54bf1c layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l54bf1c, timeScale) == 0x2a6b0, "UnkEmmyAI_l54bf1c layout");

// The name of the action the blackboard holds; empty until read.
struct UnkActionName_l54bf1c {
    base::global::UnkStringData* data;

    UnkActionName_l54bf1c() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkActionName_l54bf1c() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    CStrId* Id() { return reinterpret_cast<CStrId*>(this); }
    bool operator==(const CStrId& other) const { return data == other.data; }
};

static inline const CStrId& Id(const char* global) {
    return *reinterpret_cast<const CStrId*>(global);
}

f32 fn_710054bf1c(UnkEmmyAI_l54bf1c* self) {
    f32 speed = self->baseSpeed;
    UnkActionName_l54bf1c action;
    CBlackboard* blackboard = &self->blackboard;
    CBlackboard::CSection* section = fn_7101047270(blackboard, GameManagerIdTable().field_0x1b58);
    if (section && fn_710051d464(section, g_unk_71726e20a8, action.Id())) {
        bool alerted = self->alerted;
        bool close;
        if (self->mode == 1) {
            if (!self->vf_0x7f0() || fn_710054c504(reinterpret_cast<UnkEmmyAI_b57a3*>(self), false)) {
                f32 x = self->owner->field_0x44;
                UnkActor* target = fn_7100d7c990(self->owner);
                close = __builtin_fabsf(x - target->field_0x44) < self->Speeds()->closeDistance;
            } else {
                close = false;
            }
        } else {
            close = false;
        }

        if (self->vf_0x840()) {
            if (alerted)
                speed = self->Speeds()->alerted;
            else
                speed = self->mode ? self->Speeds()->idleOther : self->Speeds()->idle;
        } else if (action == Id(g_unk_71726ded60)) {
            speed = self->Speeds()->patrol;
        } else if (action == Id(g_unk_71726df038)) {
            switch (self->searchKind) {
            case 0:
                speed = self->Speeds()->searchNear;
                break;
            case 1:
                speed = self->Speeds()->searchFar;
                break;
            }
        } else if (action == Id(g_unk_71726df1a0)) {
            switch (self->searchKind) {
            case 0:
                speed = self->Speeds()->lookNear;
                break;
            case 1:
                speed = self->Speeds()->lookFar;
                break;
            }
        } else if (action == Id(g_unk_71726debd0)) {
            if (alerted) {
                if (self->Speeds()->followsPath) {
                    fn_7100d7c990(self->owner);
                    UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(self));
                    auto* path = navigation ? reinterpret_cast<base::reflection::UnkReflObject*>(
                                                  fn_71006f4e78(reinterpret_cast<UnkNavComponent_b66a4*>(navigation)))
                                            : nullptr;
                    if (path && fn_710009d50c(path->vf_0x0(), fn_71002acd74())) {
                        f32 length = reinterpret_cast<UnkNavPathView_l54bf1c*>(path)->length;
                        f32 nearLength = self->Speeds()->pathNear;
                        f32 range = self->Speeds()->pathFar - nearLength;
                        f32 nearSpeed = self->Speeds()->chaseNear;
                        f32 farSpeed = self->Speeds()->chaseFar;
                        f32 t = (length - nearLength) / range;
                        t = t > 0.0f ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
                        speed = nearSpeed + t * (farSpeed - nearSpeed);
                        self->pathSpeed = speed;
                    } else if (self->owner->field_0x218 && fn_71010cc3a4(self->owner->field_0x218) == 4) {
                        speed = self->pathSpeed;
                    } else {
                        speed = self->Speeds()->chaseFar;
                    }
                } else {
                    speed = self->Speeds()->chaseFar;
                }
            } else {
                s32 hunt = 0;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, g_unk_71726b7cb8))
                    fn_7100395a30(s, g_unk_71726debe0, &hunt);
                switch (hunt) {
                case 0: {
                    u32 mode = self->mode;
                    UnkEmmySpeeds_l54bf1c* speeds = self->Speeds();
                    speed = mode == 0 ? speeds->chase : speeds->chaseOther;
                    break;
                }
                case 1: {
                    u32 mode = self->mode;
                    UnkEmmySpeeds_l54bf1c* speeds = self->Speeds();
                    speed = mode == 0 ? speeds->chase : close ? speeds->chaseHuntClose : speeds->chaseHunt;
                    break;
                }
                }
            }
        }
    }

    if (self->mode == 0 && self->waited < self->Speeds()->waitTime)
        speed *= self->Speeds()->waitFactor;
    return speed * self->speedFactor * (1.0f / self->timeScale);
}
