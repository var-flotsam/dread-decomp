// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eed85c makes the sub-area
// named by an id the current one (unless it already is and the first flag is clear): it looks the id up in the
// array at +0x860 by the pooled string's hash and hands the one found to fn_7100ee4f84; with none found there
// is no current sub-area. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CStrId.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

using base::global::CStrId;

struct CSubAreaManager;
struct UnkSubArea;
void fn_7100ee4f84(CSubAreaManager* self, UnkSubArea* area, bool force, bool flag);

// The pooled string a CStrId points to, as far as its hash at +0x30.
struct UnkStrIdData_leed85c {
    u8 pad_0x0[0x30];
    u64 hash;
};
static inline u64 HashOf(const CStrId& id) { return reinterpret_cast<const UnkStrIdData_leed85c*>(id.data)->hash; }

struct UnkSubAreaEntry_leed85c {
    CStrId id;
    u8 pad_0x8[0x180];
};
static_assert(sizeof(UnkSubAreaEntry_leed85c) == 0x188, "layout");

struct UnkSubAreaMgr_leed85c {
    u8 pad_0x0[0x3f0];
    UnkSubAreaEntry_leed85c* current;  // 0x3f0
    u8 pad_0x3f8[0x468];
    UnkSubAreaEntry_leed85c* entries;  // 0x860
    u32 count;                         // 0x868
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_leed85c, entries) == 0x860, "layout");

void fn_7100eed85c(UnkSubAreaMgr_leed85c* self, const CStrId* id, bool force, bool flag) {
    const CStrId& current = self->current ? self->current->id : g_unk_7101d470e0;
    if (current.data == id->data && !force)
        return;
    UnkIter<UnkSubAreaEntry_leed85c> first{self->entries};
    UnkIter<UnkSubAreaEntry_leed85c> last{self->entries + self->count};
    UnkSubAreaEntry_leed85c* found =
        std::find_if(first, last, [id](const UnkSubAreaEntry_leed85c& e) { return HashOf(e.id) == HashOf(*id); }).p;
    if (self->entries + self->count == found) {
        self->current = nullptr;
        return;
    }
    fn_7100ee4f84(reinterpret_cast<CSubAreaManager*>(self), reinterpret_cast<UnkSubArea*>(found), force, flag);
}
