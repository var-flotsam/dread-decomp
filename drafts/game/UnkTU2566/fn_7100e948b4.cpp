// Unit 2566 (CMinimapManager::CTunableMinimapManager): the minimap's view rectangle: the camera's
// position (less the map's offset unless it is kept) with the screen-scaled view size around it; the
// rectangle saved last when the timer's state differs from `flag`, and a new one saved when neither is
// set. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU39.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

typedef u32 __attribute__((may_alias)) a_u32;

// The owner and the types it holds, as src/game/CMinimapManager.cpp defines them.
struct UnkMinimapItem;
struct UnkMinimapMarker;
struct UnkMinimapRegion;
struct UnkMinimapRef;
struct UnkMinimapViewport;
struct UnkMinimapMap;

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

struct UnkMinimapIconDef {
    u8 unk_0x0[0xb0];
    bool sized_by_box;
    bool alt_scale;
    u8 unk_0xb2[0x2];
    f32 margins[4];
};

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

// An array whose count follows its data pointer by 0x10 (the word between is unknown).
template <class T>
struct UnkMinimapArray {
    T* data;
    u64 field_0x8;
    u32 count;

    T* begin() const { return data; }
    T* end() const { return data + count; }
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
    base::global::CRntVector<base::global::CStrId> hidden_kinds;
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

bool fn_71001bb54c(UnkMinimapCameraSub* sub, UnkMinimapVec2* size);

extern const u32 g_unk_7101648bb8;  // the screen's width and height in pixels
extern const u32 g_unk_7101648bbc;

// Members of the owner its definition leaves as bytes: the view saved last (+0xd63d58), whether the
// offset is kept (+0xd641c6) and the view size the saved view is made with (+0xd641c8).
static UnkMinimapRect& SavedView(UnkMinimapOwner* self) {
    return *reinterpret_cast<UnkMinimapRect*>(self->unk_0xd63d58);
}
static bool KeepOffset(const UnkMinimapOwner* self) { return self->unk_0xd641c4[2]; }
static const UnkMinimapVec2& SavedSize(const UnkMinimapOwner* self) {
    return *reinterpret_cast<const UnkMinimapVec2*>(&self->unk_0xd641c4[4]);
}

UnkMinimapRect fn_7100e948b4(UnkMinimapOwner* self, bool flag) {
    if (fn_710002fb70(self->timer) != flag)
        return SavedView(self);

    UnkMinimapVec2 size(0.0f, 0.0f);
    fn_71001bb54c(self->camera->field_0x130, &size);
    f32 screen_w = g_unk_7101648bb8;
    f32 width = size.x * self->size.x / screen_w;
    f32 screen_h = g_unk_7101648bbc;
    f32 height = size.y * self->size.y / screen_h;

    CVector2D center;
    UnkVec3f pos = fn_7100206154(reinterpret_cast<UnkXformNode*>(self->camera->field_0x20));
    fn_710007b430(&center, reinterpret_cast<const CVector3D*>(&pos));
    if (!flag) {
        center.x -= self->offset.x;
        center.y -= self->offset.y;
        if (KeepOffset(self)) {
            center.x += self->offset.x;
            center.y += self->offset.y;
        }
    }
    f32 half_w = width * 0.5f;
    f32 half_h = height * 0.5f;
    f32 left = center.x - half_w;
    f32 bottom = center.y - half_h;
    f32 right = center.x + half_w;
    f32 top = center.y + half_h;

    if (!flag && !fn_710002fb70(self->timer)) {
        const UnkMinimapVec2& saved_size = SavedSize(self);
        f32 saved_w = size.x * saved_size.x / screen_w * 0.5f;
        f32 saved_h = size.y * saved_size.y / screen_h * 0.5f;
        UnkMinimapRect& saved = SavedView(self);
        saved.min_x = center.x - saved_w;
        saved.min_y = center.y - saved_h;
        saved.max_x = center.x + saved_w;
        saved.max_y = center.y + saved_h;
    }
    return UnkMinimapRect(left, bottom, right, top);
}
