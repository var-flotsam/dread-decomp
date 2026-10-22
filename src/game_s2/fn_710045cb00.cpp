// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_710045cb00 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    UnkFilePathStrIdBase field_0x40;
    u8 pad_0x48[0x8];
    f32 field_0x50[2];
    bool field_0x58;
    u8 pad_0x59[0x7];
    u8 field_0x60[8];
};
extern UnkStatics_710045cb00 g_unk_71726ca0c8 __attribute__((visibility("hidden")));
extern char g_unk_71726ca0c0[];
void* fn_710045b404();  // CChozoCommanderSentenceSphereMovementComponent::refl_GetClass

void fn_710045cb00() {
    InitHeaderIds(&g_unk_71726ca0c8.ids);
    g_unk_71726ca0c8.field_0x50[0] = -1.0f;
    g_unk_71726ca0c8.field_0x50[1] = -1.0f;
    g_unk_71726ca0c8.field_0x58 = true;
    InitId(&g_unk_71726ca0c8.field_0x18, "damage");
    InitClassId(&g_unk_71726ca0c8.field_0x20, "ChozoCommanderSentenceSphereMovementComponent");
    InitId(&g_unk_71726ca0c8.field_0x28, "fDamageChozoCommanderSentenceSphere");
    InitPathId(&g_unk_71726ca0c8.field_0x30, "actors/weapons/chozocommandersentencesphere/fx/sentence_sphere_destroid.bcptl");
    InitPathId(&g_unk_71726ca0c8.field_0x38, "actors/weapons/chozocommandersentencesphere/fx/sentence_sphere_explosion.bcptl");
    InitPathId(&g_unk_71726ca0c8.field_0x40, "actors/weapons/chozocommandersentencesphere/fx/sentence_sphere_implosion.bcptl");
    fn_710045b404();
    fn_710009ec00(g_unk_71726ca0c0, g_unk_71726ca0c8.field_0x60);
}
