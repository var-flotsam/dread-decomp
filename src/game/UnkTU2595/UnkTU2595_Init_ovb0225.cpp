// The static initialiser of unit 2595 (helpers in game/UnkTUInit.h): the unit's header data with two scales and
// a flag after it, the enum CPrologueMode::SState with its converters to and from the integer type, and the
// string "SamusStory". Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2595.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

struct UnkTUHeaderEx_b0225 : UnkTUHeader {
    f32 scale_a;   // 0x18
    f32 scale_b;   // 0x1c
    bool flag;     // 0x20
};

extern UnkTUHeaderEx_b0225 g_unk_71729dc218 __attribute__((visibility("hidden")));
namespace CPrologueMode { enum class SState : u32; }
template <> UnkReflType base::reflection::TypeObject<CPrologueMode::SState, UnkReflType>::object asm("g_unk_71729dc0b0");
extern char g_unk_71729dc1c8;
extern UnkConverter g_unk_71729dc1d0;
extern UnkConverter g_unk_71729dc1f0;
void fn_7100f12f8c(void* e);
void fn_7100f13198(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100f1457c() __attribute__((visibility("hidden")));
void fn_7100f14588() __attribute__((visibility("hidden")));

void fn_7100f149a0() {
    InitHeader(g_unk_71729dc218);
    g_unk_71729dc218.scale_a = -1.0f;
    g_unk_71729dc218.scale_b = -1.0f;
    g_unk_71729dc218.flag = true;
    InitEnum(&base::reflection::TypeObject<CPrologueMode::SState, UnkReflType>::object, "CPrologueMode::SState", fn_7100f13198, reinterpret_cast<void (*)()>(fn_7100f12f8c));
    fn_710009ec00(&g_unk_71729dc1c8, &base::reflection::TypeObject<CPrologueMode::SState, UnkReflType>::object);
    InitEnumConverters(&base::reflection::TypeObject<CPrologueMode::SState, UnkReflType>::object, g_unk_71729dc1d0, fn_7100f1457c, g_unk_71729dc1f0, fn_7100f14588);
    new (&g_unk_71729dc240) base::global::CRntString("SamusStory");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(CRntString_Destroy), &g_unk_71729dc240, &g_unk_7101a19000);
}
