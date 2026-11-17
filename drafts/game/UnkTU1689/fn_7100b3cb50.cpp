// Unit 1689 (map menu): rebuilds the list of marker ids from the map's entries of the eight marker types,
// keeping those not within 500 units of the player in the current area, sorted by x. Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkCloneRecords.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecordTable_le91018;
struct UnkKeyHolder_le91018;
void* fn_7100e91018(UnkRecordTable_le91018* self, UnkKeyHolder_le91018** keyRef, u32 index);
bool fn_7100eb7edc(char* p0);
void fn_71000ce300(base::UnkStrIdVec* self, const CStrId* value, u32 index);

struct UnkMapMarker_lb3cb50 {
    u8 pad_0x0[8];
    f32 x;  // 0x8
    f32 y;  // 0xc
};

struct UnkMapEntry_lb3cb50 {
    u8 pad_0x0[0x18];
    CStrId type;  // 0x18
    u8 pad_0x20[8];
};
static_assert(sizeof(UnkMapEntry_lb3cb50) == 0x28, "UnkMapEntry layout");

struct UnkMapResource_lb3cb50 : UnkCountedResource {
    u8 pad_0x58[0x6190 - 0x58];
    UnkMapEntry_lb3cb50* values;  // 0x6190
    CStrId* keys;  // 0x6198
    u32 count;  // 0x61a0
};
static_assert(__builtin_offsetof(UnkMapResource_lb3cb50, count) == 0x61a0, "UnkMapResource layout");

struct UnkMapState_lb3cb50 {
    u8 pad_0x0[0x68];
    UnkMapResource_lb3cb50* resource;  // 0x68
    u8 pad_0x70[0xa0];
    CStrId area;  // 0x110
};

struct UnkMapPlayer_lb3cb50 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

struct UnkMapMenu_lb3cb50 {
    u8 pad_0x0[0x8f0];
    base::UnkStrIdVec markers;  // 0x8f0
};

using UnkMapIter_lb3cb50 = base::UnkIterValue<UnkMapEntry_lb3cb50, CStrId>;

static inline bool IsMarkerType(const CStrId& type) {
    return type == g_unk_71727c82d8 || type == g_unk_71727c82e0 || type == g_unk_71727c82e8 ||
           type == g_unk_71727c82f0 || type == g_unk_71727c82f8 || type == g_unk_71727c8300 ||
           type == g_unk_71727c8308 || type == g_unk_71727c8310;
}

static inline UnkMapMarker_lb3cb50* MarkerOf(UnkMapState_lb3cb50* map, const CStrId* id) {
    return static_cast<UnkMapMarker_lb3cb50*>(
        fn_7100e91018(reinterpret_cast<UnkRecordTable_le91018*>(map),
                      reinterpret_cast<UnkKeyHolder_le91018**>(const_cast<CStrId*>(id)), 0));
}

void fn_7100b3cb50(UnkMapMenu_lb3cb50* self) {
    for (u32 i = 0; i < self->markers.count; ++i)
        self->markers.data[i].~CStrId();
    self->markers.count = 0;

    UnkMapState_lb3cb50* map = reinterpret_cast<UnkMapState_lb3cb50*>(fn_7100e8d8b0());
    UnkMapResource_lb3cb50* resource = map->resource;
    if (!resource)
        return;
    ++resource->ref_count;

    base::math::CVector2D player_pos;
    {
        UnkMapPlayer_lb3cb50* player = reinterpret_cast<UnkMapPlayer_lb3cb50*>(fn_7100dec788(g_unk_7101d47310));
        base::math::CVector3D position = {player->transform[0][3], player->transform[1][3], player->transform[2][3]};
        fn_710007b430(&player_pos, &position);
    }

    {
        UnkMapIter_lb3cb50 it;
        it.SetRef(resource->keys, resource->values);
        while (it.ref.index != &resource->keys[resource->count]) {
            if (IsMarkerType(it.Element().type)) {
                UnkMapMarker_lb3cb50* marker = MarkerOf(map, &it.Index());
                if (marker && fn_7100eb7edc(reinterpret_cast<char*>(marker))) {
                    const CStrId& area = *fn_7100de9bf8(g_unk_7101d47310);
                    f32 dx = marker->x - player_pos.x;
                    f32 dy = marker->y - player_pos.y;
                    f32 distance = sqrtf(dx * dx + dy * dy);
                    // Markers next to the player in the area being shown are left out.
                    if (!(map->area == area && distance < 500.0f)) {
                        u32 index = 0;
                        for (const CStrId *id = self->markers.data, *end = id + self->markers.count; id != end;
                             ++id) {
                            if (!(MarkerOf(map, id)->x < marker->x))
                                break;
                            ++index;
                        }
                        fn_71000ce300(&self->markers, &it.Index(), index);
                    }
                }
            }
            ++it.ref.index;
            ++it.ref.element;
        }
    }

    if (--resource->ref_count == 0)
        fn_710001737c(resource);
}
