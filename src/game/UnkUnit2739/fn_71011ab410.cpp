// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in unit 2739 of
// tools/tu_map.py (0x71011a7f34-0x71011abbd0, no reflected class), with the sonar's layout and its own
// resource type; and the unit's append to its list of pointers (fn_71011a9bb4).
#include "base/UnkSList.h"
#include "base/UnkResKey.h"

bool fn_71011ab410(UnkResHolder* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e453f78>(self, path);
}

// The unit's object as the append below sees it: a list of pointers at +0xc0 (base/UnkSList.h).
struct UnkUnit2739ListTag;
struct UnkUnit2739ListOwner {
    u8 pad_0x0[0xc0];
    base::UnkSList<UnkUnit2739ListTag> list;
};

// Appends `item` to the list; nothing for null.
void fn_71011a9bb4(UnkUnit2739ListOwner* self, void* item) {
    if (item)
        self->list.PushBack(reinterpret_cast<u64>(item));
}
