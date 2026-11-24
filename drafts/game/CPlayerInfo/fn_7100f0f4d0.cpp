// CPlayerInfo unit: forwards a copy of an (id, u32, u8) record to fn_7100f0e100 and reports false.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRecord {
    base::global::CStrId id;
    u32 field_0x8;
    u8 field_0xc;
};

struct UnkPlayerInfo;
void CPlayerInfo_RefreshHasItemFlagOnMessage(UnkPlayerInfo* self, const UnkRecord& record);

bool fn_7100f0f4d0(UnkPlayerInfo* self, const UnkRecord* in) {
    UnkRecord copy = *in;
    CPlayerInfo_RefreshHasItemFlagOnMessage(self, copy);
    return false;
}
