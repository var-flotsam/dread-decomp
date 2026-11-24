// base::res::CRes's unit (tools/tu_map.py): the kind of a resource by its path's extension (lookup
// textures, textures, models, particles, sounds), or `fallback` for any other path.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

// Whether the path contains the text of `part` (an empty path contains nothing).
static inline bool Contains(const CFilePathStrId& path, const CFilePathStrId& part) {
    ++path.data->reads;
    if (path.data == &g_unk_7101d47200->empty)
        return false;
    const base::global::CRntString* str = &path.data->String();
    ++part.data->reads;
    return CRntString_Contains(str, part.data->text ? part.data->text : g_unk_710162e2ed);
}

u32 CRes_GetKindFromPathExtension(void* self, const CFilePathStrId& path, u32 fallback) {
    static CFilePathStrId s_lookupTexture("_lookup.bctex", true);
    static CFilePathStrId s_texture(".bctex", true);
    static CFilePathStrId s_model(".bcmdl", true);
    static CFilePathStrId s_particle(".bcptl", true);
    static CFilePathStrId s_wave(".bcwav", true);
    static CFilePathStrId s_ogg(".ogg ", true);

    if (Contains(path, s_lookupTexture))
        return 0x43;
    if (Contains(path, s_texture))
        return 0x14;
    if (Contains(path, s_model))
        return 0x1e;
    if (Contains(path, s_particle))
        return 0x1a;
    if (Contains(path, s_wave))
        return 0xb;
    if (Contains(path, s_ogg))
        return 0xb;
    return fallback;
}
