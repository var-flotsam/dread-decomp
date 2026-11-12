#pragma once

// CBossAIComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 549,
// 0x71003ab520-0x71003b11d0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkModeFlags;
struct UnkMgr_0xf18;
struct UnkSettings_b0158;
struct UnkCoreXAI_A2_b73;
struct UnkNode;
struct UnkQuetzoaXAI_b58;
struct UnkQuetzoaXMessage_b58;
struct UnkZiplineLinkTarget_b49;
struct UnkZiplineLink_b49;
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49*);
void fn_71003ace0c(UnkNode*);
void fn_71003ac388(void*);  // Derived classes call it as their base's destructor.
void fn_71003acb5c(UnkCoreXAI_A2_b73*);
u32 fn_71003adaf8(UnkQuetzoaXAI_b58*, UnkQuetzoaXMessage_b58*);
extern u8* g_unk_71726bc558;
extern UnkSettings_b0158* g_unk_71726bc568;
extern UnkMgr_0xf18* g_unk_71726bc560;  // The music manager singleton.
extern UnkModeFlags* g_unk_71726bc570;
