// Unit 2566 (CMinimapManager::CTunableMinimapManager): collects the map's markers that the
// blackboard section of the current map says are shown. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTU782.h"
#include "game/CBlackboard.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1689.h"
#include "types.h"

using base::global::CStrId;

// A marker record (0x80 bytes).
struct UnkMapMarker_996b4 {
    u8 data[0x80];
};

// The markers of a map: the records and their keys.
struct UnkMapMarkers_996b4 {
    u8 pad_0x0[0x62a8];
    UnkMapMarker_996b4* values;
    CStrId* keys;
    u32 count;
};

// A key and its record, destroyed out of line (src/game_aca57c/fn_7100e4ed10.cpp).
struct UnkIdsArraysRecord;
struct UnkMapMarkerEntry_996b4 {
    CStrId key;
    UnkMapMarker_996b4 marker;
};

// An iterator over the markers: it refers to a key and its record (state 2) or holds a copy (state 1).
struct UnkMapMarkerIter_996b4 {
    union {
        UnkMapMarkerEntry_996b4 value;
        struct {
            CStrId* index;
            UnkMapMarker_996b4* element;
        } ref;
    };
    s32 state;

    UnkMapMarkerIter_996b4(CStrId* index, UnkMapMarker_996b4* element) {
        state = 2;
        ref.index = index;
        ref.element = element;
    }
    ~UnkMapMarkerIter_996b4() {
        if (state == 2) {
        } else if (state == 1) {
            fn_7100b47fb0(reinterpret_cast<UnkIdsArraysRecord*>(&value));
        } else {
            return;
        }
        state = 0;
    }
    CStrId& Index() { return state == 2 ? *ref.index : value.key; }
    UnkMapMarker_996b4& Element() { return state == 2 ? *ref.element : value.marker; }
    void operator++() {
        ++ref.index;
        ++ref.element;
    }
};

struct UnkMarkersOwner_996b4 {
    u8 pad_0x0[0x68];
    UnkMapMarkers_996b4* markers;
    u8 pad_0x70[0xa0];
    CStrId field_0x110;
    u8 pad_0x118[0xd640c8];
    base::global::CRntVector<UnkMapMarker_996b4*> shown;
};
static_assert(__builtin_offsetof(UnkMarkersOwner_996b4, shown) == 0xd641e0, "UnkMarkersOwner_996b4 layout");


void fn_7100e996b4(UnkMarkersOwner_996b4* self) {
    if (!self->markers)
        return;
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* blackboard = &gm->field_0x968;
    CStrId sectionName = fn_7100dfbda4(gm, self->field_0x110);
    CBlackboard::CSection* section = fn_7101047484(blackboard, sectionName);
    self->shown.Clear();
    UnkMapMarkers_996b4* markers = self->markers;
    for (UnkMapMarkerIter_996b4 it(markers->keys, markers->values); it.ref.index != markers->keys + markers->count;
         ++it) {
        UnkMapMarker_996b4* marker = &it.Element();
        bool shown = false;
        if (section) {
            alignas(8) u8 property[sizeof(base::reflection::UnkReflRegistryMap::Value)];
            fn_71004f1748(reinterpret_cast<base::reflection::UnkReflRegistryMap::Value*>(property),
                          reinterpret_cast<base::reflection::UnkReflRegistryItem*>(g_unk_7172753148), it.Index());
            CStrId* name = reinterpret_cast<CStrId*>(property);
            fn_710006d11c(section, name, &shown);
            name->~CStrId();
        }
        if (shown)
            self->shown.PushBack(marker);
    }
}
