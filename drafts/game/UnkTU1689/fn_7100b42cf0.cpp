// Unit 1689 (GUI::CMapMenuController's): passes an area and an id on to the map data, then records a marker id
// (at the zero position) in the list the map data keeps under the area, unless that list already has it.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;

extern const base::math::CVector2D g_unk_7101647780;

// The dictionary's out-of-line growth, defined with its own view of the arrays.
struct UnkReleasedStrId;
struct UnkIdValues;
using UnkIdValuesArrays = CRntSmallDictionary<UnkReleasedStrId, CRntVector<UnkIdValues>>;
void fn_7100b48944(UnkIdValuesArrays* self, u32 n);

// A string id that can be moved from (CStrId's copy).
struct UnkMovableStrId_lb42cf0 : UnkStrIdBase {
    UnkMovableStrId_lb42cf0(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkMovableStrId_lb42cf0(UnkMovableStrId_lb42cf0&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovableStrId_lb42cf0() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    bool operator==(const CStrId& other) const { return data == other.data; }
};

struct UnkMarker_lb42cf0 {
    UnkMovableStrId_lb42cf0 id;
    f32 x;
    f32 y;

    UnkMarker_lb42cf0(const CStrId& id, const base::math::CVector2D& position) : id(id), x(position.x), y(position.y) {}
    UnkMarker_lb42cf0(UnkMarker_lb42cf0&& other)
        : id(static_cast<UnkMovableStrId_lb42cf0&&>(other.id)), x(other.x), y(other.y) {}
};

struct UnkMarkers_lb42cf0 : CRntVector<UnkMarker_lb42cf0> {
    explicit UnkMarkers_lb42cf0(u32 flags) : CRntVector(flags) {}

    bool Contains(const CStrId& id) const {
        bool found = false;
        for (u32 i = 0; i < count && !found; ++i)
            found = data[i].id == id;
        return found;
    }

    // EmplaceBack through the out-of-line reserve.
    void Add(UnkMarker_lb42cf0&& marker) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        fn_7100b48af4(reinterpret_cast<UnkIdValuesArray*>(this), new_capacity);
        new (&data[count]) UnkMarker_lb42cf0(static_cast<UnkMarker_lb42cf0&&>(marker));
        ++count;
    }
};

struct UnkAreaMarkers_lb42cf0 : CRntSmallDictionary<CStrId, UnkMarkers_lb42cf0> {
    // Adds an empty list under `area` and returns it.
    UnkMarkers_lb42cf0* Add(const CStrId& area) {
        if (count == capacity)
            fn_7100b48944(reinterpret_cast<UnkIdValuesArrays*>(this), count + step);
        new (&keys[count]) CStrId(area);
        new (&values[count]) UnkMarkers_lb42cf0(0xf);  // growth step 10, allocation flags 0xf
        u32 n = ++count;
        UnkMarkers_lb42cf0* end = &values[n];
        return n ? end - 1 : values;
    }
};

struct UnkMapOwner_lb42cf0 {
    u8 pad_0x0[0xd642a0];
    UnkAreaMarkers_lb42cf0 area_markers;
};

void fn_7100b42cf0(const CStrId* area, CStrId* id, const CStrId* marker) {
    UnkMapData* map = fn_7100e8d8b0();
    UnkAreaMarkers_lb42cf0* area_markers = &reinterpret_cast<UnkMapOwner_lb42cf0*>(map)->area_markers;
    fn_7100e9cbac(map, area, id, true);
    UnkMarkers_lb42cf0* markers = area_markers->Find(*area);
    if (markers == area_markers->End()) {
        markers = area_markers->Add(*area);
    } else if (markers->Contains(*marker)) {
        return;
    }
    markers->Add(UnkMarker_lb42cf0(*marker, g_unk_7101647780));
}
