#pragma once

// reflection: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2525,
// 0x7100d8c56c-0x7100d911d8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "base/reflection/CClass.h"
#include "base/global/CStrId.h"
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkEnvironmentManager;
struct UnkTypedObject;
struct UnkEnvironmentData;
struct UnkEnvironmentVisuals;
struct UnkUnit2525_b51;
struct UnkReflType;
struct UnkTUHeader;
struct UnkTUHeaderFns;
struct UnkValue;
struct UnkAssignedRecord_a1ba4;
struct UnkCompared_0x18;
struct UnkFormatArgs;
struct UnkItem;
struct UnkScene;
struct UnkSceneItem;
struct UnkTextBuffer200;
void fn_7100d8ced8(void* self);  // Constructor.
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);
bool fn_71000a1ed4(const UnkCompared_0x18* a, const UnkCompared_0x18* b);
void fn_7100d8e0a0(void* manager);

// Out-of-line destructor of a music preset map's values (src/game/sound/UnkMusicPresets.cpp).
void fn_7100d90304(void*);

void fn_71000a1784(UnkFormatArgs* args, UnkTextBuffer200* out);
void* fn_7100202574(UnkItem* item);
void fn_7100d8ef04(void* self, f32 dt);
void fn_7100d8f9e4(void*);
void fn_710009aae8(void* self);
void fn_71001eed18(void* self, void* item);
bool fn_71001efb44(UnkScene* scene, bool flag, const u64* id);
void fn_7100200138(UnkItem* item, bool value);
bool fn_7100200144(UnkItem* item);
void fn_710020014c(UnkItem* item, bool value);
void fn_7100200160(UnkSceneItem* item, f32 dt);
bool fn_7100202d24(UnkItem* item, void* arg);
void fn_7100adc470();
void* fn_7100adc4c0(void*, void*);
void fn_7100b03ecc();
void fn_7100b75ea8();
void fn_7100b76544();
void fn_7100b76b28();
void fn_7100b7d05c();
void fn_7100b7db14();
void fn_7100b7e0f8();
void fn_7100d8efd0(void* self, bool a, bool b, bool c);
extern u8 g_unk_7172757530[];
extern char g_unk_7172758254;
extern char g_unk_7172758255;
extern char g_unk_7172758256;
extern char g_unk_717275dfb8;
extern char g_unk_717275dfb9;
extern char g_unk_7172766268;
extern char g_unk_7172766269;
extern char g_unk_717276626a;
extern char g_unk_717276626b;
extern char g_unk_71727663a8;
extern char g_unk_71727663a9;
extern char g_unk_71727664e8;
extern char g_unk_71727664e9;
extern char g_unk_7172766660;
extern char g_unk_7172767138;
extern char g_unk_7172767139;
extern char g_unk_717276713a;
extern char g_unk_717276713b;
extern char g_unk_7172767278;
extern char g_unk_7172767279;
extern char g_unk_71727673b8;
extern char g_unk_71727673b9;
extern char g_unk_7172767530;
void fn_710025bda0(void* self);
UnkReflType* fn_710025d654();  // base::reflection::refl_GetType<gameeditor::CGameModelIndex>.
UnkReflType* fn_7100adc2b4();
UnkReflType* fn_7100adc390();
UnkReflType* fn_7100b040c0();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CTrackSet>>.
UnkReflType* fn_7100b042c4();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CDisplayObjectAnimationDef>>.
UnkReflType* fn_7100b762b0();  // base::reflection::refl_GetType<GUI::CSkin*>.
UnkReflType* fn_7100b7638c();
UnkReflType* fn_7100b76468();
UnkReflType* fn_7100b76a4c();
UnkReflType* fn_7100b77030();
UnkReflType* fn_7100b7d880();  // base::reflection::refl_GetType<GUI::CSpriteSheet*>.
UnkReflType* fn_7100b7d95c();
UnkReflType* fn_7100b7da38();
UnkReflType* fn_7100b7e01c();
UnkReflType* fn_7100b7e600();
UnkReflType* fn_7100b80034();  // base::reflection::refl_GetType<GUI::CSpriteSheetItem*>.
extern UnkReflType g_unk_71726a0628;
extern UnkValue g_unk_7172758298;
extern UnkTUHeaderFns g_unk_71727585d8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275de80;
extern UnkTUHeader g_unk_717275e1e0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172766130;
extern UnkReflType g_unk_7172766270;
extern UnkReflType g_unk_71727663b0;
extern UnkReflType g_unk_71727664f0;
extern UnkTUHeader g_unk_7172766bb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172767000;
extern UnkReflType g_unk_7172767140;
extern UnkReflType g_unk_7172767280;
extern UnkReflType g_unk_71727673c0;
extern UnkTUHeader g_unk_7172767b98 __attribute__((visibility("hidden")));
void fn_7100d8d854(UnkUnit2525_b51* self, void* resource);
void fn_7100d8db30(UnkUnit2525_b51* self, void* resource, bool flag);
void fn_7100d8e398(UnkEnvironmentVisuals* self, const UnkEnvironmentData* data, bool force);
void fn_71000a179c(base::reflection::UnkReflValue* self, const base::reflection::UnkReflValue* other);
extern base::reflection::CType g_unk_7172756e50;
void fn_7100d8e32c(UnkEnvironmentManager* self, const base::global::CStrId* id, bool a, bool b);
void fn_71000a174c(base::reflection::UnkTypedRef* out, const void* ref);
// A value converted to `type` (returned through x8).
base::reflection::UnkReflValue fn_71000a1774(const base::reflection::UnkReflValueRef* value, const base::reflection::CType* type);
const base::reflection::CType* fn_7100ad62d4();  // base::reflection::refl_GetType<CEnvironmentManager*>.
const base::reflection::CType* fn_7100ad6644();  // base::reflection::refl_GetType<sound::CMusicManager*>.
const base::global::CStrId* fn_7100d8efc8(UnkEnvironmentManager* self);
// Builds a reflected value for an object (by reference when by_ref is 1).
void fn_71000a1828(base::reflection::UnkReflValue* out, const UnkTypedObject* object, u32 by_ref, u32 kind);
// The value a typed reference holds, as a reflected value.
void fn_71000a1ae4(base::reflection::UnkReflValue* out, const base::reflection::UnkTypedRef* ref, u32 a, u32 b);
void fn_7100b7710c(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100b7e6dc(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
extern UnkConverter g_unk_7172758258;
extern UnkConverter g_unk_7172758278;
void fn_71000a1760(base::reflection::UnkTypedRef* out, const base::reflection::UnkTypedRef* in);
