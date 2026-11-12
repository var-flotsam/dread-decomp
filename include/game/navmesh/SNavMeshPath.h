#pragma once

// SNavMeshPath: functions and globals of this translation unit that other files use (tools/tu_map.py unit 366,
// 0x71002ac2f4-0x71002af630). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkLinkList;
struct UnkNavMeshPath;
struct UnkObject_71002ad0dc;
void fn_71002acfe4(void* sub);
void fn_71002ad0dc(UnkObject_71002ad0dc* self);
void fn_71002adc8c(void* member);
void* fn_71002ad4f4(void* self, const void* other);
void* fn_71002ad420(void* self, const void* other);
void fn_71002ad978(UnkNavMeshPath* self, void* value);
void fn_71002adbb0(UnkNavMeshPath* self, void* value);
extern char g_unk_7101a21168[];
void fn_71002ae87c(void* set, void* item);
void fn_71002af070(UnkLinkList* list, UnkLinkList** slot);
