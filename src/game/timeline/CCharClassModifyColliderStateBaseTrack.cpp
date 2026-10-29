// From unit 2452 of tools/tu_map.py (0x7100d1bb60-0x7100d211e0, CCharClassModifyColliderStateBaseTrack by reflection): the track's
// list of ids, read from one comma-separated string id. Names and layouts are placeholders.
// Holds fn_7100d1f764.
#include "game/UnkStrIdSplit.h"

struct UnkLeftoverArg {};

// Replaces `out` with the ids of the comma-separated parts of `name` (the first argument is not read).
// NON_MATCHING(minor): the empty-id loop's counter is loaded sign-extended (the original zero-extends
// it), and its entry test is `count != i + 1` where the original tests `count <= i`; the split helper
// only runs it when the count is at most i, so the two agree.
void fn_7100d1f764(UnkLeftoverArg, const base::global::CStrId* name, base::UnkStrIdVec* out) {
    UnkSplitStrIds(name, out);
}
