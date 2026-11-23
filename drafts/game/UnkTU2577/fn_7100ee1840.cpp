// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): sets the nine group
// prefixes (g_unk_71729da9f0: "lg_GLOBAL", "ssg_GLOBAL", "sg_SubArea_NO_SUBAREA_RUNTIME", "eg_GLOBAL",
// "bg_SubArea_" and four empty ones) and builds each group's name from its prefix and "_"
// (g_unk_71729daa38). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdBase;

extern UnkStrIdBase g_unk_71729da9f0[9];

// An id holding the pool's empty string (making the pool if there is none yet).
struct UnkEmptyStrId_ee1840 : CStrId {
    UnkEmptyStrId_ee1840() : CStrId(base::global::UnkStrIdInline(), nullptr, false) {}
};

// An id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_ee1840 {
    base::global::UnkStringData* data;

    explicit UnkBuiltStrId_ee1840(const CRntString& str) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), str, true);
    }
    ~UnkBuiltStrId_ee1840() { AsId().~CStrId(); }
    CStrId& AsId() { return *reinterpret_cast<CStrId*>(this); }
};

static inline CStrId& Prefix(u32 i) {
    return static_cast<CStrId&>(g_unk_71729da9f0[i]);
}

static inline CStrId& GroupName(u32 i) {
    return static_cast<CStrId&>(g_unk_71729daa38[i]);
}

void fn_7100ee1840() {
    Prefix(0) = UnkEmptyStrId_ee1840();
    Prefix(1) = CStrId("lg_GLOBAL", true);
    Prefix(2) = CStrId("ssg_GLOBAL", true);
    Prefix(3) = CStrId("sg_SubArea_NO_SUBAREA_RUNTIME", true);
    Prefix(4) = CStrId("eg_GLOBAL", true);
    Prefix(5) = CStrId("bg_SubArea_", true);
    Prefix(6) = UnkEmptyStrId_ee1840();
    Prefix(7) = UnkEmptyStrId_ee1840();
    Prefix(8) = UnkEmptyStrId_ee1840();

    for (u64 i = 0; i < 9; ++i) {
        static const CStrId s_separator("_", true);
        const CStrId& prefix = Prefix(i);
        ++prefix.data->reads;
        ++s_separator.data->reads;
        GroupName(i) = static_cast<CStrId&&>(
            UnkBuiltStrId_ee1840(CRntString_Concat(&prefix.data->String(), &s_separator.data->String())).AsId());
    }
}
