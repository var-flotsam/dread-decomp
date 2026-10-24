// Static initialiser of the unit of CChozoWarriorAIComponent and its ETransformationType enum.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_710049d630 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    u8 pad_0x60[0x28];
    u8 field_0x88[8];
    u8 pad_0x90[0x10];
    u8 field_0xa0[8];
};
extern UnkStatics_710049d630 g_unk_71726d1248 __attribute__((visibility("hidden")));
void fn_7100497b38(void* self, const UnkStrIdBase* ids, u32 count);
void fn_7100497e30(void* self);  // its destructor, as registered
namespace base { namespace reflection { class CClass; } }
const base::reflection::CClass* fn_7100497f2c();  // CChozoWarriorAIComponent::refl_GetClass
extern char g_unk_71726d10e0[];
extern UnkEnumType g_unk_71726d10e8;
void fn_7100498514(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100498308();
extern char g_unk_71726d1200[];
extern UnkConversion g_unk_71726d1208;
extern UnkConversion g_unk_71726d1228;
void fn_710049d3d4() __attribute__((visibility("hidden")));
void fn_710049d3e0() __attribute__((visibility("hidden")));

void fn_710049d630() {
    InitHeaderIds(&g_unk_71726d1248.ids);
    InitId(&g_unk_71726d1248.field_0x18, "shield");
    InitId(&g_unk_71726d1248.field_0x20, "shieldgrapple");
    InitId(&g_unk_71726d1248.field_0x28, "block_ghost_aura");
    InitId(&g_unk_71726d1248.field_0x30, "attack_walljump_init");
    InitId(&g_unk_71726d1248.field_0x38, "transformation");
    InitId(&g_unk_71726d1248.field_0x40, "transformation_quick_init");
    InitId(&g_unk_71726d1248.field_0x48, "transformation_quick");
    InitId(&g_unk_71726d1248.field_0x50, "chozowarrior_transformation");
    InitId(&g_unk_71726d1248.field_0x58, "CChozoWarriorAIComponent");
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("attack_walljump_end1", true), UnkStrIdTmp("attack_walljump_end2", true), UnkStrIdTmp("attack_walljump_end3", true)};
        fn_7100497b38(g_unk_71726d1248.field_0x88, ids, 3);
    }();
    __cxa_atexit(fn_7100497e30, g_unk_71726d1248.field_0x88, g_unk_7101a19000);
    fn_7100497f2c();
    fn_710009ec00(g_unk_71726d10e0, g_unk_71726d1248.field_0xa0);
    InitEnum(&g_unk_71726d10e8, "CChozoWarriorAIComponent::ETransformationType", fn_7100498514, fn_7100498308,
             g_unk_71726d1200, &g_unk_71726d1208, fn_710049d3d4, &g_unk_71726d1228, fn_710049d3e0);
}
