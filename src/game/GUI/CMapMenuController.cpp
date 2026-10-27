// GUI::CMapMenuController's unit (tools/tu_map.py unit 1689): fn_7100b414f8 lays out the map
// menu's markers. Each marker of the current map (a record keyed by a string id) gets its widget
// (fn_7100b4190c), shown when the blackboard section of the current map says so, scaled with the
// map's zoom and placed at its position on the map, anchored on the side it points to.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "types.h"

using base::global::CStrId;

// The GUI objects and their 2D values (as src/game_s7/UnkGuiS7.h names them).
using UnkGuiObj = UnkGuiDisplayObject;
using UnkVec2f = base::math::CVector2D;

void fn_71011cfdb8(UnkGuiObj* object, const UnkVec2f* scale);
void fn_71011cd5a0(UnkGuiObj* object);

// A marker: its box on the map (from its first to its second corner) and its name.
struct UnkMapMarker {
    f32 x0, y0;
    f32 x1, y1;
    CStrId name;
    u8 pad_0x18[0x68];
};
static_assert(sizeof(UnkMapMarker) == 0x80, "UnkMapMarker layout");

// The markers of a map: a counted resource holding the records and their keys.
struct UnkMapMarkers : UnkCountedResource {
    u8 pad_0x58[0x6250];
    UnkMapMarker* values;
    CStrId* keys;
    u32 count;
};
static_assert(__builtin_offsetof(UnkMapMarkers, values) == 0x62a8, "UnkMapMarkers layout");

// A key and its record, destroyed out of line (fn_7100b47fb0, src/game_aca57c/fn_7100e4ed10.cpp,
// which sees the pair as UnkIdsArraysRecord).
struct UnkIdsArraysRecord;
void fn_7100b47fb0(UnkIdsArraysRecord* self);
struct UnkMapMarkerEntry {
    CStrId key;
    UnkMapMarker marker;
};

// An iterator over the markers (as base::UnkIterValue): it refers to a key and its record
// (state 2) or holds a copy (state 1).
struct UnkMapMarkerIter {
    union {
        UnkMapMarkerEntry value;
        struct {
            CStrId* index;
            UnkMapMarker* element;
        } ref;
    };
    s32 state;

    UnkMapMarkerIter(CStrId* index, UnkMapMarker* element) {
        state = 2;
        ref.index = index;
        ref.element = element;
    }
    ~UnkMapMarkerIter() {
        if (state == 2) {
        } else if (state == 1) {
            fn_7100b47fb0(reinterpret_cast<UnkIdsArraysRecord*>(&value));
        } else {
            return;
        }
        state = 0;
    }
    CStrId& Index() { return state == 2 ? *ref.index : value.key; }
    UnkMapMarker& Element() { return state == 2 ? *ref.element : value.marker; }
    void operator++() {
        ++ref.index;
        ++ref.element;
    }
};

// A reference to a counted resource for the length of a scope.
template <class T>
struct UnkCountedRef {
    T* ptr;

    explicit UnkCountedRef(T* p) : ptr(p) { ++ptr->ref_count; }
    ~UnkCountedRef() {
        if (--ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

struct UnkMapData {
    u8 pad_0x0[0x68];
    UnkMapMarkers* markers;
    u8 pad_0x70[0x4c];
    f32 zoom;
    u8 pad_0xc0[0x50];
    CStrId field_0x110;
    u8 pad_0x118[0xd63c18];
    f32 bounds[4];  // the map's box: min x, min y, max x, max y
};
static_assert(__builtin_offsetof(UnkMapData, bounds) == 0xd63d30, "UnkMapData layout");

UnkMapData* fn_7100e8d8b0();
bool fn_7100e94214();

struct UnkMapMenuSub {
    u8 pad_0x0[0xb4];
    bool field_0xb4;
};
struct UnkMapMenuHolder {
    u8 pad_0x0[0x10];
    UnkMapMenuSub* field_0x10;
};
struct UnkMapMenuController {
    u8 pad_0x0[0x238];
    UnkMapMenuHolder* field_0x238;
};

// The widget of the marker under `key`.
UnkGuiObj* fn_7100b4190c(UnkMapMenuController* self, const CStrId* key, CStrId name);
// The blackboard property name of a marker: fn_71004f1748 builds it in place, as it builds the
// registry tables' values (base/reflection/UnkReflStatics.h), from an owner and the marker's key:
// 16 bytes, the name's string first, then the owner.
extern u8 g_unk_7172753148[];
void fn_71004f1748(base::reflection::UnkReflRegistryMap::Value* value, base::reflection::UnkReflRegistryItem* item,
                   const base::global::CStrId& name);

// Lays out the markers of the current map.
// NON_MATCHING(minor): register allocation and scheduling: the original knows fn_7100b4190c (its
// unit neighbour) leaves the name it is passed alone, so it keeps that name's string in a register
// for the release instead of reloading it, spills the blackboard pointer instead, and selects the
// horizontal side of a flat marker where ours branches.
void fn_7100b414f8(UnkMapMenuController* self) {
    UnkMapData* map = fn_7100e8d8b0();
    if (fn_7100e94214())
        return;
    UnkMapMarkers* markers = map->markers;
    if (!markers)
        return;
    UnkCountedRef<UnkMapMarkers> hold(markers);
    UnkGameManager* gm = g_unk_7101d47310;
    CStrId section = fn_7100dfbda4(gm, map->field_0x110);
    f32 width = map->bounds[2] - map->bounds[0];
    f32 height = map->bounds[3] - map->bounds[1];
    for (UnkMapMarkerIter it(markers->keys, markers->values); it.ref.index != markers->keys + markers->count; ++it) {
        CStrId* key = &it.Index();
        UnkMapMarker* m = &it.Element();
        UnkGuiObj* widget = fn_7100b4190c(self, key, m->name);
        bool shown = false;
        {
            alignas(8) u8 property[sizeof(base::reflection::UnkReflRegistryMap::Value)];
            fn_71004f1748(reinterpret_cast<base::reflection::UnkReflRegistryMap::Value*>(property),
                          reinterpret_cast<base::reflection::UnkReflRegistryItem*>(g_unk_7172753148), *key);
            CStrId* name = reinterpret_cast<CStrId*>(property);
            if (CBlackboard::CSection* s = fn_7101047270(&gm->field_0x968, section))
                fn_710006d11c(s, name, &shown);
            name->~CStrId();
        }
        bool visible;
        if (shown) {
            bool hidden = false;
            if (self->field_0x238 && self->field_0x238->field_0x10)
                hidden = self->field_0x238->field_0x10->field_0xb4;
            visible = !hidden;
        } else {
            visible = false;
        }
        fn_71011cdc30(widget, visible);
        f32 x;
        f32 y;
        u32 h;
        u32 v;
        f32 dy = m->y1 - m->y0;
        if (dy == 0.0f) {
            x = m->x1;
            v = 3;
            y = m->y1 + 200.0f;
            h = x - m->x0 > 0.0f ? 3 : 2;
        } else {
            x = m->x1;
            v = dy > 0.0f ? 3 : 2;
            y = (dy > 0.0f ? 200.0f : -200.0f) + m->y1;
            h = 1;
        }
        fn_71011d0088(widget, 1);
        f32 scale = 2.6f / map->zoom;
        f32 s = scale <= 0.33f ? 0.33f : (scale > 5.5f ? 5.5f : scale);
        UnkVec2f size = {s, s};
        fn_71011cfdb8(widget, &size);
        f32 u = (x - map->bounds[0]) / width;
        f32 w = (y - map->bounds[1]) / height;
        u32 anchor;
        if (h == 1) {
            u = u + -0.5f;
            anchor = 1;
        } else if (h == 3) {
            u = 1.0f - u;
            anchor = 2;
        } else {
            anchor = 0;
        }
        fn_71011ed018(widget, anchor);
        w = v == 2 ? 1.0f - w : w;
        fn_71011ed084(widget, (v != 2) << 1);
        UnkVec2f pos = {u, w};
        fn_71011cde64(widget, &pos, h, v);
        fn_71011cd5a0(widget);
    }
}
