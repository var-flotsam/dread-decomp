// CReserveTankInfo's unit (tools/tu_map.py unit 2593): the unit's static initialiser. The header
// data, the two classes' registrations, the reserve tanks' tunable, the "AMIIBO" id and the seven
// bool properties of the reserve tanks' flags. Names and layouts are placeholders.
#include <new>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CReserveTankInfo.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkReserveTankInfoTU {
    UnkTUHeader header;
    u8 pad_0x18[0x10];
    u8 info_class[0x138];      // 0x28
    u8 tunables_class[0x8];    // 0x160
};
static_assert(__builtin_offsetof(UnkReserveTankInfoTU, tunables_class) == 0x160, "layout");

extern UnkReserveTankInfoTU g_unk_71729dbe00 __attribute__((visibility("hidden")));
extern char g_unk_71729dbbb8[];
extern char g_unk_71729dbbb9[];
extern UnkTunable g_unk_71729dbbc0;
extern CStrId g_unk_71729dbbf8;
extern void* g_unk_7101caa610[];  // the reserve tanks' tunable vtable
extern void* g_unk_7101caa640[];  // the flag properties' vtable
void fn_7100f11624(void* property);
void fn_7100f10870(void* property);

// The flags are declared through their definitions' type; they are extended properties.
static inline UnkPropertyEx<>* AsProperty(UnkDef_7100fc4b34& def) { return reinterpret_cast<UnkPropertyEx<>*>(&def); }

// A bool flag property named `name`.
template <bool VtableFirst>
static inline __attribute__((always_inline)) void InitFlag(UnkDef_7100fc4b34& def, const char* name) {
    if (VtableFirst)
        InitPropertyExVtableFirst(AsProperty(def), g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101caa640,
                                  fn_71002ef5bc, name, refl_TypeOf<fn_710000d228>(), fn_7100f11624, fn_7100f10870);
    else
        InitPropertyEx(AsProperty(def), g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101caa640, fn_71002ef5bc,
                       name, refl_TypeOf<fn_710000d228>(), fn_7100f11624, fn_7100f10870);
}

void fn_7100f11780() {
    UnkReserveTankInfoTU& tu = g_unk_71729dbe00;
    InitHeader(tu.header);
    fn_7100f0fdbc();
    fn_710009ec00(g_unk_71729dbbb8, tu.info_class);
    fn_7100f1003c();
    fn_710009ec00(g_unk_71729dbbb9, tu.tunables_class);
    InitTunable(&g_unk_71729dbbc0, fn_7100136ecc, g_unk_7101caa610, fn_7100075104);
    new (&g_unk_71729dbbf8) CStrId("AMIIBO", true);
    fn_7100000250(fn_710000051c, &g_unk_71729dbbf8, &g_unk_7101a19000);
    InitFlag<true>(g_unk_71729dbc00, "MENU_UNLOCKED");
    InitFlag<false>(g_unk_71729dbc40, "ITEM_RESERVE_TANK_LIFE_ACTIVE");
    InitFlag<true>(g_unk_71729dbc80, "ITEM_RESERVE_TANK_LIFE_FULL");
    InitFlag<false>(g_unk_71729dbcc0, "ITEM_RESERVE_TANK_SPECIAL_ENERGY_ACTIVE");
    InitFlag<true>(g_unk_71729dbd00, "ITEM_RESERVE_TANK_SPECIAL_ENERGY_FULL");
    InitFlag<false>(g_unk_71729dbd40, "ITEM_RESERVE_TANK_MISSILE_ACTIVE");
    InitFlag<true>(g_unk_71729dbd80, "ITEM_RESERVE_TANK_MISSILE_FULL");
}
