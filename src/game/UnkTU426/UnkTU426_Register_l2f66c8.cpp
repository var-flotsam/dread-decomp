// Unit 426 (tools/tu_map.py): the resource manager registers itself, as the factory of three resource
// types (the "playdef" and "playdata" types among them), with the global registry. Names and layouts
// are placeholders.
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "game/UnkTU426.h"
#include "types.h"

// As src/game_s7/fn_7100d92158.cpp defines it.
// A registry entry (built out of line by fn_710002003c).
struct UnkEntry_7100d92158 {
    UnkDesc_7100d92158* desc;
    void* factory;
    UnkStrIdTwoStep id;
    UnkPathIdTwoStep path_0x18;
    UnkPathIdTwoStep path_0x20;
    u64 field_0x28;
    void* field_0x30;
    u64 field_0x38;
    void* field_0x40;

    UnkEntry_7100d92158(UnkDesc_7100d92158* desc, void* factory);  // fn_710002003c
};

// The manager as this function sees it: the factory interface is its second base.
struct UnkResourceManager_l2f66c8 {
    void* vtable;
    void* factory_vtable;  // 0x8, the factory base
};

static inline UnkDesc_7100d92158* Desc(void* type) {
    return static_cast<UnkDesc_7100d92158*>(type);
}

void fn_71002f66c8(UnkResourceManager* manager) {
    auto* self = reinterpret_cast<UnkResourceManager_l2f66c8*>(manager);
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(Desc(&g_unk_71727b9db0), &self->factory_vtable));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(Desc(g_unk_71727b9e30), &self->factory_vtable));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(Desc(g_unk_71727b9eb0), &self->factory_vtable));
}
