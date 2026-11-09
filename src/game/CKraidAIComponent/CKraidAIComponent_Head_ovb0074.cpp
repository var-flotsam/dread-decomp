// CKraidAIComponent's unit (tools/tu_map.py): whether the target's helper at +0x208 reports
// fn_71009e6434, after making sure the unit's "Head" string id (a guarded static) exists. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"
using base::global::CStrId;
struct UnkHelper;
bool fn_71009e6434(UnkHelper* helper);
struct UnkKraidTarget_ovb0074 {
    u8 pad_0x0[0x208];
    UnkHelper* helper;  // 0x208
};
bool fn_71005fd088(void*, void*, UnkKraidTarget_ovb0074* target) {
    static CStrId s_head("Head", true);
    if (target->helper && fn_71009e6434(target->helper))
        return true;
    return false;
}
