#pragma once

// UnkTU2772: functions of this translation unit that other files call (tools/tu_map.py unit 2772,
// 0x71012063dc-0x710123ac10). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkTaskManager;
struct UnkAtkArchive;
struct UnkAtkArchive_b0148;
struct UnkAtkEntryIndex_b0148;
struct UnkAtkInfo_b0150;
struct UnkAtkItemRefs;
struct UnkAtkKeyedNode_b49;
struct UnkAtkListOwner;
struct UnkAtkLoader;
struct UnkAtkLoaderObject_b0148;
struct UnkAtkLock;
struct UnkAtkPlayer98;
struct UnkAtkVoice;
struct UnkAtkWave_b0150;
class UnkEffect_b0184;
struct UnkListItem;
struct UnkListOwner;
int fn_71012395a0();
u32 fn_71012069f8(UnkAtkArchive* archive, u32 id);
void* fn_7101211d30(UnkAtkLoader* self, u32 file_id, void* heap, void* arg, u32 unk);
void fn_7101206678(UnkAtkPlayer98* player, bool on, s32 frames);
bool fn_7101206ad8(UnkAtkArchive_b0148* archive, UnkAtkEntryIndex_b0148* out, s32 key);
bool fn_7101206b4c(UnkAtkArchive* archive, UnkAtkItemRefs* refs, u32 id);
bool fn_7101206e60(UnkAtkArchive* archive, u32* out, u32 id);
bool fn_7101206ed4(UnkAtkArchive* archive, u32 id, UnkAtkInfo_b0150* out);
bool fn_710120b310(UnkAtkKeyedNode_b49* node, const void* key);
void fn_710120e658(UnkEffect_b0184* effect, f32 value);
void fn_710120e660(UnkEffect_b0184* effect, f32 value, u32 index);
void fn_710121279c(UnkAtkLoaderObject_b0148* self, u32 id, void* arg);
u32 fn_7101212f5c(UnkAtkInfo_b0150* info);
void fn_7101212f94(UnkAtkWave_b0150* w, void* data, u32 mode);
void fn_7101213168(UnkAtkInfo_b0150* info);
u64 fn_71012131a0(UnkAtkWave_b0150* w, u32 index);
u64 fn_7101213294(UnkAtkWave_b0150* w, u32 index, void* data);
void fn_710122ae00(void* owner, void* object);
void fn_710122b868(void* item);
void fn_710122c43c(UnkListItem* item, u32 state);
void fn_710122c958(UnkListOwner* self, UnkListItem* item);
void fn_710122cf1c(UnkAtkListOwner* owner);
void fn_710122faa0(UnkAtkVoice* self, void* params, u32 arg);
[[noreturn]] void fn_71012390d0(const char* a, const char* b, const char* c, u32 line, const u32* result, const char* format);
void fn_71012392d0(UnkAtkLock* lock);
void fn_71012392e0(UnkAtkLock* lock);
void fn_710120b198(void* heap, char* begin, u64 size);
void fn_710120e5d0(void* sound, f32 value, u32 x1);
void* fn_710120806c(void* player, u32 id);
void fn_7101233f2c(void* effect, u32 value);
void fn_7101234588(void* effect, u32 value);
void fn_7101234dcc(void* effect, u32 value);
u64 fn_7101214040(UnkAtkTaskManager* self);
