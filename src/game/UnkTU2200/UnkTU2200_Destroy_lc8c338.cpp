// Unit 2200 (hint CCutSceneDef): the destructor of the record fn_7100c8c7b4 assigns: frees its four
// arrays (path entries, path ids, shots, takes), last first, then releases its string and path ids.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCutsceneRecord_lc8c338.h"
#include "game/UnkTU2200.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

UnkRecord_lc8c338::~UnkRecord_lc8c338() {
    entries.Free();
    paths.Free();
    shots.Free();
    takes.Free();
}
