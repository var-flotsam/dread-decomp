// The static initialisers of two resource classes' units: the header ids in the reverse order
// ("sources" first, and no word stored), the class registered, and the resource type (class name,
// file extension and source extension) added to a registry under a category, the registry being
// destroyed at exit (as at the end of fn_7100e72680 in ../game_s7/fn_7100e72680.cpp).
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkResourceStatics {
    UnkStrIdBase sources;
    UnkStrIdBase binaries;
    u8 pad_0x10[0x8];
    u8 type[8];  // the class type, built by the class getter
};

__attribute__((always_inline)) inline void InitResource(UnkResourceStatics* s, const base::reflection::CClass* (*get_class)(), char* list,
                                                        const char* name, const char* ext, const char* source,
                                                        void* registry, const char* category) {
    new (&s->sources) UnkStrIdBase("sources", true);
    __cxa_atexit(fn_710000051c, &s->sources, g_unk_7101a19000);
    new (&s->binaries) UnkStrIdBase("binaries", true);
    __cxa_atexit(fn_710000051c, &s->binaries, g_unk_7101a19000);
    get_class();
    fn_710009ec00(list, s->type);
    {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = source;
        fn_7100004578(&types[0], name, ext, sources0, 1);
        fn_7100004698(registry, category, types, 1);
        fn_7100004a44(&types[0]);
    }
    __cxa_atexit(fn_7100004b94, registry, g_unk_7101a19000);
}

extern UnkResourceStatics g_unk_7101d4b5a8 __attribute__((visibility("hidden")));
extern char g_unk_7101d4b410[];
extern char g_unk_7101d4b418[];

void fn_7100005700() {
    InitResource(&g_unk_7101d4b5a8, fn_710000430c, g_unk_7101d4b410, "base::animation::CAnimationResource",
                 ".bcskla", ".cskla", g_unk_7101d4b418, "anim");
}

extern UnkResourceStatics g_unk_7101d4f3b8 __attribute__((visibility("hidden")));
extern char g_unk_7101d4f330[];
extern char g_unk_7101d4f338[];

void fn_7100029f20() {
    InitResource(&g_unk_7101d4f3b8, fn_7100029a50, g_unk_7101d4f330, "base::curves::CCurveResource", ".bcurv",
                 ".curv", g_unk_7101d4f338, "curve");
}
