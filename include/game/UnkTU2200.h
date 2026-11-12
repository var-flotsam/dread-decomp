#pragma once

// UnkTU2200: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2200,
// 0x7100c8137c-0x7100c932c4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/global/CStrId.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkFXPathEntry_b0068;
struct UnkReflType;
struct UnkTUHeader;
struct UnkElem40Array_b42a3;
struct UnkElemA0Array_b42a3;
void fn_7100c886c0(void*);  // The element's destructor.
void fn_7100c8a490(void*);  // The element's destructor.
void fn_7100c8b9c8(UnkElemA0Array_b42a3* self, u32 capacity);
void fn_7100c8398c();
void fn_7100c83f7c();
void fn_7100c84f5c();
void fn_7100c8554c();
void fn_7100c86aa4();
void fn_7100c87094();
void fn_7100c8b758(UnkElem40Array_b42a3* self, u32 capacity);
extern char g_unk_71727930c8;
extern char g_unk_71727930c9;
extern char g_unk_71727930ca;
extern char g_unk_71727930cb;
extern char g_unk_7172793208;
extern char g_unk_7172793209;
extern char g_unk_7172793348;
extern char g_unk_7172793349;
extern char g_unk_71727934c0;
extern char g_unk_71727934c1;
extern char g_unk_71727934c2;
extern char g_unk_71727934c3;
extern char g_unk_7172793600;
extern char g_unk_7172793601;
extern char g_unk_7172793740;
extern char g_unk_7172793741;
extern char g_unk_71727938b8;
extern char g_unk_71727938b9;
extern char g_unk_71727938ba;
extern char g_unk_71727938bb;
extern char g_unk_71727939f8;
extern char g_unk_71727939f9;
extern char g_unk_7172793b38;
extern char g_unk_7172793b39;
extern char g_unk_7172793cb0;
extern char g_unk_7172793cb1;
UnkReflType* fn_7100c837d4();
UnkReflType* fn_7100c838b0();
UnkReflType* fn_7100c83ea0();
UnkReflType* fn_7100c84490();
UnkReflType* fn_7100c84da4();
UnkReflType* fn_7100c84e80();
UnkReflType* fn_7100c85470();
UnkReflType* fn_7100c85a60();
UnkReflType* fn_7100c868ec();
UnkReflType* fn_7100c869c8();
UnkReflType* fn_7100c86fb8();
UnkReflType* fn_7100c875a8();
UnkReflType* fn_7100c8787c();
extern UnkReflType g_unk_71727930d0;
extern UnkReflType g_unk_7172793210;
extern UnkReflType g_unk_7172793350;
extern UnkReflType g_unk_71727934c8;
extern UnkReflType g_unk_7172793608;
extern UnkReflType g_unk_7172793748;
extern UnkReflType g_unk_71727938c0;
extern UnkReflType g_unk_7172793a00;
extern UnkReflType g_unk_7172793b40;
extern UnkTUHeader g_unk_7172794a98 __attribute__((visibility("hidden")));
extern const base::reflection::CType g_unk_7172794c08 __attribute__((visibility("hidden")));
void fn_7100c8456c(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100c85b3c(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100c87684(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100c83d1c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c8430c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c852ec(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c858dc(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c86e34(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c87424(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
// Grows the array to at least `capacity` entries (out of line).
void fn_7100c8c210(base::UnkVector<UnkFXPathEntry_b0068>* self, u32 capacity);
