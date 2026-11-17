// CUsableComponent's static initialiser (tools/tu_map.py, unit 1607): the unit header, the component's id,
// four bool properties, the component's and its tunable's registrations, the tunable and one more static.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CUsableComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkUsableComponentTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    UnkReflType component_type;  // 0x28
    u8 field_0x28_rest[0x160 - 0x28 - sizeof(UnkReflType)];
    UnkReflType tunable_type;  // 0x160
};

extern UnkUsableComponentTU g_unk_7172753520 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172753140;
extern char g_unk_7101a945e0[];
void fn_7100aa3e68(UnkProperty* p);
void fn_7100a9bc70(void* p);
extern char g_unk_71727531e8;
extern char g_unk_71727531e9;

// The tunable (declared with the save station's view of it) and its constructor and destructor.
struct UnkSaveStationTunables_ovb0118;
struct UnkUsableTunable_la9d3a8;
void fn_7100a9d3a8(UnkUsableTunable_la9d3a8* self);
void fn_7100a9d710(void* self);

// Another static of the unit, built and destroyed by two copies of one function.
struct UnkLinkGroups;

static inline UnkProperty* AsProperty(u8* p) {
    return reinterpret_cast<UnkProperty*>(p);
}

void fn_7100aa4800() {
    UnkUsableComponentTU& tu = g_unk_7172753520;
    InitHeader(tu.header);
    new (&g_unk_7172753140) CStrId("USABLE", true);
    fn_7100000250(fn_710000051c, &g_unk_7172753140, &g_unk_7101a19000);
    void** vtable = reinterpret_cast<void**>(g_unk_7101a945e0);
    InitPropertyVtableFirst(AsProperty(g_unk_7172753148), vtable, g_unk_71726db788, "Used",
                            refl_TypeOf<fn_710000d228>(), fn_7100aa3e68, fn_7100a9bc70);
    InitProperty(AsProperty(g_unk_7172753170), vtable, g_unk_71726db788, "Completed", refl_TypeOf<fn_710000d228>(),
                 fn_7100aa3e68, fn_7100a9bc70);
    InitPropertyVtableFirst(AsProperty(g_unk_7172753198), vtable, g_unk_71726db788, "Discovered",
                            refl_TypeOf<fn_710000d228>(), fn_7100aa3e68, fn_7100a9bc70);
    InitProperty(AsProperty(g_unk_71727531c0), vtable, g_unk_71726db788, "Disabled", refl_TypeOf<fn_710000d228>(),
                 fn_7100aa3e68, fn_7100a9bc70);
    fn_7100a9bcec();
    fn_710009ec00(&g_unk_71727531e8, &tu.component_type);
    fn_7100a9c7f4();
    fn_710009ec00(&g_unk_71727531e9, &tu.tunable_type);
    fn_7100a9d3a8(reinterpret_cast<UnkUsableTunable_la9d3a8*>(&g_unk_71727531f0));
    fn_7100000250(fn_7100a9d710, &g_unk_71727531f0, &g_unk_7101a19000);
    fn_7100a9dbac(&g_unk_7172753260);
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100a9dd78), &g_unk_7172753260, &g_unk_7101a19000);
}
