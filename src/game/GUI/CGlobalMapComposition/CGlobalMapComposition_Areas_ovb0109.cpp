// GUI::CGlobalMapComposition (unit 1680 of tools/tu_map.py): the global map's area handling.
// - fn_7100b262f4 frees a region's two area lists; fn_7100b2b660 grows the regions keyed by id
//   (values of 0x58 bytes and string-id keys, moved over one by one).
// - fn_7100b29d9c counts down the reveal timer (+0x5ac): in state 1 it goes to state 2 and marks the
//   pending area's region (fn_7100b26470) for another 1.5 s; in state 2 it hands the pending area to
//   the world (fn_7100e9867c) and clears the composition's flag (fn_71011cdce8).
// - fn_7100b2adfc is the confirm: after the base handler, while the flag is set, the current area
//   (or the empty id) goes to the world, the flag is cleared and the confirm sound plays.
// - fn_7100b29ed0 maps a point inside a rectangle onto a view's frame and into its local space,
//   in units of the frame's size.
// - fn_7100b2af40 and fn_7100b2b074 place and read an area of the "AreasComposition" container
//   (looked up once and kept at +0x5e0).
// - fn_7100b289f4 fades out the two sounds, resets the selected ids and leaves two deferred sets.
// - fn_7100b2a898 stores a target and four ids and moves to state 3.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/UnkSound.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CStrId;

struct UnkGallery_A5_b75;
void fn_71011cf70c(UnkGallery_A5_b75* self);
struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkStateSink* g_unk_717275c380;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
extern UnkEnumType g_unk_71729f01a8;
struct UnkGlobalMapComposition;
void fn_7100b26470(UnkGlobalMapComposition* self, void* object, bool flag);
UnkVec2Result fn_71011cf12c(UnkGuiDisplayObject* self, const UnkVec2Result& point);
struct UnkGuiObj;
void fn_71011cd5a0(UnkGuiObj* object);

// The world takes the area to show (the game manager's +0x2228).
void fn_7100e9867c(void* world, const CStrId& area);
// A point of a display object's anchor state (the block at +0x78) by horizontal and vertical
// anchor, as src/game/GUI/CMapMenuController/CMapMenuController_BindWidgets.cpp declares it.
struct UnkGuiItemState;
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};
UnkGuiPos fn_71011f6240(const UnkGuiItemState* state, u32 h, u32 v);

// ---- The regions ----

// An area entry: an id released in two steps and two more words.
struct UnkMapAreaEntry_ovb0109 {
    UnkStrIdMember id;
    u64 field_0x8;
    u64 field_0x10;
};
static_assert(sizeof(UnkMapAreaEntry_ovb0109) == 0x18, "UnkMapAreaEntry_ovb0109 size");

// An array of area entries laid out as base::UnkVector, moved by taking the other's storage.
struct UnkMapAreaList_ovb0109 {
    UnkMapAreaEntry_ovb0109* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkMapAreaList_ovb0109(UnkMapAreaList_ovb0109&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    void Free() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkMapAreaEntry_ovb0109();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A region: two area lists and its marker on the map.
struct UnkMapRegion_ovb0109 {
    UnkMapAreaList_ovb0109 first;   // 0x0
    UnkMapAreaList_ovb0109 second;  // 0x28
    void* marker;                   // 0x50

    UnkMapRegion_ovb0109(UnkMapRegion_ovb0109&& other)
        : first(static_cast<UnkMapAreaList_ovb0109&&>(other.first)),
          second(static_cast<UnkMapAreaList_ovb0109&&>(other.second)), marker(other.marker) {}
    ~UnkMapRegion_ovb0109();  // fn_7100b262f4
};
static_assert(sizeof(UnkMapRegion_ovb0109) == 0x58, "UnkMapRegion_ovb0109 size");

__attribute__((noinline)) void fn_7100b262f4(UnkMapRegion_ovb0109* self) {
    self->second.Free();
    self->first.Free();
}

inline UnkMapRegion_ovb0109::~UnkMapRegion_ovb0109() { fn_7100b262f4(this); }

// The regions keyed by id: two parallel arrays sharing a count, capacity and allocator.
struct UnkMapRegions_ovb0109 {
    UnkMapRegion_ovb0109* values;
    UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    // The region with this key, or the one past the end.
    UnkMapRegion_ovb0109* Find(const CStrId& key) {
        u32 n = count;
        UnkMapRegion_ovb0109* v = values;
        u64 i = 0;
        for (; i != n; i++) {
            if (keys[i].data == key.data)
                break;
        }
        return &v[i];
    }
};

void fn_7100b2b660(UnkMapRegions_ovb0109* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* v = static_cast<UnkMapRegion_ovb0109*>(self->allocator->vf_0x40((u64)n * 0x58, 8, self->flags));
    auto* k = static_cast<UnkStrIdMember*>(self->allocator->vf_0x40((u64)n * 8, 8, self->flags));
    if (self->values) {
        UnkMapRegion_ovb0109* src = self->values;
        UnkMapRegion_ovb0109* dst = v;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkMapRegion_ovb0109(static_cast<UnkMapRegion_ovb0109&&>(*src));
            src->~UnkMapRegion_ovb0109();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        UnkStrIdMember* src = self->keys;
        UnkStrIdMember* dst = k;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            union Slot {
                base::global::UnkStringData* p;
            };
            dst->data = src->data;
            reinterpret_cast<Slot*>(&src->data)->p = &g_unk_7101d47108->empty;
            src->~UnkStrIdMember();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = v;
    self->keys = k;
    self->capacity = n;
}

// ---- The composition ----

// A handle on a sound: the reference and the serial it was taken with.
struct UnkMapSoundRef_ovb0109 {
    UnkSound* sound;
    s64 serial;
};
struct UnkMapSoundHandle_ovb0109 {
    UnkMapSoundRef_ovb0109* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
    void FadeOut() const {
        if (UnkSound* sound = Get())
            fn_7100105a80(sound, 0.2f, 0.0f);
    }
};

// A registration in a deferred set: the set, removed from it by the entry's address.
struct UnkMapRegistration_ovb0109 {
    base::UnkDeferredSet<void>* set;

    void Remove() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
    }
};

struct UnkMapArea_ovb0109 {
    u8 pad_0x0[0x290];
    CStrId name;  // 0x290
};

struct UnkMapTarget_ovb0109 {
    u64 field_0x0;
    u64 field_0x8;
};

// The confirm input: set once it has been handled.
struct UnkMapInput_ovb0109 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkMapComposition_ovb0109 {
    UnkGuiDisplayObject base;
    u8 pad_0x198[0x588 - sizeof(UnkGuiDisplayObject)];
    s32 state;  // 0x588
    u8 pad_0x58c[0x14];
    CStrId pending_area;            // 0x5a0
    s32 reveal_state;               // 0x5a8
    f32 reveal_timer;               // 0x5ac
    UnkMapRegions_ovb0109 regions;  // 0x5b0
    u8 pad_0x5d8[0x8];
    UnkGuiDisplayObject* areas;  // 0x5e0: the "AreasComposition" container
    UnkMapArea_ovb0109* area;    // 0x5e8
    u8 pad_0x5f0[0x108];
    CStrId selected_area;  // 0x6f8
    CStrId hovered_area;   // 0x700
    u8 pad_0x708[0x8];
    UnkMapSoundHandle_ovb0109 open_sound;   // 0x710
    UnkMapSoundHandle_ovb0109 close_sound;  // 0x720
    u8 pad_0x730[0x10];
    UnkMapRegistration_ovb0109 registration_a;  // 0x740
    u8 pad_0x748[0x10];
    UnkMapRegistration_ovb0109 registration_b;  // 0x758
    u8 pad_0x760[0x10];
    UnkMapTarget_ovb0109 target;  // 0x770
    CStrId scenario;              // 0x780
    CStrId target_area;           // 0x788
    CStrId room;                  // 0x790
    CStrId marker;                // 0x798
};
static_assert(__builtin_offsetof(UnkMapComposition_ovb0109, regions) == 0x5b0, "UnkMapComposition_ovb0109 layout");
static_assert(__builtin_offsetof(UnkMapComposition_ovb0109, selected_area) == 0x6f8,
              "UnkMapComposition_ovb0109 layout");
static_assert(__builtin_offsetof(UnkMapComposition_ovb0109, marker) == 0x798, "UnkMapComposition_ovb0109 layout");

static inline CStrId AreaName(UnkMapComposition_ovb0109* self) {
    if (self->area)
        return self->area->name;
    return g_unk_7101d470e0;
}

void fn_7100b29d9c(UnkMapComposition_ovb0109* self, f32 dt) {
    if (!(self->reveal_timer > 0.0f))
        return;
    f32 t = self->reveal_timer - dt;
    self->reveal_timer = t;
    if (!(t <= 0.0f))
        return;
    switch (self->reveal_state) {
    case 1: {
        self->reveal_state = 2;
        {
            CStrId area(self->pending_area);
            fn_7100b26470(reinterpret_cast<UnkGlobalMapComposition*>(self), self->regions.Find(area)->marker, true);
        }
        self->reveal_timer = 1.5f;
        break;
    }
    case 2:
        fn_7100e9867c(g_unk_7101d47310->field_0x2228, self->pending_area);
        fn_71011cdce8(&self->base, false);
        break;
    }
}

void fn_7100b2adfc(UnkMapComposition_ovb0109* self, UnkMapInput_ovb0109* input) {
    fn_71011cf70c(reinterpret_cast<UnkGallery_A5_b75*>(self));
    if (!self->base.field_0xb4)
        return;
    fn_7100e9867c(g_unk_7101d47310->field_0x2228, AreaName(self));
    fn_71011cdce8(&self->base, false);
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01a8), self);
    input->handled = true;
}

// ---- Points on the map ----

struct UnkMapRect_ovb0109 {
    f32 x0, y0, x1, y1;
};

// A frame's size: its width and height times its scale.
struct UnkMapFrame_ovb0109 {
    u8 pad_0x0[0x14];
    f32 width;    // 0x14
    f32 height;   // 0x18
    f32 scale_x;  // 0x1c
    f32 scale_y;  // 0x20
};

struct UnkMapView_ovb0109 {
    u8 pad_0x0[0x230];
    UnkGuiDisplayObject* frame;  // 0x230
};

static inline UnkMapFrame_ovb0109* Frame(UnkGuiDisplayObject* view) {
    return reinterpret_cast<UnkMapFrame_ovb0109*>(fn_71011cde14(reinterpret_cast<UnkMapView_ovb0109*>(view)->frame));
}

// The anchor state of a display object (+0x78), as fn_71011f6240 takes it.
static inline UnkGuiItemState* AnchorState(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkGuiItemState*>(reinterpret_cast<u8*>(object) + 0x78);
}

UnkVec2Result fn_7100b29ed0(void* self, const UnkVec2Result& point, UnkGuiDisplayObject* view,
                            const UnkMapRect_ovb0109& rect) {
    UnkVec2Result result(0.0f, 0.0f);
    if (!view)
        return result;
    f32 left = rect.x0;
    f32 top = rect.y0;
    f32 u = (point.x - left) / (rect.x1 - left);
    f32 v = (point.y - top) / (rect.y1 - top);
    u = u > 0.0f ? u : 0.0f;
    v = v > 0.0f ? v : 0.0f;
    UnkMapFrame_ovb0109* frame = Frame(view);
    UnkGuiPos origin = fn_71011f6240(reinterpret_cast<UnkGuiItemState*>(frame), 2, 3);
    f32 w = frame->width * frame->scale_x;
    f32 h = frame->height * frame->scale_y;
    UnkVec2Result local = fn_71011cf12c(view, UnkVec2Result(u * w + origin.x, origin.y - v * h));
    f32 x = local.x;
    f32 y = local.y;
    frame = Frame(view);
    result = UnkVec2Result(x / (frame->width * frame->scale_x), y / (frame->height * frame->scale_y));
    return result;
}

static inline UnkGuiDisplayObject* AsContainer(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(fn_71011d7b84()))
               ? o
               : nullptr;
}

static inline UnkGuiDisplayObject* Areas(UnkMapComposition_ovb0109* self) {
    if (!self->areas)
        self->areas = AsContainer(fn_71011d94d4(&self->base, CStrId("AreasComposition", true)));
    return self->areas;
}

void fn_7100b2af40(UnkMapComposition_ovb0109* self, const CStrId& name, const base::math::CVector2D* position) {
    if (UnkGuiDisplayObject* area = AsContainer(fn_71011d94d4(Areas(self), name))) {
        fn_71011cde64(area, position, 0, 0);
        fn_71011cd5a0(reinterpret_cast<UnkGuiObj*>(area));
    }
}

UnkVec2Result fn_7100b2b074(UnkMapComposition_ovb0109* self, const CStrId& name) {
    UnkVec2Result result(0.0f, 0.0f);
    if (UnkGuiDisplayObject* area = AsContainer(fn_71011d94d4(Areas(self), name))) {
        UnkGuiPos p = fn_71011f6240(AnchorState(area), 0, 0);
        result.x = p.x;
        result.y = p.y;
    }
    return result;
}

// ---- State ----

void fn_7100b289f4(UnkMapComposition_ovb0109* self) {
    self->open_sound.FadeOut();
    self->close_sound.FadeOut();
    self->selected_area.Reset();
    self->hovered_area.Reset();
    self->registration_a.Remove();
    self->registration_b.Remove();
}

void fn_7100b2a898(UnkMapComposition_ovb0109* self, const CStrId& scenario, const CStrId& area, const CStrId& room,
                   const CStrId& marker, const UnkMapTarget_ovb0109& target) {
    self->target = target;
    self->scenario = scenario;
    self->target_area = area;
    self->room = room;
    self->marker = marker;
    self->state = 3;
}
