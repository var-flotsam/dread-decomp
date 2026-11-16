// Unit 2647 (sound::CAudioPreset): its static initialiser. The unit header and two defaults, the reflected
// types of sound::CAudioMaterial and sound::CAudioPresetModifier, the preset's class, and the types of its
// array of materials by base::snd::EMaterial. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkAudioPresetTU {
    UnkTUHeader header;
    u8 pad_0x18[0x8];
    f32 field_0x20[2];
    bool field_0x28;
    u8 pad_0x29[0x7];
    UnkReflType preset_type;  // 0x30
};
static_assert(__builtin_offsetof(UnkAudioPresetTU, preset_type) == 0x30, "layout");

extern UnkAudioPresetTU g_unk_71729fb320 __attribute__((visibility("hidden")));

extern UnkReflType g_unk_71729f9fe8;
extern char g_unk_71729fa120;
extern char g_unk_71729fa121;
extern char g_unk_71729fa122;
void fn_710102cf4c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710102cba4();
UnkReflType* fn_710102d244();  // base::reflection::refl_GetType<sound::CAudioMaterial*>
UnkReflType* fn_710102d320();  // base::reflection::refl_GetType<sound::CAudioMaterial const*>

extern UnkReflType g_unk_71729fa128;
extern char g_unk_71729fa260;
extern char g_unk_71729fa261;
extern char g_unk_71729fa262;
void fn_710102d8e0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710102d3fc();
UnkReflType* fn_710102dd5c();  // base::reflection::refl_GetType<sound::CAudioPresetModifier*>
UnkReflType* fn_710102de38();  // base::reflection::refl_GetType<sound::CAudioPresetModifier const*>

extern char g_unk_71729fa263;
extern char g_unk_71729fa264;
extern char g_unk_71729fa265;
extern char g_unk_71729fa266;
extern char g_unk_71729fa267;
UnkReflType* fn_71010353bc();
UnkReflType* fn_7101035498();
UnkReflType* fn_7101035574();
UnkReflType* fn_7101035650();

extern UnkReflType g_unk_71729fa268;
extern char g_unk_71729fa3a0;
extern char g_unk_71729fa3a1;
void fn_7101035ab0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710103572c();
UnkReflType* fn_7101035c34();

extern UnkReflType g_unk_71729fa3a8;
extern char g_unk_71729fa4e0;
extern char g_unk_71729fa4e1;
void fn_7101036094(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7101035d10();
UnkReflType* fn_7101036218();

extern UnkReflType g_unk_71729fa4e8;
extern char g_unk_71729fa658;
extern char g_unk_71729fa659;
extern char g_unk_71729fa65a;
void fn_71010362f4(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_71010364ec();
UnkReflType* fn_71010365c8();

static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}

void fn_710103cb70() {
    UnkAudioPresetTU& tu = g_unk_71729fb320;
    InitHeader(tu.header);
    tu.field_0x20[0] = -1.0f;
    tu.field_0x20[1] = -1.0f;
    tu.field_0x28 = true;

    InitClass(&g_unk_71729f9fe8, "sound::CAudioMaterial", fn_710102cf4c, fn_710102cba4);
    fn_710009ec00(&g_unk_71729fa120, &g_unk_71729f9fe8);
    fn_710009ec00(&g_unk_71729fa121, fn_710102d244());
    fn_710009ec00(&g_unk_71729fa122, fn_710102d320());

    InitClass(&g_unk_71729fa128, "sound::CAudioPresetModifier", fn_710102d8e0, fn_710102d3fc);
    fn_710009ec00(&g_unk_71729fa260, &g_unk_71729fa128);
    fn_710009ec00(&g_unk_71729fa261, fn_710102dd5c());
    fn_710009ec00(&g_unk_71729fa262, fn_710102de38());

    fn_710102df18();
    fn_710009ec00(&g_unk_71729fa263, &tu.preset_type);
    fn_710009ec00(&g_unk_71729fa264, fn_71010353bc());
    fn_710009ec00(&g_unk_71729fa265, fn_7101035498());
    fn_710009ec00(&g_unk_71729fa266, fn_7101035574());
    fn_710009ec00(&g_unk_71729fa267, fn_7101035650());

    InitClass(&g_unk_71729fa268,
              "base::reflection::CollectionTypeMapper<base::global::CArray<sound::CAudioMaterial, "
              "EnumClass<base::snd::EMaterial>::Count, base::snd::EMaterial>>::TKeyElementIterator::value_type",
              fn_7101035ab0, fn_710103572c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fa3a0, &g_unk_71729fa268);
    fn_710009ec00(&g_unk_71729fa3a1, fn_7101035c34());

    InitClass(&g_unk_71729fa3a8,
              "base::reflection::CollectionTypeMapper<base::global::CArray<sound::CAudioMaterial, "
              "EnumClass<base::snd::EMaterial>::Count, base::snd::EMaterial>>::TKeyElementConstIterator::value_type",
              fn_7101036094, fn_7101035d10, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fa4e0, &g_unk_71729fa3a8);
    fn_710009ec00(&g_unk_71729fa4e1, fn_7101036218());

    InitVector(&g_unk_71729fa4e8,
               "base::global::CArray<sound::CAudioMaterial, EnumClass<base::snd::EMaterial>::Count, "
               "base::snd::EMaterial>",
               fn_71010362f4);
    fn_710009ec00(&g_unk_71729fa658, &g_unk_71729fa4e8);
    fn_710009ec00(&g_unk_71729fa659, fn_71010364ec());
    fn_710009ec00(&g_unk_71729fa65a, fn_71010365c8());
}
