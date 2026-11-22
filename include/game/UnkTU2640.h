#pragma once

// UnkTU2640: functions of this translation unit that other files call (tools/tu_map.py unit 2640,
// 0x7100fe119c-0x7101007b10). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkElem78.h"
#include "types.h"

struct UnkIdObject_lfd9aa0;
struct UnkPartFfca00;

// Out-of-line destructor of a music preset map's values (src/game/sound/UnkMusicPresets.cpp).
void fn_7100fee034(void*);

void fn_7100ffaa88(void*);
void fn_7100fefde0(void*);
UnkPartFfca00* fn_7100ffbad8(UnkPartFfca00* self, const UnkPartFfca00* other);
void fn_7101005d8c(void*);
void fn_7100ff4350(char*);
void fn_7100ff7360(char*);
template <class Elem>
struct UnkElemArray;
struct UnkElem78;
void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);

namespace base::global {
class CStrId;
}
struct UnkMap_b0213;
struct UnkMapValue_b0213;
// Inserts a node holding a copy of `key` and the value moved out of `value` (src/game/UnkTU2640/fn_7101005fcc.cpp).
void fn_7101005fcc(UnkMap_b0213* self, const base::global::CStrId* key, UnkMapValue_b0213* value);
const base::global::CStrId* fn_7100fed648(UnkIdObject_lfd9aa0* object);
void fn_7101001694(UnkElemArray<UnkElem78>* self, const UnkElem78* value);
// Clears the array.
void fn_71010017f4(UnkElemArray<UnkElem78>* self);
