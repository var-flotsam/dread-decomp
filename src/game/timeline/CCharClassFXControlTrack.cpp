// From unit 2366 of tools/tu_map.py (0x7100cfbc50-0x7100cfd340, CCharClassFXControlTrack by reflection): the track's
// list of ids, read from one comma-separated string id. Names and layouts are placeholders.
// Holds fn_7100cfca0c.
#include "game/UnkStrIdSplit.h"

struct UnkLeftoverArg {};

// Replaces `out` with the ids of the comma-separated parts of `name` (the first argument is not read).
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100cfca0c(UnkLeftoverArg, const base::global::CStrId* name, base::UnkStrIdVec* out) {
    UnkSplitStrIds(name, out);
}
