#pragma once

// CShape: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2518,
// 0x7100d6968c-0x7100d70514). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkPolygonCollectionShape;
struct UnkReflType;
struct UnkTUHeader;
struct UnkPolygonCollectionShape_b42a3;
struct UnkShape2D_b46a3;
void fn_7100d6c0e4(UnkPolygonCollectionShape_b42a3* self, const UnkPolygonCollectionShape_b42a3* other);
void fn_7100d6a2f0();
void fn_7100d6a8d4();
void fn_7100d6b240();
void fn_7100d6b824();
void fn_7100d6bfd8(void* self);  // The shape base's constructor.
void fn_7100d6c034(UnkShape2D_b46a3* self, const void* transform);
void fn_7100d6e22c();
extern char g_unk_71727b3dd8;
extern char g_unk_71727b3dd9;
extern char g_unk_71727b3dda;
extern char g_unk_71727b3ddb;
extern char g_unk_71727b3e00;
extern char g_unk_71727b3e01;
extern char g_unk_71727b3f40;
extern char g_unk_71727b3f41;
extern char g_unk_71727b4080;
extern char g_unk_71727b4081;
extern char g_unk_71727b41f8;
extern char g_unk_71727b41f9;
extern char g_unk_71727b41fa;
extern char g_unk_71727b4338;
extern char g_unk_71727b4339;
extern char g_unk_71727b4478;
extern char g_unk_71727b4479;
extern char g_unk_71727b45f0;
UnkReflType* fn_7100d6a05c();
UnkReflType* fn_7100d6a138();
UnkReflType* fn_7100d6a214();
UnkReflType* fn_7100d6a7f8();
UnkReflType* fn_7100d6addc();
UnkReflType* fn_7100d6b088();
UnkReflType* fn_7100d6b164();
UnkReflType* fn_7100d6b748();
UnkReflType* fn_7100d6bd2c();
extern UnkReflType g_unk_71727b3e08;
extern UnkReflType g_unk_71727b3f48;
extern UnkReflType g_unk_71727b4088;
extern UnkReflType g_unk_71727b4200;
extern UnkReflType g_unk_71727b4340;
extern UnkReflType g_unk_71727b4480;
extern UnkTUHeader g_unk_71727b4f88 __attribute__((visibility("hidden")));
bool fn_7100d6c288(UnkPolygonCollectionShape* self, base::global::CRntFile* file);
void fn_7100d6aeb8(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100d6be08(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
extern UnkConverter g_unk_71727b3de0;
