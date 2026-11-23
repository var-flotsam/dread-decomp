// CRotationalPlatformComponent's static initialiser: the unit header, the ERotationDirection enum
// with its converters to and from int, the component's id, one bool property, the class's
// registration and six collider and state names.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CRotationalPlatformComponent.h"
#include "game/UnkRotationalPlatformTU.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "base/reflection/TypeObject.h"

enum class ERotationDirection : u32;
template <> base::reflection::CType base::reflection::TypeObject<ERotationDirection>::object asm("g_unk_7172740788");
void fn_71009d7c0c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71009d7ac0(void* e);  // src/clones/clones_03.cpp
extern char g_unk_71727408a0;
extern UnkConverter g_unk_71727408a8;
extern UnkConverter g_unk_71727408c8;
void fn_71009d8fe4() __attribute__((visibility("hidden")));
void fn_71009d8ff0() __attribute__((visibility("hidden")));
extern CStrId g_unk_71727408e8;
extern char g_unk_7101a89e08[];
void fn_71009d93f4(UnkProperty* p);
void fn_71009d7d8c(void* p);
extern char g_unk_7172740918;

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_71009d9650() {
    UnkRotationalPlatformTU& tu = g_unk_7172740970;
    InitHeader(tu.header);
    InitEnum(AsUnk(&base::reflection::TypeObject<ERotationDirection>::object), "ERotationDirection", fn_71009d7c0c, reinterpret_cast<void (*)()>(fn_71009d7ac0));
    fn_710009ec00(&g_unk_71727408a0, AsUnk(&base::reflection::TypeObject<ERotationDirection>::object));
    InitEnumConverters(AsUnk(&base::reflection::TypeObject<ERotationDirection>::object), g_unk_71727408a8, fn_71009d8fe4, g_unk_71727408c8, fn_71009d8ff0);
    new (&g_unk_71727408e8) CStrId("ROTATIONAL", true);
    fn_7100000250(CStrId_Destroy, &g_unk_71727408e8, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_71727408f0, reinterpret_cast<void**>(g_unk_7101a89e08), g_unk_71726db788, "Used",
                            refl_TypeOf<fn_710000d228>(), fn_71009d93f4, fn_71009d7d8c);
    fn_71009d7e08();
    fn_710009ec00(&g_unk_7172740918, &tu.type);
    new (&tu.player_detector_right) CStrId("player_detector_right", true);
    fn_7100000250(CStrId_Destroy, &tu.player_detector_right, &g_unk_7101a19000);
    new (&tu.player_detector_left) CStrId("player_detector_left", true);
    fn_7100000250(CStrId_Destroy, &tu.player_detector_left, &g_unk_7101a19000);
    new (&tu.collision_opened) CStrId("collision_opened", true);
    fn_7100000250(CStrId_Destroy, &tu.collision_opened, &g_unk_7101a19000);
    new (&tu.collision_closed) CStrId("collision_closed", true);
    fn_7100000250(CStrId_Destroy, &tu.collision_closed, &g_unk_7101a19000);
    new (&tu.initial) CStrId("initial", true);
    fn_7100000250(CStrId_Destroy, &tu.initial, &g_unk_7101a19000);
    new (&tu.finished) CStrId("finished", true);
    fn_7100000250(CStrId_Destroy, &tu.finished, &g_unk_7101a19000);
}
