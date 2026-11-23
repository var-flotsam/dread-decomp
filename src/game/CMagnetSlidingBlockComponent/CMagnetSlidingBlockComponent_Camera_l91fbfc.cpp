// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): after the base update, while the flag at +0x420
// is set, follows the main camera: under a CAnimatedCameraCtrl the id at +0x418 takes the animation's name and
// the player's input component is switched on with it; otherwise it is switched off and the id released.
// Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkMagnetAnimatedCamera_l91fbfc {
    u8 pad_0x0[0x2b0];
    CStrId animation;  // 0x2b0
};

struct UnkMagnetPlayer_l91fbfc {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

struct UnkMagnetBlockCamera_l91fbfc {
    u8 pad_0x0[0x418];
    CStrId camera_id;     // 0x418
    bool follow_camera;  // 0x420
};

// A string id built from a string out of line (fn_7100000850); once moved from, its release folds away.
struct UnkMagnetBuiltId_l91fbfc {
    UnkStringData* data;

    explicit UnkMagnetBuiltId_l91fbfc(const CRntString& str) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkMagnetBuiltId_l91fbfc() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = &pool->empty;
        }
    }
};

// The id's move assignment: the old string released (count tested after the decrement), then the other's
// taken, the other left the empty string of the freshly loaded pool.
static inline void MoveId(CStrId* self, UnkMagnetBuiltId_l91fbfc* other) {
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (self->data && self->data != &pool->empty) {
            --self->data->ref_count;
            if (self->data->ref_count == 0) {
                StringPool_Release(pool, self->data);
                pool = g_unk_7101d47108;
            }
        }
    }
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

// The id's string, counting the read.
static inline const CRntString& Name(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_710091fbfc(UnkMagnetBlockCamera_l91fbfc* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!self->follow_camera)
        return;
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (!holder)
        return;
    UnkCameraCtrl* ctrl = fn_71010e2080(holder);
    if (!ctrl)
        return;
    if (fn_7100023d00(ctrl, fn_71010e7f90())) {
        auto* camera = reinterpret_cast<UnkMagnetAnimatedCamera_l91fbfc*>(
            fn_710009d50c(ctrl->vf_0x0(), fn_71010e7f90()) ? ctrl : nullptr);
        {
            UnkMagnetBuiltId_l91fbfc name(Name(camera->animation));
            MoveId(&self->camera_id, &name);
        }
        if (auto* player = reinterpret_cast<UnkMagnetPlayer_l91fbfc*>(fn_7100dec788(g_unk_7101d47310)))
            fn_7100872460(player->input, true, false, &self->camera_id);
        return;
    }
    if (self->camera_id.data == &g_unk_7101d47108->empty)
        return;
    if (auto* player = reinterpret_cast<UnkMagnetPlayer_l91fbfc*>(fn_7100dec788(g_unk_7101d47310)))
        fn_7100872460(player->input, false, false, &self->camera_id);
    self->camera_id.Reset();
    self->follow_camera = false;
}
