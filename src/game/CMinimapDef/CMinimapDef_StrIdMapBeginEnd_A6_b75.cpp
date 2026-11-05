// Unit 2564 (tools/tu_map.py): begin/end slots of a CStrId-keyed flat map of 0xc8-byte records
// (destroyed by fn_7100e6c960). Shape in src/game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h.
#include "../CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"

void fn_7100e6c960(char*);

struct alignas(8) UnkMinimapRecordC8_A6_b75 {
    u8 data[0xc8];
    ~UnkMinimapRecordC8_A6_b75() { fn_7100e6c960(reinterpret_cast<char*>(this)); }
};

void fn_7100e70654(UnkStrIdMap_A6_b75<UnkMinimapRecordC8_A6_b75>* self,
                   UnkStrIdIterValue_A6_b75<UnkMinimapRecordC8_A6_b75>* out) {
    UnkStrIdMapBegin_A6_b75(self, out);
}

void fn_7100e70768(UnkStrIdMap_A6_b75<UnkMinimapRecordC8_A6_b75>* self,
                   UnkStrIdIterValue_A6_b75<UnkMinimapRecordC8_A6_b75>* out) {
    UnkStrIdMapEnd_A6_b75(self, out);
}
