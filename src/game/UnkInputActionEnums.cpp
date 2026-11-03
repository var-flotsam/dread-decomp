// The static initialiser of the unit that reflects the enums EInputAction and EWalkInputMode: the
// unit header, both enum types, and EWalkInputMode's converters to and from int.
#include "base/reflection/CClass.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_7172729e40 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172729bc0;  // also named in src/reflection/
extern UnkReflType g_unk_7172729ce0;
void fn_71008751e4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100875570(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71008748b8(void* e);  // src/clones/clones_02.cpp
void fn_7100875364(void* e);  // src/clones/clones_02.cpp
extern char g_unk_7172729cd8;
extern char g_unk_7172729df8;
extern UnkConverter g_unk_7172729e00;
extern UnkConverter g_unk_7172729e20;
void fn_71008756f0() __attribute__((visibility("hidden")));
void fn_71008756fc() __attribute__((visibility("hidden")));

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_71008757f0() {
    InitHeader(g_unk_7172729e40);
    InitEnum(AsUnk(&g_unk_7172729bc0), "EInputAction", fn_71008751e4, reinterpret_cast<void (*)()>(fn_71008748b8));
    fn_710009ec00(&g_unk_7172729cd8, AsUnk(&g_unk_7172729bc0));
    InitEnum(&g_unk_7172729ce0, "EWalkInputMode", fn_7100875570, reinterpret_cast<void (*)()>(fn_7100875364));
    fn_710009ec00(&g_unk_7172729df8, &g_unk_7172729ce0);
    InitEnumConverters(&g_unk_7172729ce0, g_unk_7172729e00, fn_71008756f0, g_unk_7172729e20, fn_71008756fc);
}
