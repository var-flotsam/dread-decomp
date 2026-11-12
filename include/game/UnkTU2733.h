#pragma once

// UnkTU2733: functions of this translation unit that other files call (tools/tu_map.py unit 2733,
// 0x710119bc0c-0x710119d484). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkDialogText;
struct UnkLanguage;
void fn_710119c020(UnkLanguage* language);
s32 fn_710119c9f4(UnkLanguage* language);
UnkDialogText fn_710119d040(UnkLanguage* language, const base::global::CStrId& key);  // The text the language gives.
