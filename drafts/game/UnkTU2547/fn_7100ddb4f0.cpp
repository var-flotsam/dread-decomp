// tools/tu_map.py unit 2547: the file path at an index of one of five tables, the table picked by the
// mode flags. Names and layouts are placeholders.
#include "base/UnkStaticStrId.h"
#include "base/global/CFilePathStrId.h"
#include "game/CBossAIComponent.h"
#include "types.h"

using base::UnkStaticPathId;
using base::global::CFilePathStrId;

// The mode flags' bytes this lookup reads (g_unk_71726bc570).
struct UnkModeFlags_ldb4f0 {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
    bool field_0xb9;
    bool field_0xba;
    bool field_0xbb;
};

extern UnkStaticPathId g_unk_71727be010[12];
extern UnkStaticPathId g_unk_71727be070[12];
extern UnkStaticPathId g_unk_71727be0d0[12];
extern UnkStaticPathId g_unk_71727be130[12];
extern UnkStaticPathId g_unk_71727be190[12];

// The id at an index of a static table, as a file-path id.
static inline const CFilePathStrId& PathAt(const UnkStaticPathId* table, u32 index) {
    return reinterpret_cast<const CFilePathStrId&>(table[index]);
}

CFilePathStrId fn_7100ddb4f0(u32 index) {
    CFilePathStrId path;
    const UnkModeFlags_ldb4f0* flags = reinterpret_cast<const UnkModeFlags_ldb4f0*>(g_unk_71726bc570);
    if (flags->field_0xb8 && flags->field_0xba) {
        if (flags->field_0xb9)
            path = PathAt(g_unk_71727be130, index);
        else
            path = PathAt(g_unk_71727be0d0, index);
    } else if (flags->field_0xb8 && flags->field_0xbb) {
        path = PathAt(g_unk_71727be190, index);
    } else if (flags->field_0xb8 && flags->field_0xb9) {
        path = PathAt(g_unk_71727be070, index);
    } else {
        path = PathAt(g_unk_71727be010, index);
    }
    return path;
}
