#pragma once

// UnkTU32: functions of this translation unit that other files call (tools/tu_map.py unit 32,
// 0x71000255c4-0x7100027774). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkIdPair;
struct UnkLinkTarget127_b66a1;
struct UnkStrIdPathRecord_b48;
struct UnkStrIdPathResource;
void fn_71000255c4(void* member);  // A member's constructor.
void fn_7100025884(void* member, void* owner);  // Registers the owner with the member.

// Assigns `other` to `self` (move assignment).
UnkStrIdPathResource* fn_7100027118(UnkStrIdPathResource* self, UnkStrIdPathResource* other);

bool fn_710002589c(void* link, UnkLinkTarget127_b66a1* target, u32 mode, void* extra);
void fn_7100027464(UnkStrIdPathRecord_b48* self, const UnkStrIdPathRecord_b48* other);  // Copy constructor.
void fn_710002572c(void* member);  // A member's destructor (shared by several classes).
void fn_7100025a98(void* record);
void fn_7100026f70(UnkIdPair* pair);  // An id pair's destructor: releases both ids.
