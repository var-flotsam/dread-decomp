// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in unit 2739 of
// tools/tu_map.py (0x71011a7f34-0x71011abbd0, no reflected class), with the sonar's layout and its own
// resource type.
#include "base/UnkResKey.h"

bool fn_71011ab410(UnkResHolder* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e453f78>(self, path);
}
