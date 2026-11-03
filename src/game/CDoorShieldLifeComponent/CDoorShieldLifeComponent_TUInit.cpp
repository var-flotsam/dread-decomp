// CDoorShieldLifeComponent's static initialiser: the unit header, the EColor and EDoorsShieldType
// enums with their converters to and from int, the class's registration and two collider names.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkDoorShieldLifeTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId collision_l;  // 0x20
    CStrId collision_r;  // 0x28
    u8 field_0x30[0x20];
    UnkReflType type;    // 0x50
};

extern UnkDoorShieldLifeTU g_unk_7172732030 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172731d68;  // also named in src/reflection/
void fn_71008dd720(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71008dd514(void* e);  // src/clones/clones_03.cpp
extern char g_unk_7172731e80;
extern UnkConverter g_unk_7172731e88;
extern UnkConverter g_unk_7172731ea8;
void fn_71008df380() __attribute__((visibility("hidden")));
void fn_71008df38c() __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172731ec8;  // also named in src/reflection/
void fn_71008ddb6c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71008dd8a0(void* e);  // src/clones/clones_03.cpp
extern char g_unk_7172731fe0;
extern UnkConverter g_unk_7172731fe8;
extern UnkConverter g_unk_7172732008;
void fn_71008df398() __attribute__((visibility("hidden")));
void fn_71008df3a4() __attribute__((visibility("hidden")));
extern char g_unk_7172732028;

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_71008dee60() {
    UnkDoorShieldLifeTU& tu = g_unk_7172732030;
    InitHeader(tu.header);
    InitEnum(AsUnk(&g_unk_7172731d68), "CDoorShieldLifeComponent::EColor", fn_71008dd720,
             reinterpret_cast<void (*)()>(fn_71008dd514));
    fn_710009ec00(&g_unk_7172731e80, AsUnk(&g_unk_7172731d68));
    InitEnumConverters(AsUnk(&g_unk_7172731d68), g_unk_7172731e88, fn_71008df380, g_unk_7172731ea8, fn_71008df38c);
    InitEnum(AsUnk(&g_unk_7172731ec8), "CDoorShieldLifeComponent::EDoorsShieldType", fn_71008ddb6c,
             reinterpret_cast<void (*)()>(fn_71008dd8a0));
    fn_710009ec00(&g_unk_7172731fe0, AsUnk(&g_unk_7172731ec8));
    InitEnumConverters(AsUnk(&g_unk_7172731ec8), g_unk_7172731fe8, fn_71008df398, g_unk_7172732008, fn_71008df3a4);
    fn_71008ddcf0();
    fn_710009ec00(&g_unk_7172732028, &tu.type);
    new (&tu.collision_l) CStrId("collision_L", true);
    fn_7100000250(fn_710000051c, &tu.collision_l, &g_unk_7101a19000);
    new (&tu.collision_r) CStrId("collision_R", true);
    fn_7100000250(fn_710000051c, &tu.collision_r, &g_unk_7101a19000);
}
