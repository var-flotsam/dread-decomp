// Random impact sounds next to CCharClassAIComponent's unit (tools/tu_map.py unit 616 and its
// neighbour): each getter builds three file-path ids once and returns one of them at random
// (fn_71012395a0 % 3), or the global empty path id when the draw falls outside. Names are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU2772.h"
#include "types.h"

using base::global::CFilePathStrId;

static inline const CFilePathStrId& PickOne(const CFilePathStrId& a, const CFilePathStrId& b, const CFilePathStrId& c) {
    switch (fn_71012395a0() % 3) {
    case 0:
        return a;
    case 1:
        return b;
    case 2:
        return c;
    }
    return g_unk_7101d470e8;
}

const CFilePathStrId& fn_7100414270() {
    static const CFilePathStrId s_0("weapons/singleshot_fleshimpact_01.wav", true);
    static const CFilePathStrId s_1("weapons/singleshot_fleshimpact_02.wav", true);
    static const CFilePathStrId s_2("weapons/singleshot_fleshimpact_03.wav", true);
    return PickOne(s_0, s_1, s_2);
}

const CFilePathStrId& fn_710041440c() {
    static const CFilePathStrId s_0("weapons/singleshot_shieldimpact_01.wav", true);
    static const CFilePathStrId s_1("weapons/singleshot_shieldimpact_02.wav", true);
    static const CFilePathStrId s_2("weapons/singleshot_shieldimpact_03.wav", true);
    return PickOne(s_0, s_1, s_2);
}

const CFilePathStrId& fn_710041b2a4() {
    static const CFilePathStrId s_0("weapons/singleshot_metalimpact_01.wav", true);
    static const CFilePathStrId s_1("weapons/singleshot_metalimpact_02.wav", true);
    static const CFilePathStrId s_2("weapons/singleshot_metalimpact_03.wav", true);
    return PickOne(s_0, s_1, s_2);
}
