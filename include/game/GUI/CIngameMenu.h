#pragma once

// CIngameMenu: functions of this translation unit that other files call (tools/tu_map.py unit 1681,
// 0x7100b2c060-0x7100b31c74). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkIngameMenu;
struct UnkStrIdHandle;
struct UnkStateOwner;
void fn_7100b2eaac(UnkStateOwner*, s32, bool, bool);
void fn_7100b30454(UnkIngameMenu* menu, const UnkStrIdHandle& handle);
