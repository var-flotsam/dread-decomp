// CCharClassBindLightToConstantAlphaEvent unit (tools/tu_map.py, the name from reflection): when the
// target has a model (+0x228) with a node at +0x50 and the event names one (+0x10, not the empty
// string), binds the light named at +0x18 to it with the value at +0x20 (fn_710021954c); otherwise it
// unbinds the light (fn_7100219558). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

void fn_710021954c(UnkRenderManager* self, const CStrId* light, void* node, const CStrId* bone, u32 value,
                   bool flag);
void fn_7100219558(UnkRenderManager* self, const CStrId* light, bool flag);

struct UnkBindLightModel_b51 {
    u8 unk_0x0[0x50];
    void* node;  // 0x50
};

struct UnkBindLightTarget_b51 {
    u8 unk_0x0[0x228];
    UnkBindLightModel_b51* model;  // 0x228
};

struct UnkBindLightEvent_b51 {
    u8 unk_0x0[0x10];
    CStrId bone;   // 0x10
    CStrId light;  // 0x18
    u32 value;     // 0x20
};

void fn_7100cad8d4(UnkBindLightEvent_b51* self, UnkBindLightTarget_b51** target) {
    UnkBindLightTarget_b51* t = *target;
    if (!t)
        return;
    UnkBindLightModel_b51* model = t->model;
    if ((model != nullptr) & (self->bone.data != &g_unk_7101d47108->empty) && model->node) {
        fn_710021954c(g_unk_7101d47300, &self->light, model->node, &self->bone, self->value, false);
    } else {
        UnkRenderManager* rm = g_unk_7101d47300;
        fn_7100219558(rm, &self->light, false);
    }
}
