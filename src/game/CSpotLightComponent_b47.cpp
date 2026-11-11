// CSpotLightComponent's destructor (tools/tu_map.py, the name from reflection): the vtable, the scene
// object at +0x58 handed back to the render manager (fn_71002199d8), the counted resource at +0x60
// released, then the light base destructor (fn_71008ef04c). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "game/CBaseLightComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

extern char g_unk_7101a80518[];  // the component's vtable
struct UnkLightBaseB47;

struct UnkSpotLightObjectB47 {
    void** vtable;
    u8 node[8];
};

struct UnkSpotLightComponentB47 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkSpotLightObjectB47* field_0x58;
    UnkCountedResource* field_0x60;
};

void fn_71008f8ea8(UnkSpotLightComponentB47* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a80518 + 0x10);
    if (UnkSpotLightObjectB47* object = self->field_0x58)
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(object->node), 0);
    if (UnkCountedResource* res = self->field_0x60) {
        if (--res->ref_count == 0)
            fn_710001737c(res);
    }
    fn_71008ef04c(reinterpret_cast<UnkLightBaseB47*>(self));
}
