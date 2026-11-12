// CModelInstanceComponent's unit (tools/tu_map.py): the model instance. Updating (fn_710093704c): when
// the component has an instance (+0x50, not the invalid id), the owner's transform (+0x30, its +0x38)
// is copied, given the component's scale (+0x58) and its matrix passed to the render manager
// (fn_7100219e64). Creating (fn_7100936da0): with a model path (+0x48) and no instance yet, the game
// manager's model for the path (fn_7100e04a54) is instanced at that matrix (fn_7100219dd0) and shown
// as fn_71009f1988 says (fn_7100219e58). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "game/CSceneComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

// The game manager's model for a path (none when it isn't loaded).
u64 fn_7100e04a54(UnkGameManager* self, const CFilePathStrId* path);

struct UnkModelOwner_mic {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkModelInstanceComponent_mic {
    u8 pad_0x0[0x30];
    UnkModelOwner_mic* owner;  // 0x30
    u8 pad_0x38[0x10];
    CFilePathStrId model;  // 0x48
    u64 instance;    // 0x50
    UnkVec3f scale;  // 0x58
};

static const u64 kNoInstance_mic = 0xffffffff;

void fn_710093704c(UnkModelInstanceComponent_mic* self) {
    if (self->instance == kNoInstance_mic)
        return;
    UnkXform xform(self->owner->xform);
    xform.SetScale(self->scale);
    const UnkMatrix44& matrix = xform.GetMatrix();
    fn_7100219e64(g_unk_7101d47300, self->instance, &matrix);
}

void fn_7100936da0(UnkModelInstanceComponent_mic* self) {
    if (self->model.data == &g_unk_7101d47200->empty)
        return;
    if (self->instance != kNoInstance_mic)
        return;
    u64 model = fn_7100e04a54(g_unk_7101d47310, &self->model);
    if (!model)
        return;
    UnkXform xform(self->owner->xform);
    xform.SetScale(self->scale);
    UnkMatrix44 matrix;
    UnkCopyMatrix(&matrix, xform.GetMatrix());
    fn_7100219dd0(g_unk_7101d47300, model, &matrix, 0, 1, &self->instance, 0);
    fn_7100219e58(g_unk_7101d47300, self->instance, fn_71009f1988(self));
}
