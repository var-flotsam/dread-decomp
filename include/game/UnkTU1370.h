#pragma once

// UnkTU1370: functions of this translation unit that other files call (tools/tu_map.py unit 1370,
// 0x71008c04fc-0x71008c1b48). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkRequest;
struct UnkBase_71003cc474;
struct UnkGooplotRequest_b63a3;
struct UnkStruct_71008c0c74;
void fn_71008c1558(UnkBase_71003cc474* self);
void fn_71008c0c74(UnkStruct_71008c0c74* self);
void fn_71008c1090(UnkGooplotRequest_b63a3* self, const UnkGooplotRequest_b63a3* other);
void fn_71008c1538(UnkRequest* request, f32 value);
