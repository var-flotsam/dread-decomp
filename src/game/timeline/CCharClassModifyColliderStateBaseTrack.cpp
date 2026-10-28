// From unit 2452 of tools/tu_map.py (0x7100d1bb60-0x7100d211e0, CCharClassModifyColliderStateBaseTrack by reflection): the track's
// list of ids, read from one comma-separated string id. Names and layouts are placeholders.
// Holds fn_7100d1f764.
#include "game/UnkStrIdSplit.h"

struct UnkLeftoverArg {};

// Replaces `out` with the ids of the comma-separated parts of `name` (the first argument is not read).
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100d1f764(UnkLeftoverArg, const base::global::CStrId* name, base::UnkStrIdVec* out) {
    UnkSplitStrIds(name, out);
}
