// CItemLifeComponent's static initialiser: the unit header, the component's id, two bool
// properties, a name, the class's registration and a global block of three empty vectors.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkItemLifeTU {
    UnkTUHeader header;
    CStrId destroyed;  // 0x18
    u64 field_0x20;
    UnkReflType type;  // 0x28
};

// Three vectors and a flag; element types are unknown.
struct UnkItemLifeLists {
    base::UnkVector<void*> lists[3];
    bool field_0x78;

    UnkItemLifeLists() : lists{base::UnkVector<void*>(0xf), base::UnkVector<void*>(0xf), base::UnkVector<void*>(0xf)} {
        field_0x78 = false;
    }
};

extern UnkItemLifeTU g_unk_7172732448 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172732318;
extern UnkProperty g_unk_7172732320;
extern UnkProperty g_unk_7172732348;
extern char g_unk_7101a7f228[];
void fn_71008e2738(UnkProperty* p);
void fn_71008dff20(void* p);
extern char g_unk_7172732370;
extern UnkItemLifeLists g_unk_7172732378;
void fn_71008e03dc(void* p);

void fn_71008e2ac0() {
    UnkItemLifeTU& tu = g_unk_7172732448;
    InitHeader(tu.header);
    new (&g_unk_7172732318) CStrId("LIFE", true);
    fn_7100000250(fn_710000051c, &g_unk_7172732318, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172732320, reinterpret_cast<void**>(g_unk_7101a7f228), g_unk_71726db788, "Dead",
                            refl_TypeOf<fn_710000d228>(), fn_71008e2738, fn_71008dff20);
    InitProperty(&g_unk_7172732348, reinterpret_cast<void**>(g_unk_7101a7f228), g_unk_71726db788, "Disabled",
                 refl_TypeOf<fn_710000d228>(), fn_71008e2738, fn_71008dff20);
    new (&tu.destroyed) CStrId("destroyed", true);
    fn_7100000250(fn_710000051c, &tu.destroyed, &g_unk_7101a19000);
    fn_71008dff9c();
    fn_710009ec00(&g_unk_7172732370, &tu.type);
    new (&g_unk_7172732378) UnkItemLifeLists();
    fn_7100000250(fn_71008e03dc, &g_unk_7172732378, &g_unk_7101a19000);
}
