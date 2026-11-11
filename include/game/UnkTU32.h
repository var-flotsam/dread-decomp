#pragma once

// UnkTU32: functions of this translation unit that other files call (tools/tu_map.py unit 32,
// 0x71000255c4-0x7100027774). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkLinkTarget127_b66a1;
struct UnkStrIdPathRecord_b48;
struct UnkStrIdPathResource;
void fn_71000255c4(void* member);
void fn_7100025884(void* member, void* owner);
void fn_7100027118(UnkStrIdPathResource* self, UnkStrIdPathResource* other);
bool fn_710002589c(void* link, UnkLinkTarget127_b66a1* target, u32 mode, void* extra);
void fn_7100027464(UnkStrIdPathRecord_b48* self, const UnkStrIdPathRecord_b48* other);
void fn_710002572c(void* member);
void fn_7100025a98(void* record);
