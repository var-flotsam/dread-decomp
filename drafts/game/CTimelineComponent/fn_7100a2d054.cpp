// CTimelineComponent unit: erases one entry of the keyed track table (values are unique_ptrs with a
// std::function deleter, keys in a parallel array), moving the later ones down. Names and layouts
// are placeholders.
#include <functional>
#include <memory>
#include <new>
#include <utility>

#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineTrack_ovb0089;
using UnkTimelineTrackPtr_ovb0089 =
    std::unique_ptr<UnkTimelineTrack_ovb0089, std::function<void(UnkTimelineTrack_ovb0089*)>>;

static inline UnkTimelineTrackPtr_ovb0089* Tracks(UnkKeyedTable* table) {
    return reinterpret_cast<UnkTimelineTrackPtr_ovb0089*>(table->values);
}

void fn_7100a2d054(UnkKeyedTable* table, UnkTableEntry* entry) {
    u32 index = reinterpret_cast<UnkTimelineTrackPtr_ovb0089*>(entry) - Tracks(table);
    Tracks(table)[index].~UnkTimelineTrackPtr_ovb0089();
    u32 count = table->count;
    if (count > 1 && index != count - 1) {
        UnkTimelineTrackPtr_ovb0089* tracks = Tracks(table);
        for (u32 i = index; i < table->count - 1; ++i) {
            UnkTimelineTrackPtr_ovb0089* next = &tracks[i + 1];
            new (next - 1) UnkTimelineTrackPtr_ovb0089(std::move(*next));
            next->~UnkTimelineTrackPtr_ovb0089();
        }
        u64* keys = table->keys;
        for (u32 i = index; i < table->count - 1; ++i)
            keys[i] = keys[i + 1];
    }
    --table->count;
}
