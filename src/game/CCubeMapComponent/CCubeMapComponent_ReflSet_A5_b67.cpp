// CCubeMapComponent's unit (tools/tu_map.py): a reflected setter for the string id at +0x88 (copy-assigned from the
// argument); it then passes the position of the +0x50 object (words at +0x3c, +0x4c, +0x5c) and the id to the render
// manager (fn_710021a0f0). Answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;


void fn_710021a0f0(UnkRenderManager* manager, const UnkVec3f* pos, const CStrId* name);

struct UnkCubeMap_A5_b67 {
    u8 pad_0x0[0x50];
    const u8* xform;
    u8 pad_0x58[0x30];
    CStrId name;
};

UnkReflValue fn_71007a0e04(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkCubeMap_A5_b67* self = *reinterpret_cast<UnkCubeMap_A5_b67**>(obj + 8);
    self->name = *base::reflection::refl_ArgValue<CStrId>(arg);
    UnkRenderManager* manager = g_unk_7101d47300;
    const f32* m = reinterpret_cast<const f32*>(self->xform);
    UnkVec3f pos(m[15], m[19], m[23]);
    fn_710021a0f0(manager, &pos, &self->name);
    return base::reflection::refl_EmptyValue();
}
