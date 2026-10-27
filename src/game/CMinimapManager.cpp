// The minimap manager's unit (CMinimapManager, CTunableMinimapManager; translation unit 2566 of
// data/tu_map.csv). The owner is the object behind the game manager's world pointer; the unit's
// statics hold four draw lists the frame fills. Here: drawing a map item's icon and outline
// (fn_7100e964fc), resizing an item's fixed vector of modifiers (fn_7100ea9b04), the camera update
// (fn_7100e90e6c), the frame start (fn_7100e91d68), the two draw passes (fn_7100e93e04), fitting the
// viewport to the screen (fn_7100e8f6bc), the markers (fn_7100ea3f7c), the regions' points and
// outlines (fn_7100ea4504) and showing a hidden icon kind again (fn_7100e9d1f8).
#include <new>

#include "../game_aca57c/UnkBufferString.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkGameManager.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkMapMenuController.h"
#include "game/UnkMinimapIconKinds.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

typedef u32 __attribute__((may_alias)) a_u32;

// The vectors and the colour copy word by word; their user-provided copy constructors make
// by-value arguments go through a temporary passed by address.
struct UnkMinimapVec2 {
    f32 x;
    f32 y;
    UnkMinimapVec2() {}
    UnkMinimapVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkMinimapVec2(const UnkMinimapVec2& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
    }
};

struct UnkMinimapVec3 {
    f32 x;
    f32 y;
    f32 z;
    UnkMinimapVec3() {}
    UnkMinimapVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkMinimapVec3& operator=(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        return *this;
    }
    UnkMinimapVec3 operator*(f32 s) const { return UnkMinimapVec3(x * s, y * s, z * s); }
    UnkMinimapVec3(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
    }
};

// The colour is a vector (its assignment is the vector's, fn_710007b518).
struct UnkMinimapColor : base::math::CVector4D {
    UnkMinimapColor() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }
    UnkMinimapColor(const UnkMinimapVec3& rgb, f32 a_) {
        x = rgb.x;
        y = rgb.y;
        z = rgb.z;
        w = a_;
    }
    UnkMinimapColor(const UnkMinimapColor& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
};

// An item's icon definition: whether it is sized from the item's box (0xb0), which of the
// owner's two scales it uses (0xb1), and four margins that must all be positive for the boxed
// drawing.
struct UnkMinimapIconDef {
    u8 unk_0x0[0xb0];
    bool sized_by_box;
    bool alt_scale;
    u8 unk_0xb2[0x2];
    f32 margins[4];
};

extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

// A string (base::global::CRntString's layout) built from the empty literal by fn_710007e704.
struct UnkMinimapName {
    u8 unk_0x0[0x30];

    __attribute__((always_inline)) UnkMinimapName() {
        fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(this), g_unk_710162e2ed);
    }
    __attribute__((always_inline)) ~UnkMinimapName() {
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};

// A modifier applied to an item (0xd8 bytes): two strings, a scale of ones and the factor at
// 0xd0 the drawing multiplies.
struct UnkMinimapModifier {
    u32 field_0x0[4] = {};
    u32 field_0x10 = 0;
    u8 unk_0x14[0x4];
    UnkMinimapName field_0x18;
    u32 field_0x48 = 0;
    u8 unk_0x4c[0x4];
    f32 field_0x50[3] = {1.0f, 1.0f, 1.0f};
    UnkString20 field_0x60;
    u32 field_0xc8 = 0;
    u8 unk_0xcc[0x4];
    f32 factor = 0.0f;
    bool field_0xd4 = true;
    bool field_0xd5 = false;
};
static_assert(sizeof(UnkMinimapModifier) == 0xd8, "UnkMinimapModifier layout");

// Up to three modifiers in raw storage, the count first (a fixed-capacity vector).
struct UnkMinimapModifiers {
    u32 count;
    alignas(4) u8 storage[3 * sizeof(UnkMinimapModifier)];

    UnkMinimapModifier* data() { return reinterpret_cast<UnkMinimapModifier*>(storage); }
    const UnkMinimapModifier* begin() const { return reinterpret_cast<const UnkMinimapModifier*>(storage); }
    const UnkMinimapModifier* end() const { return begin() + count; }

    __attribute__((always_inline)) f32 Product(f32 value) const {
        for (const UnkMinimapModifier& it : *this)
            value *= it.factor;
        return value;
    }
};

struct UnkMinimapItem {
    UnkMinimapIconDef* icon;
    UnkMinimapVec2 pos;
    UnkMinimapVec2 box_min;
    UnkMinimapVec2 box_max;
    UnkMinimapVec3 color;
    f32 field_0x2c;
    f32 size;
    bool field_0x34;
    bool flipped;
    u8 unk_0x36[0x2];
    UnkMinimapIconDef* outline;
    UnkMinimapVec3 outline_color;
    u8 unk_0x4c[0x14];
    UnkMinimapModifiers alpha_modifiers;
    UnkMinimapModifiers size_modifiers;
};
static_assert(__builtin_offsetof(UnkMinimapItem, size_modifiers) == 0x2ec, "UnkMinimapItem layout");

// A rectangle (min x, min y, max x, max y); returned in memory, copied word by word.
struct UnkMinimapRect {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
    UnkMinimapRect(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
    UnkMinimapRect& operator=(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
        return *this;
    }
};

// The map's camera object and the scene objects of the two draw passes (game/UnkSceneObject.h): the
// camera's view transform at 0x20 and, two levels down, a dirty flag.
using UnkMinimapCamera = UnkSceneObject;
using UnkMinimapCameraSub = UnkSceneObjectSub;

// A vector of pointers (with a capacity and more after the count; 0x28 bytes).
template <class T>
struct UnkMinimapPtrVector {
    T** data;
    u32 count;
    u8 unk_0xc[0x1c];

    T* const* begin() const { return data; }
    T* const* end() const { return data + count; }
};
using UnkMinimapItemList = UnkMinimapPtrVector<UnkMinimapItem>;

// A marker the map draws where its box overlaps the view: a kind (0x10) that picks its colour, the
// shape it draws (0x18), its box and a word passed along.
struct UnkMinimapMarker {
    u8 unk_0x0[0x10];
    u8 kind[0x8];
    u8 shape[0x50];
    UnkMinimapRect box;
    u32 field_0x78;
};

// An array whose count follows its data pointer by 0x10 (the word between is unknown).
template <class T>
struct UnkMinimapArray {
    T* data;
    u64 field_0x8;
    u32 count;

    T* begin() const { return data; }
    T* end() const { return data + count; }
};

// A region's outline segment: its box and its kind (2 is drawn dimmed).
struct UnkMinimapSegment {
    u8 unk_0x0[0x8];
    UnkMinimapRect box;
    u32 kind;
    u8 unk_0x1c[0x4];
};
// A shape: its segments and colour.
struct UnkMinimapShape {
    u8 unk_0x0[0x8];
    UnkMinimapArray<UnkMinimapSegment> segments;
    u8 unk_0x20[0x14];
    UnkMinimapVec3 color;
    u8 unk_0x40[0x10];
};
static_assert(sizeof(UnkMinimapShape) == 0x50, "UnkMinimapShape layout");
// A point the region draws at its position.
struct UnkMinimapPoint {
    u8 unk_0x0[0x8];
    u8 pos[0x18];
};
struct UnkMinimapRegion {
    UnkMinimapArray<UnkMinimapShape> shapes;
    u8 unk_0x18[0x28];
    UnkMinimapArray<UnkMinimapPoint> points;
    u8 unk_0x58[0x38];
};
static_assert(sizeof(UnkMinimapRegion) == 0x90, "UnkMinimapRegion layout");

// The reference to the map menu (game/UnkMapMenuController.h): its target at +0x10, which
// fn_7100b41e5c tests.
struct UnkMinimapRef {
    u8 unk_0x0[0x10];
    UnkMapMenuController* target;
};

// The map's on-screen viewport: a node at 0x18 (fn_710020a5a0 scales it, fn_710020a4c4 places it), a
// dirty flag and the view whose rectangle (position, size) fn_71001c36c0 applies.
struct UnkMinimapView {
    u8 unk_0x0[0x90];
    base::math::CVector2D pos;
    base::math::CVector2D size;
};
struct UnkMinimapViewport;
void fn_71001c36c0(UnkMinimapView* view, const base::math::CVector2D* pos, const base::math::CVector2D* size);
struct UnkMinimapViewport {
    u8 unk_0x0[0x18];
    u8 node[0xfb];
    bool dirty;
    u8 unk_0x114[0x64];
    UnkMinimapView* view;

    __attribute__((always_inline)) void SetViewSize(const base::math::CVector2D& size) {
        UnkMinimapView* v = view;
        v->size = size;
        fn_71001c36c0(v, &v->pos, &v->size);
        dirty = true;
    }
    __attribute__((always_inline)) void SetViewPos(const base::math::CVector2D& pos) {
        UnkMinimapView* v = view;
        v->pos = pos;
        fn_71001c36c0(v, &v->pos, &v->size);
        dirty = true;
    }
};
static_assert(__builtin_offsetof(UnkMinimapViewport, view) == 0x178, "UnkMinimapViewport layout");

// The map at +0x68 of the owner: its origin in world units.
struct UnkMinimapMap {
    u8 unk_0x0[0x68];
    UnkMinimapVec2 origin;
};

// The object behind the game manager's world pointer (UnkGameManager::field_0x2228).
struct UnkMinimapOwner {
    u8 unk_0x0[0x68];
    UnkMinimapMap* map;
    u8 unk_0x70[0x34];
    UnkMinimapVec2 screen_pos;  // the map's place on screen, -1 to 1
    UnkMinimapVec2 size;
    f32 field_0xb4;
    f32 field_0xb8;
    f32 zoom;
    u8 unk_0xc0[0xc];
    bool field_0xcc;
    u8 unk_0xcd[0x43];
    base::global::CStrId field_0x110;  // an area's id: the camera follows the player while it is the current one
    u8 field_0x118[0xd63d30 - 0x118];
    UnkMinimapRect view;
    u8 unk_0xd63d40[0x10];
    UnkMinimapVec2 offset;
    u8 unk_0xd63d58[0x20];
    UnkMinimapVec3 camera_pos;
    u8 unk_0xd63d84[0xc];
    u8 timer_0xd63d90[0xb0];
    u8 timer[0x100];
    UnkMinimapRef* field_0xd63f40;
    UnkMinimapCamera* camera;
    UnkMinimapViewport* viewport;
    u8 unk_0xd63f58[0x48];
    UnkMinimapCamera* pass_objects[2];
    u8 unk_0xd63fb0[0x1c];
    u32 pass_layer;
    u8 unk_0xd63fd0[0x1f0];
    f32 field_0xd641c0;
    u8 unk_0xd641c4[0x1c];
    UnkMinimapPtrVector<UnkMinimapMarker> markers;
    u8 unk_0xd64208[0xc0];
    base::UnkVector<base::global::CStrId> hidden_kinds;
    u8 unk_0xd642f0[0x28];
    UnkMinimapItemList pass_items[2];
    u8 unk_0xd64368[0x50];
    UnkMinimapArray<UnkMinimapRegion> regions;
    u8 unk_0xd643d0[0x14];
    f32 scale;
    f32 alt_scale;

    __attribute__((always_inline)) const f32* Scale(const UnkMinimapIconDef* icon) const {
        return icon->alt_scale ? &alt_scale : &scale;
    }
};
static_assert(__builtin_offsetof(UnkMinimapOwner, screen_pos) == 0xa4, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, field_0xcc) == 0xcc, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, field_0x110) == 0x110, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, timer) == 0xd63e40, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, field_0xd641c0) == 0xd641c0, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, camera) == 0xd63f48, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, pass_layer) == 0xd63fcc, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, markers) == 0xd641e0, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, hidden_kinds) == 0xd642c8, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, pass_items) == 0xd64318, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, regions) == 0xd643b8, "UnkMinimapOwner layout");
static_assert(__builtin_offsetof(UnkMinimapOwner, scale) == 0xd643e4, "UnkMinimapOwner layout");

// A draw list (0x28 bytes): its entries and their count, reset at the start of each pass.
struct UnkMinimapDrawList {
    void* data;
    u32 count;
    u8 unk_0xc[0x1c];
};

// A list's entries as a pointer and a 64-bit count, as the scene objects take them.
struct UnkMinimapSpan {
    void* data;
    u64 count;
    UnkMinimapSpan(const UnkMinimapDrawList& list) : data(list.data), count(list.count) {}
};

// The unit's statics, which the compiler merged into one block (addressed from one base): the map
// cell size at 0x71729cdaa4 (a guarded function-local static of fn_7100e91d68), its guard at
// 0x71729cdb10 and the four draw lists at 0x71729cdba0. Declared from the 8-aligned word before
// the cell size so the guard and the lists keep their natural alignment.
struct UnkMinimapStatics {
    u8 unk_0x0[0x4];
    f32 cell_size;
    u8 unk_0x8[0x68];
    u64 cell_size_guard;
    u8 unk_0x78[0x88];
    UnkMinimapDrawList lists[4];
};
static_assert(__builtin_offsetof(UnkMinimapStatics, cell_size_guard) == 0x70, "UnkMinimapStatics layout");
static_assert(__builtin_offsetof(UnkMinimapStatics, lists) == 0x100, "UnkMinimapStatics layout");
extern UnkMinimapStatics g_unk_71729cdaa0 HIDDEN;

bool fn_7100eb7f6c(const UnkMinimapItem* item);
void fn_7100eb9b90(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   UnkMinimapDrawList* lists2, const UnkMinimapIconDef* icon, UnkMinimapVec3 pos,
                   f32 size, UnkMinimapColor color, f32 field_0x2c, bool field_0x34, bool flipped);
void fn_7100ebaaa0(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   UnkMinimapDrawList* lists2, const UnkMinimapIconDef* icon, UnkMinimapVec3 pos,
                   UnkMinimapVec2 half_size, f32 size, UnkMinimapColor color);

void fn_7100e964fc(UnkMinimapOwner* self, const UnkMinimapItem* item, f32 depth) {
    if (!fn_7100eb7f6c(item))
        return;
    f32 alpha = item->alpha_modifiers.Product(1.0f);
    const UnkMinimapIconDef* icon = item->icon;
    f32 scale = *self->Scale(icon);
    UnkMinimapVec2 xy = item->pos;
    f32 y;
    if (icon->sized_by_box)
        y = (item->box_max.y - item->box_min.y) * 0.5f + xy.y;
    else
        y = (item->flipped ? -250.0f : 250.0f) + xy.y;
    UnkMinimapVec3 pos(xy.x, y, depth);
    if (icon->margins[0] > 0.0f && icon->margins[1] > 0.0f && icon->margins[2] > 0.0f &&
        icon->margins[3] > 0.0f) {
        f32 outline_scale = 1.0f;
        const UnkMinimapIconDef* outline = item->outline;
        if (outline && outline->sized_by_box)
            outline_scale = *self->Scale(outline);
        fn_7100ebaaa0(&g_unk_71729cdaa0.lists[0], &g_unk_71729cdaa0.lists[3], &g_unk_71729cdaa0.lists[1],
                      &g_unk_71729cdaa0.lists[2], icon, pos,
                      UnkMinimapVec2(outline_scale * ((item->box_max.x - item->box_min.x) * 0.5f),
                                     outline_scale * ((item->box_max.y - item->box_min.y) * 0.5f)),
                      scale * item->size_modifiers.Product(item->size), UnkMinimapColor(item->color, alpha));
        return;
    }
    f32 size = icon->sized_by_box ? scale * 250.0f : 250.0f;
    fn_7100eb9b90(&g_unk_71729cdaa0.lists[0], &g_unk_71729cdaa0.lists[3], &g_unk_71729cdaa0.lists[1],
                  &g_unk_71729cdaa0.lists[2], icon, pos, size * item->size_modifiers.Product(item->size),
                  UnkMinimapColor(item->color, alpha), item->field_0x2c, item->field_0x34, item->flipped);
    if (const UnkMinimapIconDef* outline = item->outline)
        fn_7100eb9b90(&g_unk_71729cdaa0.lists[0], &g_unk_71729cdaa0.lists[3], &g_unk_71729cdaa0.lists[1],
                      &g_unk_71729cdaa0.lists[2], outline, pos, size * item->size_modifiers.Product(item->size),
                      UnkMinimapColor(item->outline_color, alpha), item->field_0x2c, item->field_0x34,
                      item->flipped);
}

// Grows the vector with default modifiers or destroys the ones past the new size.
void fn_7100ea9b04(UnkMinimapModifiers* self, u32 size) {
    if (self->count < size) {
        for (u32 i = self->count; i < size; ++i)
            new (&self->data()[i]) UnkMinimapModifier;
    } else if (size < self->count) {
        for (u64 i = size; i < self->count; ++i)
            self->data()[i].~UnkMinimapModifier();
    }
    self->count = size;
}

const base::global::CStrId* fn_7100de9bf8(UnkGameManager* self);
bool fn_710002fb70(void* timer);
UnkMinimapRect fn_7100e94e10();
UnkMinimapRect fn_7100e948b4(UnkMinimapOwner* self, bool flag);
void fn_7100205f98(void* xform, const UnkMinimapVec3* eye, f32 fov, f32 near, f32 far);
void fn_7100145400(UnkMinimapCamera* camera, const UnkMinimapVec3* target);

// Places the map camera 8000 units (times the zoom) above its point, which follows the bounds'
// centre on a 10-unit grid while the world's object is the player's, then stores the view.
// NON_MATCHING(minor): the original adds the zoom term first in the eye's x and y sums (ours
// adds the centre first) and keeps zoom * 8000 in s3 (ours s4); the same commutative adds.
void fn_7100e90e6c(UnkMinimapOwner* self) {
    UnkMinimapVec3 center;
    const base::global::CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
    if (self->field_0x110.data == current->data) {
        UnkMinimapRect bounds = fn_7100e94e10();
        f32 sum_x = bounds.min_x + bounds.max_x;
        f32 sum_y = bounds.min_y + bounds.max_y;
        f32 mid_x = sum_x * 0.5f;
        f32 mid_y = sum_y * 0.5f;
        center.z = self->camera_pos.z;
        center.x = __builtin_roundf(mid_x * 0.1f) * 10.0f;
        center.y = __builtin_roundf(mid_y * 0.1f) * 10.0f;
    } else {
        center = self->camera_pos;
    }
    f32 zoom = self->zoom;
    UnkMinimapVec3 eye(center.x + zoom * 0.0f, center.y + zoom * 0.0f, center.z + zoom * 8000.0f);
    UnkMinimapVec3 target(center.x, center.y, center.z);
    fn_710002fb70(self->timer);
    eye.x = self->offset.x + eye.x;
    eye.y = self->offset.y + eye.y;
    target.x = self->offset.x + target.x;
    target.y = self->offset.y + target.y;
    fn_7100205f98(self->camera->field_0x20, &eye, 45.0f, 100.0f, 100000.0f);
    fn_7100145400(self->camera, &target);
    self->camera->field_0x130->flags->dirty = true;
    self->view = fn_7100e948b4(self, fn_710002fb70(self->timer));
}

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern const f32 g_unk_710164be30;
extern u8 g_unk_71727cc99c[] HIDDEN;

void fn_7100e79fa4(void* field_0x118, const UnkMinimapRect* view, u32 flags, u32* count, void* out);
void fn_7100e951d4(UnkMinimapOwner* self, u32 count, void* items, u32 field_0x3);
UnkMinimapVec3 fn_7100206154(void* xform);
bool fn_71001bb54c(UnkMinimapCameraSub* sub, UnkMinimapVec2* size);

// Starts a frame: resets three draw lists, gathers and adds the items in view, and hands the
// render manager the camera's view rectangle in map cells (x over 1024, y over 512).
// NON_MATCHING(minor): scheduling: the original loads the map origin before the camera position
// (other registers for both and for the half sizes) and builds the 1/512 constant before 1/1024.
void fn_7100e91d68(UnkMinimapOwner* self) {
    g_unk_71729cdaa0.lists[0].count = 0;
    g_unk_71729cdaa0.lists[1].count = 0;
    g_unk_71729cdaa0.lists[3].count = 0;
    u64* guard = &g_unk_71729cdaa0.cell_size_guard;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(guard)) {
            g_unk_71729cdaa0.cell_size = g_unk_710164be30 + g_unk_710164be30;
            __cxa_guard_release(guard);
        }
    }
    {
        u32 count = 0;
        fn_7100e79fa4(self->field_0x118, &self->view, 0x80000, &count, g_unk_71727cc99c);
        fn_7100e951d4(self, count, g_unk_71727cc99c, 0);
    }
    const UnkMinimapMap* map = self->map;
    UnkMinimapVec3 pos = fn_7100206154(self->camera->field_0x20);
    UnkMinimapCameraSub* sub = self->camera->field_0x130;
    f32 scale_y = sub->field_0xc4;
    f32 scale_x = sub->field_0xcc;
    UnkMinimapVec2 size(0.0f, 0.0f);
    fn_71001bb54c(sub, &size);
    f32 x = pos.x;
    f32 y = pos.y;
    UnkMinimapVec2 origin = map->origin;
    f32 half_y = scale_y * (size.y * 0.5f);
    f32 half_x = scale_x * (size.x * 0.5f);
    f32 left = x - half_x;
    f32 right = x + half_x;
    f32 bottom = y - half_y;
    f32 top = y + half_y;
    UnkMinimapVec2 lo(left - origin.x, bottom - origin.y);
    UnkMinimapVec2 hi(right - origin.x, top - origin.y);
    f32 cell = g_unk_71729cdaa0.cell_size;
    UnkMinimapRect rect((lo.x / cell + 0.0f) / 1024.0f, (lo.y / cell + 0.0f) / 512.0f, hi.x / cell / 1024.0f,
                        hi.y / cell / 512.0f);
    fn_7100219294(g_unk_7101d47300, 0x604391e752cd5cbd, &rect.min_x, 6);
}

void fn_7100148160(UnkMinimapCamera* object, u32 layer, const UnkMinimapSpan& a, const UnkMinimapSpan& b,
                   const UnkMinimapSpan& c);
void fn_7100148178(UnkMinimapCamera* object, u32 layer, const UnkMinimapSpan& a, bool flag);

// Draws the two passes: each scene object's two slots off, then every item of the pass into the
// emptied draw lists, which go to the object with the pass layer.
void fn_7100e93e04(UnkMinimapOwner* self) {
    if (!self->pass_objects[1] || !self->map)
        return;
    fn_71001be430(self->pass_objects[0]->field_0x130, 0, false);
    fn_71001be430(self->pass_objects[0]->field_0x130, 1, false);
    fn_71001be430(self->pass_objects[1]->field_0x130, 0, false);
    fn_71001be430(self->pass_objects[1]->field_0x130, 1, false);
    for (u32 pass = 0; pass < 2; ++pass) {
        UnkMinimapDrawList* lists = g_unk_71729cdaa0.lists;
        lists[0].count = 0;
        lists[1].count = 0;
        lists[2].count = 0;
        lists[3].count = 0;
        for (UnkMinimapItem* item : self->pass_items[pass])
            fn_7100e964fc(self, item, 900.0f);
        fn_7100148160(self->pass_objects[pass], self->pass_layer, lists[0], lists[1], lists[2]);
        fn_7100148178(self->pass_objects[pass], self->pass_layer, lists[3], true);
        fn_71001be430(self->pass_objects[pass]->field_0x130, self->pass_layer, true);
        fn_7100149490(self->pass_objects[pass], true);
    }
}

extern const u32 g_unk_7101648bb8;  // the screen's width and height in pixels
extern const u32 g_unk_7101648bbc;
extern bool g_unk_71727cc998 HIDDEN;

f32 fn_7100132ff0();
u32 fn_7100132fc0();
f32 fn_7100132fe0();
f32 fn_71001d7320(UnkGlobal_710b372a98* self);
struct UnkStruct_710020a5a0;
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020a4c4(void* node, const UnkMinimapVec3& pos);

// Fits the map's viewport to the screen: the size ratios go to the render manager and the view, the
// node is scaled and placed, and outside the editor the render manager gets the map's opacity (1
// unless hidden) and its rectangle.
// NON_MATCHING(minor): register allocation: the scaled height lands in s3 (the original reuses s0).
void fn_7100e8f6bc(UnkMinimapOwner* self) {
    f32 screen_w = g_unk_7101648bb8;
    f32 screen_h = g_unk_7101648bbc;
    f32 width = self->size.x;
    f32 height = self->size.y;
    f32 screen_aspect = screen_w / screen_h;
    f32 aspect = width / height;
    f32 base_scale, fx, fy;
    if (screen_aspect < aspect) {
        base_scale = width / screen_w;
        fx = screen_aspect * fn_7100132ff0();
        fy = screen_aspect / aspect;
    } else {
        base_scale = height / screen_h;
        fy = 1.0f;
        fx = aspect / screen_aspect;
    }
    f32 sx = fx * base_scale;
    f32 sy = fy * base_scale;
    f32 ratios[4] = {width, height, sx, sy};
    fn_7100219294(g_unk_7101d47300, 0xfad8a189175fde2e, ratios, 6);
    if (!fn_710002fb70(self->timer) && g_unk_710b372a98->pad_0x0[0x318367b]) {
        f32 d = fn_71001d7320(g_unk_710b372a98);
        sx /= d;
        sy /= d;
    }
    base::math::CVector2D view_size = {sx, sy};
    base::math::CVector2D view_pos = {(1.0f - sx) * 0.5f, (1.0f - sy) * 0.5f};
    self->viewport->SetViewSize(view_size);
    self->viewport->SetViewPos(view_pos);
    UnkMinimapVec2 scale(width / fn_7100132fc0(), height / fn_7100132fc0());
    scale.x *= self->field_0xb4;
    scale.y *= self->field_0xb4;
    fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(self->viewport->node),
                  reinterpret_cast<const base::math::CVector2D*>(&scale));
    {
        UnkMinimapVec2 screen_pos = self->screen_pos;
        f32 k = fn_7100132ff0();
        fn_710020a4c4(self->viewport->node, UnkMinimapVec3(screen_pos.x, screen_pos.y * k, 0.0f));
    }
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    f32 cx = (self->screen_pos.x + 1.0f) * 0.5f;
    f32 cy = (self->screen_pos.y + 1.0f) * 0.5f;
    f32 hx = scale.x * 0.5f;
    f32 hy = scale.y * 0.5f * fn_7100132fe0();
    f32 opacity;
    if (!g_unk_71727cc998) {
        opacity = 0.0f;
    } else {
        f32 v;
        if ((!fn_710002fb70(self->timer) && fn_710002fb70(self->timer_0xd63d90)) ||
            (self->field_0xd63f40 && self->field_0xd63f40->target && fn_7100b41e5c(self->field_0xd63f40->target))) {
            v = 0.0f;
        } else {
            v = self->field_0xb8;
            if (self->field_0xcc && !fn_710002fb70(self->timer))
                v *= self->field_0xd641c0;
        }
        opacity = v > 0.0f ? 1.0f : 0.0f;
    }
    f32 params[5] = {opacity, cx - hx, cy - hy, hx + hx, hy + hy};
    fn_7100219294(g_unk_7101d47300, 0x12194b8295e98a84, params, 0);
}

const base::math::CVector3D* fn_7100b37b80(const void* kind);

inline UnkMinimapVec3 operator*(const base::math::CVector3D& v, f32 s) { return UnkMinimapVec3(v.x * s, v.y * s, v.z * s); }

// The colour and the word fn_7100eb8a20 takes with a marker's shape.
struct UnkMinimapMarkerColor {
    f32 r, g, b, a;
    UnkMinimapMarkerColor(const UnkMinimapVec3& rgb, f32 a_) : r(rgb.x), g(rgb.y), b(rgb.z), a(a_) {}
};
struct UnkMinimapMarkerArg {
    u64 field_0x0;
    u32 field_0x8;
    UnkMinimapMarkerArg(u32 value) : field_0x0(0), field_0x8(value) {}
};
void fn_7100eb8a20(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   const void* shape, const UnkMinimapMarkerColor& color, const UnkMinimapMarkerArg& arg);

// Draws the markers whose box overlaps the view, at 30% of their kind's colour, unless the in-game
// menu's HUD object (game/UnkGameManager.h, fn_7100deac84) pauses the map.
void fn_7100ea3f7c(UnkMinimapOwner* self) {
    UnkIngameMenuHud* pause = fn_7100deac84(g_unk_7101d47310)->field_0x2d0;
    if (pause && pause->field_0x236)
        return;
    for (UnkMinimapMarker* marker : self->markers) {
        if (marker->box.max_x >= self->view.min_x && marker->box.min_x <= self->view.max_x &&
            marker->box.max_y >= self->view.min_y && marker->box.min_y <= self->view.max_y) {
            UnkMinimapDrawList* lists = g_unk_71729cdaa0.lists;
            fn_7100eb8a20(&lists[0], &lists[3], &lists[1], marker->shape,
                          UnkMinimapMarkerColor(*fn_7100b37b80(marker->kind) * 0.3f, 0.0f),
                          UnkMinimapMarkerArg(marker->field_0x78));
        }
    }
}

void fn_7100eb8cf0(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   const void* shape, UnkMinimapColor color, bool flag, f32 size);

// Draws every region's points, then the outline segments that overlap the view (kind 2 dimmed to
// three quarters, the others at 5% opacity).
// NON_MATCHING(minor): loop strength reduction bases the segment pointer at +0x18 (the original at
// +0xc, so other load offsets), the colour's address is formed after the branch and the points
// loop's first two instructions are swapped. The asm keeps the box address live across the colour
// assignment as the original does.
void fn_7100ea4504(UnkMinimapOwner* self) {
    const UnkMinimapRect& view = self->view;
    for (UnkMinimapRegion& region : self->regions) {
        for (UnkMinimapPoint& point : region.points) {
            UnkMinimapDrawList* lists = g_unk_71729cdaa0.lists;
            fn_7100eb8cf0(&lists[0], &lists[3], &lists[1], point.pos, UnkMinimapColor(), true, 500.0f);
        }
        for (UnkMinimapShape& shape : region.shapes) {
            UnkMinimapVec3 color = shape.color;
            for (UnkMinimapSegment& segment : shape.segments) {
                if (segment.box.max_x >= view.min_x && segment.box.min_x <= view.max_x &&
                    segment.box.max_y >= view.min_y && segment.box.min_y <= view.max_y) {
                    const UnkMinimapRect* box = &segment.box;
                    asm("" : "+r"(box));
                    UnkMinimapColor c;
                    base::math::CVector4D value;  // uninitialised
                    if (segment.kind == 2) {
                        value.x = color.x * 0.75f;
                        value.y = color.y * 0.75f;
                        value.z = color.z * 0.75f;
                        value.w = 0.75f;
                    } else {
                        value.w = 0.05f;
                        value.x = color.x;
                        value.y = color.y;
                        value.z = color.z;
                    }
                    fn_710007b518(&c, &value);
                    UnkMinimapDrawList* lists = g_unk_71729cdaa0.lists;
                    fn_7100eb8cf0(&lists[0], &lists[3], &lists[1], box, c, true, 500.0f);
                }
            }
        }
    }
}

// The ids of the icon kinds that fn_7100e9d1f8 can show again are game/UnkMinimapIconKinds.h's.
void fn_71000ce604(base::UnkVector<base::global::CStrId>* self, base::global::CStrId* it);  // erases
void fn_7100e957a8(UnkMinimapOwner* self);

// Removes a kind from the hidden ones (`count` ids at `data`) and refreshes the map when it was
// there.
__attribute__((always_inline)) inline void UnkMinimapShowKind(UnkMinimapOwner* self,
                                                             base::UnkVector<base::global::CStrId>* kinds,
                                                             base::global::CStrId* data, u32 count,
                                                             const base::global::CStrId& kind) {
    base::global::CStrId* it = data;
    for (u64 left = count; left != 0; --left, ++it) {
        if (it->data == kind.data)
            break;
    }
    if (it != data + count) {
        fn_71000ce604(kinds, it);
        fn_7100e957a8(self);
    }
}

// Shows an icon kind again (by its number, 1 to 9; 8 has none) while an area is loaded.
// NON_MATCHING(minor): block layout: the original sends an empty or exhausted search straight to
// the return; ours sends it through the found-position check (which then fails), and the loop
// setup's two copies swap places.
void fn_7100e9d1f8(UnkMinimapOwner* self, s32 kind) {
    if (self->field_0x110.data == &g_unk_7101d47108->empty)
        return;
    base::UnkVector<base::global::CStrId>* kinds = &self->hidden_kinds;
    base::global::CStrId* data = kinds->data;
    u32 count = kinds->count;
    switch (kind) {
    case 1:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80b0);
        break;
    case 2:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80b8);
        break;
    case 3:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80c0);
        break;
    case 4:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80c8);
        break;
    case 5:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80d0);
        break;
    case 6:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80d8);
        break;
    case 7:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80e0);
        break;
    case 9:
        UnkMinimapShowKind(self, kinds, data, count, g_unk_71727c80e8);
        break;
    }
}
