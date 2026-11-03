// Destructors of CDirLightComponent and (its copy) CSegmentLightComponent (tools/tu_map.py, names from
// reflection): the vtable, the scene object at +0x58 handed back to the render manager
// (fn_71002199d8, null when there is none), then the light base destructor (fn_71008ef04c); the
// deleting forms free the object after. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

extern char g_unk_7101a7fb98[];  // CDirLightComponent's vtable
extern char g_unk_7101a802b8[];  // CSegmentLightComponent's vtable
struct UnkLightBaseB47;
void fn_71008ef04c(UnkLightBaseB47* self);

struct UnkLightObjectB49a4 {
    void** vtable;
    u8 node[8];

    __attribute__((always_inline)) UnkItemBase_71011a91c4* Item() {
        return reinterpret_cast<UnkItemBase_71011a91c4*>(node);
    }
};

struct UnkLightComponentB49a4 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkLightObjectB49a4* field_0x58;

    __attribute__((always_inline)) void Destroy(char* vt) {
        vtable = reinterpret_cast<void**>(vt + 0x10);
        fn_71002199d8(g_unk_7101d47300, field_0x58 ? field_0x58->Item() : nullptr, 0);
        fn_71008ef04c(reinterpret_cast<UnkLightBaseB47*>(this));
    }
};

void fn_71008eb024(UnkLightComponentB49a4* self) { self->Destroy(g_unk_7101a7fb98); }

void fn_71008eb078(UnkLightComponentB49a4* self) {
    self->Destroy(g_unk_7101a7fb98);
    operator delete(self);
}

void fn_71008f3f9c(UnkLightComponentB49a4* self) { self->Destroy(g_unk_7101a802b8); }

void fn_71008f3ff0(UnkLightComponentB49a4* self) {
    self->Destroy(g_unk_7101a802b8);
    operator delete(self);
}
