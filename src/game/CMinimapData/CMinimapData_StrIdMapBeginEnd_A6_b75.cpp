// Unit 2563 (tools/tu_map.py): begin/end slots of a CStrId-keyed flat map of 0x60-byte records
// (destroyed by fn_7100e4a1d0). Shape in UnkStrIdIterBeginEnd_A6_b75.h.
#include "UnkStrIdIterBeginEnd_A6_b75.h"

struct UnkRecord60;
void fn_7100e4a1d0(UnkRecord60*);

struct alignas(8) UnkMinimapRecord60_A6_b75 {
    u8 data[0x60];
    ~UnkMinimapRecord60_A6_b75() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }
};

void fn_7100e5a824(UnkStrIdMap_A6_b75<UnkMinimapRecord60_A6_b75>* self,
                   UnkStrIdIterValue_A6_b75<UnkMinimapRecord60_A6_b75>* out) {
    UnkStrIdMapBegin_A6_b75(self, out);
}

void fn_7100e5a938(UnkStrIdMap_A6_b75<UnkMinimapRecord60_A6_b75>* self,
                   UnkStrIdIterValue_A6_b75<UnkMinimapRecord60_A6_b75>* out) {
    UnkStrIdMapEnd_A6_b75(self, out);
}
