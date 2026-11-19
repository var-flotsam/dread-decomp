#pragma once

// UnkTU2560: functions of this translation unit that other files call (tools/tu_map.py unit 2560,
// 0x7100e1bc3c-0x7100e1dbbc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2560Record;
struct UnkDropRequest_ovb0208;
struct UnkDropTarget_ovb0208;
struct UnkBillboardEntry_bbc;
struct UnkRecord_b218;
namespace base::global {
class CStrId;
}
// Registers a billboard record under `key` with the billboard manager.
void fn_7100e1cfec(void* self, base::global::CStrId* key, UnkRecord_b218* record);
void fn_7100e1bca4(void* self);
void* fn_7100e1cb50(void* manager, u64* key, void* arg);
UnkBillboardEntry_bbc* fn_7100e1caf0(void* manager, u32 id);
void fn_7100e1ceb0(void* manager, const u64* key);
void fn_7100e1cd34(void* self, UnkDropTarget_ovb0208* target, UnkDropRequest_ovb0208* request);
void fn_7100e1d538(UnkUnit2560Record* self);
