// Static initialiser of the unit of the CChozoCommanderUltimateGrabAttack::SState enum.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_71004435c0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x10];
    u8 field_0x40[8];
    u8 pad_0x48[0x130];
    u8 field_0x178[8];
};
extern UnkStatics_71004435c0 g_unk_71726c8800 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726c8668;
void fn_710043d464(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710043cef8();
extern char g_unk_71726c8780[];
extern UnkConversion g_unk_71726c8788;
extern UnkConversion g_unk_71726c87a8;
void fn_7100441980() __attribute__((visibility("hidden")));
void fn_710044198c() __attribute__((visibility("hidden")));
extern char g_unk_71726c87c8[];
extern char g_unk_71726c87c9[];
extern UnkTunable g_unk_71726c87d0;
extern void* g_unk_7101a363c8[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_71004435c0() {
    InitHeaderIds(&g_unk_71726c8800.ids);
    InitId(&g_unk_71726c8800.field_0x18, "ugrabcommandersamusintroair");
    InitId(&g_unk_71726c8800.field_0x20, "ugrabcommandersamusintroground");
    InitId(&g_unk_71726c8800.field_0x28, "ugrabcommandersamusintroground_noimpact");
    InitEnum(&g_unk_71726c8668, "CChozoCommanderUltimateGrabAttack::SState", fn_710043d464, fn_710043cef8,
             g_unk_71726c8780, &g_unk_71726c8788, fn_7100441980, &g_unk_71726c87a8, fn_710044198c);
    fn_710043d5e8();
    fn_710009ec00(g_unk_71726c87c8, g_unk_71726c8800.field_0x40);
    fn_710043d868();
    fn_710009ec00(g_unk_71726c87c9, g_unk_71726c8800.field_0x178);
    InitTunable(&g_unk_71726c87d0, fn_7100136ecc, g_unk_7101a363c8, fn_7100075104);
}
