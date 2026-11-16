// From unit 45 of tools/tu_map.py (named after nn::time; the timeline code of
// src/base/global/UnkTimelineTracks.cpp): whether a track list holds a track of the given class (+0x8
// of each track). Names and layouts are placeholders.
#include <algorithm>

#include "../../../game_s7/UnkIter.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkClassTrack_l06ba58 {
    void* vtable;
    const base::reflection::CClass* cls;
};

struct UnkClassTrackList_l06ba58 {
    UnkClassTrack_l06ba58** data;
    u32 count;
};

bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls) {
    UnkClassTrackList_l06ba58* list = static_cast<UnkClassTrackList_l06ba58*>(tracks);
    UnkIter<UnkClassTrack_l06ba58*> first = {list->data};
    UnkIter<UnkClassTrack_l06ba58*> last = {list->data + list->count};
    return std::find_if(first, last, [cls](UnkClassTrack_l06ba58* track) { return track->cls == cls; }) != last;
}
