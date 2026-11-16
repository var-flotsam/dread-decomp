// CBossCameraCtrl's unit (tools/tu_map.py, unit 1164): removes a target from the boss camera's list of
// counted weak handles (+0x2a0) and tells the world (game manager +0x2228) whether the list is now empty.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2566.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7172710f50 __attribute__((visibility("hidden")));

struct UnkCameraCtrl;

// A counted reference to a weak handle: an element of the target list.
struct UnkTargetRef_l750948 {
    UnkWeakHandle* p;
    UnkTargetRef_l750948(const UnkTargetRef_l750948& other) : p(nullptr) { AssignRef(&p, other.p); }
    ~UnkTargetRef_l750948() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

// The boss camera as far as this function reads it.
struct UnkBossCameraView_l750948 {
    u8 pad_0x0[0x2a0];
    base::global::CRntVector<UnkTargetRef_l750948> targets;  // 0x2a0
};

void fn_7100750948(UnkCameraCtrl* ctrl, void* arg) {
    UnkBossCameraView_l750948* self = reinterpret_cast<UnkBossCameraView_l750948*>(ctrl);
    UnkWeakTarget* target = static_cast<UnkWeakTarget*>(arg);
    base::global::CRntVector<UnkTargetRef_l750948>& targets = self->targets;

    UnkTargetRef_l750948* end = targets.data + targets.count;
    for (UnkTargetRef_l750948* it = targets.data; it != end; ++it) {
        UnkWeakHandle* h = it->p;
        if ((h ? h->target : nullptr) == target) {
            u32 index = it - targets.data;
            if (index < targets.count) {
                targets.data[index].~UnkTargetRef_l750948();
                for (u32 i = index; i < targets.count - 1; ++i) {
                    new (&targets.data[i]) UnkTargetRef_l750948(targets.data[i + 1]);
                    targets.data[i + 1].~UnkTargetRef_l750948();
                }
                targets.count = targets.count - 1;
            }
            fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), targets.count == 0,
                          &g_unk_7172710f50);
            return;
        }
    }
}
