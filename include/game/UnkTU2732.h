#pragma once

// UnkTU2732: functions of this translation unit that other files call (tools/tu_map.py unit 2732,
// 0x710119ad3c-0x710119bc0c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"
void fn_710119aee8(void* self);
void fn_710119ae24(void* self, void* item, bool flag);
void fn_710119ad3c(void* self);
struct UnkItemListTag;
struct UnkListedItem;
struct UnkListedItem_710119ba7c;
using UnkItemList = base::UnkSList<UnkItemListTag>;  // the list fn_710119ba7c appends to
void fn_710119ba7c(UnkItemList* self, UnkListedItem_710119ba7c* item);
