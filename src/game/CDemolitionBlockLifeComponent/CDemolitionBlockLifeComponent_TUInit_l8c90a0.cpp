// CDemolitionBlockLifeComponent's static initialiser (tools/tu_map.py unit 1374): the unit header, the
// EDemolitionPhase enum with its converters to and from int, the component's id, two bool properties,
// the names, sound presets and dissolve effect the component uses, and the class's registration.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CDemolitionBlockLifeComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;
using base::global::CRntString;

// The unit's data (its statics, merged into one block).
struct UnkDemolitionBlockLifeTU {
    UnkTUHeader header;
    CStrId destroyed;               // 0x18
    CStrId explosion;               // 0x20
    CStrId material;                // 0x28
    CFilePathStrId dissolve_fx;     // 0x30
    u64 field_0x38;
    CRntString phase_one_two_sound;  // 0x40
    CRntString phase_one_one_sound;  // 0x70
    CRntString phase_two_one_sound;  // 0xa0
    UnkReflType type;  // 0xd0
};
static_assert(__builtin_offsetof(UnkDemolitionBlockLifeTU, phase_one_two_sound) == 0x40, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlockLifeTU, type) == 0xd0, "layout");

extern UnkDemolitionBlockLifeTU g_unk_7172730d30 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172730b30;  // also named in src/reflection/
void fn_71008c448c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71008c4220();
extern char g_unk_7172730c48;
extern UnkConverter g_unk_7172730c50;
extern UnkConverter g_unk_7172730c70;
void fn_71008c89d8() __attribute__((visibility("hidden")));
void fn_71008c89e4() __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172730c90;
extern UnkProperty g_unk_7172730cc0;
extern void* g_unk_7101a7e5d0[];
void fn_71008c8de8(UnkProperty* p);
void fn_71008c460c(void* p);
extern char g_unk_7172730ce8;

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_71008c90a0() {
    UnkDemolitionBlockLifeTU& tu = g_unk_7172730d30;
    InitHeader(tu.header);
    InitEnum(AsUnk(&g_unk_7172730b30), "EDemolitionPhase", fn_71008c448c, fn_71008c4220);
    fn_710009ec00(&g_unk_7172730c48, AsUnk(&g_unk_7172730b30));
    InitEnumConverters(AsUnk(&g_unk_7172730b30), g_unk_7172730c50, fn_71008c89d8, g_unk_7172730c70, fn_71008c89e4);
    new (&g_unk_7172730c90) CStrId("BLOCKLIFE", true);
    fn_7100000250(fn_710000051c, &g_unk_7172730c90, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172730c98, g_unk_7101a7e5d0, g_unk_71726db788, "Dead",
                            refl_TypeOf<fn_710000d228>(), fn_71008c8de8, fn_71008c460c);
    InitProperty(&g_unk_7172730cc0, g_unk_7101a7e5d0, g_unk_71726db788, "HuskRemoved", refl_TypeOf<fn_710000d228>(),
                 fn_71008c8de8, fn_71008c460c);
    new (&tu.destroyed) CStrId("destroyed", true);
    fn_7100000250(fn_710000051c, &tu.destroyed, &g_unk_7101a19000);
    new (&tu.explosion) CStrId("explosion", true);
    fn_7100000250(fn_710000051c, &tu.explosion, &g_unk_7101a19000);
    new (&tu.phase_one_two_sound)
        CRntString("audiopreset:system/snd/presets/props/breakables/breakable_phaseone_two");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), &tu.phase_one_two_sound, &g_unk_7101a19000);
    new (&tu.phase_one_one_sound)
        CRntString("audiopreset:system/snd/presets/props/breakables/breakable_phaseone_one");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), &tu.phase_one_one_sound, &g_unk_7101a19000);
    new (&tu.phase_two_one_sound)
        CRntString("audiopreset:system/snd/presets/props/breakables/breakable_phasetwo_one");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), &tu.phase_two_one_sound, &g_unk_7101a19000);
    new (&tu.material) CStrId("cascamat", true);
    fn_7100000250(fn_710000051c, &tu.material, &g_unk_7101a19000);
    new (&tu.dissolve_fx) CFilePathStrId("actors/props/breakablecave006/fx/dissolve_cascarilla.bcptl", true);
    fn_7100000250(fn_7100000c7c, &tu.dissolve_fx, &g_unk_7101a19000);
    fn_71008c4688();
    fn_710009ec00(&g_unk_7172730ce8, &tu.type);
}
