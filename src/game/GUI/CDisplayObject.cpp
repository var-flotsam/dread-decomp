// GUI::CDisplayObject's unit (the class name from reflection; layout in game/UnkGuiDisplayObject.h):
// applying a list of 0x18-byte entries to an object one by one, each through an iterator over the
// list, and telling the object once if any of them changed it. Names are placeholders.
#include <new>

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiListEntry {
    u8 data[0x18];
};

struct UnkGuiEntryList {
    UnkGuiListEntry* data;
    u32 count;
};

struct UnkGuiEntryIter {
    const UnkGuiEntryList* list;
    UnkGuiListEntry* entry;
};

// Applies the entry the iterator is at; true when that changed the object.
bool fn_71011cd5dc(UnkGuiDisplayObject* self, const UnkGuiEntryIter* it);

void fn_71011cd424(UnkGuiDisplayObject* self, const UnkGuiEntryList* list) {
    UnkGuiEntryIter it{list, list->data};
    bool changed = false;
    for (UnkGuiListEntry* entry = it.entry; entry != list->data + list->count;) {
        changed |= fn_71011cd5dc(self, &it);
        it.entry = ++entry;
    }
    if (changed)
        self->vf_0x80();
}

// Sets the flag at +0x185; a change runs vf_0xa0 the first time, marks the derived flags stale and
// sets change bit 4.
void fn_71011d0088(UnkGuiDisplayObject* self, bool flag) {
    if (self->field_0x185 != flag) {
        self->field_0x185 = flag;
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
        self->field_0x183 = true;
        self->field_0x144 |= 4;
    }
}

// The conditions an object checks (+0x100: how many, from +0x104, an unaligned array of pointers),
// each through its vf_0x20; and its state object at +0x30. Only these fields are laid out here.
struct UnkGuiCondition {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};
struct UnkGuiObjectSub_0x30 {
    u8 pad_0x0[0x11];
    bool field_0x11;
};
struct UnkGuiConditionView {
    u8 pad_0x0[0x30];
    UnkGuiObjectSub_0x30* field_0x30;
    u8 pad_0x38[0xc8];
    u32 condition_count;  // 0x100
    u8 conditions[1];     // 0x104

    UnkGuiCondition** begin() { return reinterpret_cast<UnkGuiCondition**>(conditions); }
    UnkGuiCondition** end() { return begin() + condition_count; }
};

// Whether any of the object's conditions fails.
bool fn_71011ceac0(UnkGuiDisplayObject* self) {
    UnkGuiConditionView* view = reinterpret_cast<UnkGuiConditionView*>(self);
    for (UnkGuiCondition** it = view->begin(); it != view->end(); ++it)
        if (!(*it)->vf_0x20())
            return true;
    return false;
}

// Whether the object has a state object at +0x30 whose flag at +0x11 is clear.
bool fn_71011ceb34(UnkGuiDisplayObject* self) {
    UnkGuiConditionView* view = reinterpret_cast<UnkGuiConditionView*>(self);
    if (view->field_0x30)
        return !view->field_0x30->field_0x11;
    return false;
}

// ---- fn_71011cce1c: the constructor ----

#include "base/global/CStrId.h"

using base::global::CStrId;

// A display state (the 0x40 bytes of src/game/UnkGuiItemState.cpp): no anchoring, no angle, unit
// size and scale, depth 0, white, the parent's frame (INT32_MAX), active and shown.
struct UnkGuiStateInit {
    u32 anchor[4];
    f32 angle;
    f32 width;
    f32 height;
    f32 scale_x;
    f32 scale_y;
    f32 depth;
    f32 color[4];
    s32 frame;
    bool field_0x3c;
    bool field_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
};

extern char g_unk_7101cb5b90[];  // CDisplayObject's vtable
void fn_7101043904(void* self);  // the base's constructor
void fn_7100b1df48(void* slots);  // the slots at +0xb8 (game/UnkRenderPassSlots.h)
void fn_710007e6c4(void* self);  // an empty CRntString

struct UnkGuiDisplayObjectInit {
    char* vtable;
    u8 base_0x8[0x8];
    bool field_0x10;
    u8 pad_0x11[0x7];
    CStrId field_0x18;
    CStrId field_0x20;
    u64 field_0x28[2];
    UnkGuiStateInit state;    // 0x38
    UnkGuiStateInit derived;  // 0x78
    u8 slots[0x48];           // 0xb8
    u32 field_0x100;
    u8 pad_0x104[0x1c];
    u64 field_0x120;
    u64 field_0x128;
    base::global::UnkStringData* field_0x130;  // an empty string id
    u8 pad_0x138[0x4];
    u32 field_0x13c;
    u8 pad_0x140[0x4];
    s32 field_0x144;
    u64 field_0x148;
    u8 name[0x30];  // 0x150
    u8 field_0x180;
    u8 field_0x181;
    u8 field_0x182;
    u8 field_0x183;
    u8 field_0x184;
    u8 pad_0x185;
    u16 field_0x186;
    u8 field_0x188;
};
static_assert(__builtin_offsetof(UnkGuiDisplayObjectInit, field_0x188) == 0x188, "UnkGuiDisplayObjectInit layout");

static inline void InitState(UnkGuiStateInit* s) {
    s->anchor[0] = 0;
    s->anchor[1] = 0;
    s->anchor[2] = 0;
    s->anchor[3] = 0;
    s->angle = 0.0f;
    s->width = 1.0f;
    s->height = 1.0f;
    s->scale_x = 1.0f;
    s->scale_y = 1.0f;
    s->depth = 0.0f;
    s->color[0] = 1.0f;
    s->color[1] = 1.0f;
    s->color[2] = 1.0f;
    s->color[3] = 1.0f;
    s->frame = 0x7fffffff;
    s->field_0x3c = true;
    s->field_0x3d = true;
    s->field_0x3e = false;
}

void fn_71011cce1c(UnkGuiDisplayObject* object) {
    auto* self = reinterpret_cast<UnkGuiDisplayObjectInit*>(object);
    fn_7101043904(self);
    self->vtable = g_unk_7101cb5b90 + 0x10;
    self->field_0x10 = false;
    new (&self->field_0x18) CStrId("", true);
    new (&self->field_0x20) CStrId("", true);
    self->field_0x28[0] = 0;
    self->field_0x28[1] = 0;
    InitState(&self->state);
    InitState(&self->derived);
    fn_7100b1df48(self->slots);
    self->field_0x100 = 0;
    self->field_0x130 = nullptr;
    self->field_0x120 = 0;
    self->field_0x128 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x13c = 0;
    self->field_0x130 = &g_unk_7101d47108->empty;
    self->field_0x144 = -1;
    self->field_0x148 = 0;
    fn_710007e6c4(self->name);
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x182 = 0;
    self->field_0x183 = 1;
    self->field_0x184 = 1;
    self->field_0x186 = 1;
    self->field_0x188 = 0;
}

// ---- fn_71011cde64: placing the object ----

#include "base/math/CVector.h"

extern const f32 g_unk_7101647774;  // the tolerance the scale is compared to 1 with

// The placement as the setter sees it: the two modes and the position, and the scale after them.
struct UnkGuiPlacement {
    u8 pad_0x0[0x78];
    u32 h_mode;  // 0x78
    u32 v_mode;  // 0x7c
    base::math::CVector2D position;  // 0x80
    u8 pad_0x88[0xc];
    f32 scale_x;  // 0x94
    f32 scale_y;  // 0x98
};

// Whether `x` lies outside 1 +- `eps` (false for NaN).
static inline bool AwayFromOne(f32 x, f32 eps) {
    return !(x - eps <= 1.0f) || x + eps < 1.0f;
}

// NON_MATCHING(minor): the original repeats the first-change call in each of the two branches of the
// scale test and loads the tolerance after the x scale; the behaviour is the same.
void fn_71011cde64(UnkGuiDisplayObject* self, const base::math::CVector2D* position, u32 h_mode, u32 v_mode) {
    UnkGuiPlacement* p = reinterpret_cast<UnkGuiPlacement*>(self);
    if (p->h_mode == h_mode && p->v_mode == v_mode && position->x - p->position.x == 0.0f &&
        position->y - p->position.y == 0.0f)
        return;
    p->position.x = position->x;
    p->position.y = position->y;
    p->h_mode = h_mode;
    p->v_mode = v_mode;
    // A unit scale (within the tolerance) changes only the position (bit 0); any other also the
    // size (bit 4).
    f32 eps = g_unk_7101647774;
    u32 bits = AwayFromOne(p->scale_x, eps) || AwayFromOne(p->scale_y, eps) ? 0x11 : 1;
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
    self->field_0x183 = true;
    self->field_0x144 |= bits;
}

// Marks the object changed: a unit scale (within the tolerance) changes only the position (bit 0),
// any other also the size (bit 4); the first change runs vf_0xa0.
static inline void MarkPlaced(UnkGuiDisplayObject* self, const UnkGuiPlacement* p) {
    f32 sx = p->scale_x;
    f32 eps = g_unk_7101647774;
    u32 bits;
    if (sx - eps > 1.0f || sx + eps < 1.0f || p->scale_y - eps > 1.0f || eps + p->scale_y < 1.0f) {
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
        bits = 0x11;
    } else {
        bits = 1;
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
    }
    self->field_0x183 = true;
    self->field_0x144 |= bits;
}

// Sets one coordinate and the mode it is measured by (the other coordinate is kept as it is; the
// unchanged test compares it with itself, so a NaN there always counts as a change).
template <u32 Mode>
__attribute__((always_inline)) inline void SetX(f32 x, UnkGuiDisplayObject* self) {
    UnkGuiPlacement* p = reinterpret_cast<UnkGuiPlacement*>(self);
    if (p->h_mode == Mode && p->position.y - p->position.y == 0.0f && x - p->position.x == 0.0f)
        return;
    p->position.x = x;
    p->h_mode = Mode;
    MarkPlaced(self, p);
}

template <u32 Mode>
__attribute__((always_inline)) inline void SetY(f32 y, UnkGuiDisplayObject* self) {
    UnkGuiPlacement* p = reinterpret_cast<UnkGuiPlacement*>(self);
    if (p->v_mode == Mode && p->position.x - p->position.x == 0.0f && y - p->position.y == 0.0f)
        return;
    p->position.y = y;
    p->v_mode = Mode;
    MarkPlaced(self, p);
}

// NON_MATCHING(minor), each of the eight: the original lays out the first-change call of the
// non-unit-scale branch inline and its other copy at the end; the behaviour is the same.
void fn_71011cc65c(f32 x, UnkGuiDisplayObject* self) { SetX<0>(x, self); }
void fn_71011cc940(f32 x, UnkGuiDisplayObject* self) { SetX<1>(x, self); }
void fn_71011cc748(f32 x, UnkGuiDisplayObject* self) { SetX<2>(x, self); }
void fn_71011cc844(f32 x, UnkGuiDisplayObject* self) { SetX<3>(x, self); }
void fn_71011cca3c(f32 y, UnkGuiDisplayObject* self) { SetY<0>(y, self); }
void fn_71011ccd20(f32 y, UnkGuiDisplayObject* self) { SetY<1>(y, self); }
void fn_71011ccb28(f32 y, UnkGuiDisplayObject* self) { SetY<2>(y, self); }
void fn_71011ccc24(f32 y, UnkGuiDisplayObject* self) { SetY<3>(y, self); }
