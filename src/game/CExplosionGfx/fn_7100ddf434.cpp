// Member fields of CExplosionGfx (its class getter's x4 function, fn_7100ddf434). Written from
// tools/refl_attrs.py's draft; the parser stops at defaults copied from global vectors, which
// were added by hand. Those copies are member by member (the original loads the components as
// integers and merges neighbours, unlike a copy of the whole vector). The three globals are
// constants reached through the GOT (names unknown).
#include "base/reflection/UnkReflAttributes.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
extern base::reflection::CType g_unk_71727be388;

extern const base::math::CVector2D g_unk_7101647788;
extern const base::math::CVector2D g_unk_7101647780;
extern const base::math::CVector4D g_unk_71016477c4;
void fn_7100ddf434(CClass* cls) {
    UnkReflField* field0 = fn_7100096524(cls, CStrId("fLength", true), fn_710000abbc(), 0xd4, -1, -1);
    field0->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field1 = fn_7100096524(cls, CStrId("sParticlesPath", true), fn_710000ad9c(), 0xa0, -1, -1);
    field1->attrs.Add<base::global::CFilePathStrId>(fn_7100075240(), base::global::CFilePathStrId(g_unk_7101d470e8));
    UnkReflField* field2 = fn_7100096524(cls, CStrId("sAdditionalParticlesPath", true), fn_710000ad9c(), 0xa8, -1, -1);
    field2->attrs.Add<base::global::CFilePathStrId>(fn_7100075240(), base::global::CFilePathStrId(g_unk_7101d470e8));
    UnkReflField* field3 = fn_7100096524(cls, CStrId("sMeshPath", true), fn_710000ad9c(), 0xb0, -1, -1);
    field3->attrs.Add<base::global::CFilePathStrId>(fn_7100075240(), base::global::CFilePathStrId(g_unk_7101d470e8));
    UnkReflField* field4 = fn_7100096524(cls, CStrId("fMainExplosionScale", true), fn_710000abbc(), 0xb8, -1, -1);
    field4->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field5 = fn_7100096524(cls, CStrId("vRandomScale", true), fn_7100024504(), 0xbc, -1, -1);
    field5->attrs.Add<base::math::CVector2D>(fn_7100075240(), base::math::CVector2D{g_unk_7101647788.x, g_unk_7101647788.y});
    UnkReflField* field6 = fn_7100096524(cls, CStrId("uAmount", true), fn_7100005178(), 0xd0, -1, -1);
    field6->attrs.Add<u32>(fn_7100075240(), 0x1u);
    UnkReflField* field7 = fn_7100096524(cls, CStrId("fRadius", true), fn_710000abbc(), 0xc4, -1, -1);
    field7->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field8 = fn_7100096524(cls, CStrId("fRadiusAmplitude", true), fn_710000abbc(), 0xc8, -1, -1);
    field8->attrs.Add<f32>(fn_7100075240(), 0.0f);
    field8->attrs.Add<f32>(fn_7100091080(), 0.0f);
    field8->attrs.Add<f32>(fn_7100091190(), 1.0f);
    UnkReflField* field9 = fn_7100096524(cls, CStrId("fRandomArcAmplitude", true), fn_710000abbc(), 0xcc, -1, -1);
    field9->attrs.Add<f32>(fn_7100075240(), 0.0f);
    field9->attrs.Add<f32>(fn_7100091080(), 0.0f);
    field9->attrs.Add<f32>(fn_7100091190(), 1.0f);
    UnkReflField* field10 = fn_7100096524(cls, CStrId("vRandomDelay", true), fn_7100024504(), 0xd8, -1, -1);
    field10->attrs.Add<base::math::CVector2D>(fn_7100075240(), base::math::CVector2D{g_unk_7101647780.x, g_unk_7101647780.y});
    UnkReflField* field11 = fn_7100096524(cls, CStrId("oConstantColor0TransitionInit", true), fn_71000248c4(), 0x4c, -1, -1);
    field11->attrs.Add<base::math::CVector4D>(fn_7100075240(), base::math::CVector4D{g_unk_71016477c4.x, g_unk_71016477c4.y, g_unk_71016477c4.z, g_unk_71016477c4.w});
    UnkReflField* field12 = fn_7100096524(cls, CStrId("oConstantColor0TransitionEnd", true), fn_71000248c4(), 0x5c, -1, -1);
    field12->attrs.Add<base::math::CVector4D>(fn_7100075240(), base::math::CVector4D{g_unk_71016477c4.x, g_unk_71016477c4.y, g_unk_71016477c4.z, g_unk_71016477c4.w});
    UnkReflField* field13 = fn_7100096524(cls, CStrId("eConstant0TransitionMode", true), &g_unk_71727be388, 0x94, -1, -1);
    field13->attrs.Add<s32>(fn_7100075240(), &g_unk_71727be388, 0);
    UnkReflField* field14 = fn_7100096524(cls, CStrId("fConstantColor0TransitionTime", true), fn_710000abbc(), 0x8c, -1, -1);
    field14->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field15 = fn_7100096524(cls, CStrId("oConstantColor1TransitionInit", true), fn_71000248c4(), 0x6c, -1, -1);
    field15->attrs.Add<base::math::CVector4D>(fn_7100075240(), base::math::CVector4D{g_unk_71016477c4.x, g_unk_71016477c4.y, g_unk_71016477c4.z, g_unk_71016477c4.w});
    UnkReflField* field16 = fn_7100096524(cls, CStrId("oConstantColor1TransitionEnd", true), fn_71000248c4(), 0x7c, -1, -1);
    field16->attrs.Add<base::math::CVector4D>(fn_7100075240(), base::math::CVector4D{g_unk_71016477c4.x, g_unk_71016477c4.y, g_unk_71016477c4.z, g_unk_71016477c4.w});
    UnkReflField* field17 = fn_7100096524(cls, CStrId("eConstant1TransitionMode", true), &g_unk_71727be388, 0x98, -1, -1);
    field17->attrs.Add<s32>(fn_7100075240(), &g_unk_71727be388, 0);
    UnkReflField* field18 = fn_7100096524(cls, CStrId("fConstantColor1TransitionTime", true), fn_710000abbc(), 0x90, -1, -1);
    field18->attrs.Add<f32>(fn_7100075240(), 0.0f);
}
