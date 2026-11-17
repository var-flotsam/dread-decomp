// GUI::CHUD (unit 2751): the unit's static initialiser. Builds the unit header, registers the HUD's
// reflected classes, the animation vector's iterator and vector types and EForceVisibilityState,
// then builds two parameter blocks and a handle. Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/UnkAtExit.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CType;
using namespace unk_static_init;
#include "base/reflection/TypeObject.h"
namespace GUI::CHUD { enum class EForceVisibilityState : u32; }
template <> base::reflection::CType base::reflection::TypeObject<GUI::CHUD::EForceVisibilityState>::object asm("g_unk_7172a0f690");

static inline UnkReflType* AsUnk(const CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<CType*>(t));
}

// The unit's data: the header, two values, then the classes the getters build in place.
struct UnkStatics_l1e88e0 {
    HeaderIds2 ids;
    u8 pad_0x20[0x40];
    f32 field_0x60[2];
    bool field_0x68;
    u8 pad_0x69[0x2f];
    u8 chud_class[0x138];                // 0x98
    u8 resource_animation_class[0x138];  // 0x1d0
    u8 missile_animation_class[0x138];   // 0x308
    u8 bomb_animation_class[0x138];      // 0x440
    u8 energy_animation_class[0x138];    // 0x578
};
static_assert(__builtin_offsetof(UnkStatics_l1e88e0, energy_animation_class) == 0x578, "UnkStatics_l1e88e0 layout");
extern UnkStatics_l1e88e0 g_unk_7172a0ffd0 __attribute__((visibility("hidden")));

// A parameter block of the HUD: a name, three scales and a text.
struct UnkHudParams_l1e88e0 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    CRntString name;  // 0x18
    u32 field_0x48;
    u8 pad_0x4c[4];
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    UnkBufferString<0x20, g_unk_7101a1b5a0> text;  // 0x60
    u32 field_0xc8;
    u8 pad_0xcc[4];
    u32 field_0xd0;
    u16 field_0xd4;

    UnkHudParams_l1e88e0()
        : field_0x0(0), field_0x8(0), field_0x10(0), name(g_unk_710162e2ed), field_0x48(0), field_0x50(1.0f),
          field_0x54(1.0f), field_0x58(1.0f), field_0xc8(0), field_0xd0(0), field_0xd4(1) {}
};
static_assert(__builtin_offsetof(UnkHudParams_l1e88e0, field_0xd4) == 0xd4, "UnkHudParams_l1e88e0 layout");

struct UnkHudHandle_l1e88e0 {
    u64 field_0x0;
    s64 field_0x8;
};

extern char g_unk_7172a0f288[];
extern char g_unk_7172a0f289[];
extern char g_unk_7172a0f28a[];
extern char g_unk_7172a0f28b[];
extern char g_unk_7172a0f28c[];
extern char g_unk_7172a0f28d[];
extern char g_unk_7172a0f28e[];
extern char g_unk_7172a0f28f[];
extern char g_unk_7172a0f290[];
extern char g_unk_7172a0f3d0[];
extern char g_unk_7172a0f3d1[];
extern char g_unk_7172a0f510[];
extern char g_unk_7172a0f511[];
extern char g_unk_7172a0f688[];
extern char g_unk_7172a0f7a8[];
extern CType g_unk_7172a0f298;
extern CType g_unk_7172a0f3d8;
extern CType g_unk_7172a0f518;
extern UnkHudParams_l1e88e0 g_unk_7172a0f7b0;
extern UnkHudParams_l1e88e0 g_unk_7172a0f888;
extern UnkHudHandle_l1e88e0 g_unk_7172a0f960;

UnkReflType* fn_71011ddccc();
UnkReflType* fn_71011ddda8();
UnkReflType* fn_71011dde84();
UnkReflType* fn_71011ddf60();
UnkReflType* fn_71011de544();
UnkReflType* fn_71011deb28();
void fn_71011de3c0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011de03c();
void fn_71011de9a4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011de620();
void fn_71011dec04(UnkReflType* self, const CStrId& name, void* a, void* b);
void fn_71011deff0(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71011dede4();
void fn_71011df170(void* self);  // UnkHudParams_l1e88e0's destructor, as registered

void fn_71011e88e0() {
    InitHeaderIds(&g_unk_7172a0ffd0.ids);
    g_unk_7172a0ffd0.field_0x60[0] = -1.0f;
    g_unk_7172a0ffd0.field_0x60[1] = -1.0f;
    g_unk_7172a0ffd0.field_0x68 = true;

    fn_71011dce90();
    fn_710009ec00(g_unk_7172a0f288, g_unk_7172a0ffd0.chud_class);
    fn_71011dd304();
    fn_710009ec00(g_unk_7172a0f289, g_unk_7172a0ffd0.resource_animation_class);
    fn_71011dd568();
    fn_710009ec00(g_unk_7172a0f28a, g_unk_7172a0ffd0.missile_animation_class);
    fn_71011dd7e0();
    fn_710009ec00(g_unk_7172a0f28b, g_unk_7172a0ffd0.bomb_animation_class);
    fn_71011dda58();
    fn_710009ec00(g_unk_7172a0f28c, g_unk_7172a0ffd0.energy_animation_class);
    fn_710009ec00(g_unk_7172a0f28d, fn_71011ddccc());
    fn_710009ec00(g_unk_7172a0f28e, fn_71011ddda8());
    fn_710009ec00(g_unk_7172a0f28f, fn_71011dde84());
    fn_710009ec00(g_unk_7172a0f290, fn_71011ddf60());

    InitClass(AsUnk(&g_unk_7172a0f298),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<GUI::CHUD::"
              "SResourceAnimation>>>::TKeyElementIterator::value_type",
              fn_71011de3c0, fn_71011de03c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7172a0f3d0, &g_unk_7172a0f298);
    fn_710009ec00(g_unk_7172a0f3d1, fn_71011de544());
    InitClass(AsUnk(&g_unk_7172a0f3d8),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<GUI::CHUD::"
              "SResourceAnimation>>>::TKeyElementConstIterator::value_type",
              fn_71011de9a4, fn_71011de620, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7172a0f510, &g_unk_7172a0f3d8);
    fn_710009ec00(g_unk_7172a0f511, fn_71011deb28());
    InitVector(AsUnk(&g_unk_7172a0f518), "base::global::CRntVector<std::unique_ptr<GUI::CHUD::SResourceAnimation>>",
               fn_71011dec04);
    fn_710009ec00(g_unk_7172a0f688, &g_unk_7172a0f518);
    InitEnum(AsUnk(&base::reflection::TypeObject<GUI::CHUD::EForceVisibilityState>::object), "GUI::CHUD::EForceVisibilityState", fn_71011deff0, fn_71011dede4);
    fn_710009ec00(g_unk_7172a0f7a8, &base::reflection::TypeObject<GUI::CHUD::EForceVisibilityState>::object);

    new (&g_unk_7172a0f7b0) UnkHudParams_l1e88e0();
    fn_7100000250(fn_71011df170, &g_unk_7172a0f7b0, g_unk_7101a19000);
    new (&g_unk_7172a0f888) UnkHudParams_l1e88e0();
    fn_7100000250(fn_71011df170, &g_unk_7172a0f888, g_unk_7101a19000);
    g_unk_7172a0f960 = {0, -1};
}
