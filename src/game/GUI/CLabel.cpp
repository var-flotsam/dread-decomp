// From unit 2752 of tools/tu_map.py (0x71011e8df8-0x71011f0810, GUI::CLabel by reflection): the
// label's per-frame refresh of the viewports its groups' items show. Names and layouts are
// placeholders; the viewport is src/game/CMinimapManager.cpp's (a scene item at +0x18 whose flag
// at 0xfb, the viewport's +0x113, marks a pending update, and the view at +0x178 whose rectangle
// fn_71001c36c0 applies).
// Holds fn_71011ee5a8.
#include "base/math/CVector.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::math::CVector2D;

// The view (the same definition as src/game/CMinimapManager.cpp's).
struct UnkMinimapView {
    u8 unk_0x0[0x90];
    base::math::CVector2D pos;
    base::math::CVector2D size;
};
void fn_71001c36c0(UnkMinimapView* view, const base::math::CVector2D* pos, const base::math::CVector2D* size);
void fn_71001c3770(UnkMinimapView* view, bool value);

struct UnkStruct_710020a5a0;
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020ad90(void* self, s32 value);

// A viewport an entry shows (laid out as src/game/CMinimapManager.cpp's UnkMinimapViewport).
struct UnkLabelViewport {
    u8 unk_0x0[0x18];
    UnkToggle node;  // its flag_0xfb is set once the view is moved or resized
    u8 unk_0x128[0x50];
    UnkMinimapView* view;
};
static_assert(__builtin_offsetof(UnkLabelViewport, view) == 0x178, "UnkLabelViewport placeholder layout");

// A 2D value the scale helpers hand back in memory (copied member by member).
struct UnkLabelVec2 {
    f32 x;
    f32 y;
    UnkLabelVec2(const UnkLabelVec2& other) : x(other.x), y(other.y) {}
};

// What the GUI root finds for an entry's key: its rectangle and the object at +0x18 whose +0x8 the
// viewport is built from.
struct UnkLabelSource {
    u8 unk_0x0[0x8];
    CVector2D pos;
    CVector2D size;
    char* field_0x18;
};

// An item's entry (0x30 bytes): a value that is zero until the entry is resolved, the key it is
// looked up by and the viewport built for it, with the frame it was last refreshed in.
struct UnkLabelEntry {
    s32 field_0x0;
    u8 unk_0x4[0x4];
    u8 key[0x1c];
    s32 frame;
    UnkLabelViewport* viewport;
};
static_assert(sizeof(UnkLabelEntry) == 0x30, "UnkLabelEntry placeholder layout");

struct UnkLabelItemSub {
    u8 unk_0x0[0x11b];
    bool field_0x11b;
    u8 unk_0x11c[0x14];
    void* field_0x130;
};

struct UnkLabelItem {
    u8 unk_0x0[0x10];
    s32* field_0x10;
    u8 unk_0x18[0x28];
    UnkLabelEntry* entries;  // +0x40
    u32 count;               // +0x48
    u8 unk_0x4c[0x1c];
    UnkLabelItemSub* field_0x68;
};

struct UnkLabelItemNode {
    UnkLabelItem* item;
    UnkLabelItemNode* next;
};
struct UnkLabelGroup {
    u8 unk_0x0[0x18];
    UnkLabelItemNode* items;
};
struct UnkLabelGroupNode {
    UnkLabelGroup* group;
    UnkLabelGroupNode* next;
};

struct UnkLabel {
    u8 unk_0x0[0x13c];
    s32 field_0x13c;
    s32 field_0x140;  // 0x7fffffff when unset: field_0x13c is used instead
    u8 unk_0x144[0x18c];
    UnkLabelGroupNode* groups;  // +0x2d0
};
static_assert(__builtin_offsetof(UnkLabel, groups) == 0x2d0, "UnkLabel placeholder layout");

void fn_71001c4418(void* self, const s32* value);
UnkLabelSource* fn_71011b9fac(UnkGuiRoot* root, const void* key);
void fn_71011baa38(UnkGuiRoot* root, char* source, UnkLabelViewport** out, s32 frame);
UnkLabelVec2 fn_7100b82b18(const UnkLabelSource* source, s32 frame);
UnkLabelVec2 fn_71011b8160(s32 frame, const UnkLabelVec2& v);
UnkLabelVec2 fn_71011b7f48(s32 frame, const UnkLabelVec2& v);

// The label's depth: field_0x140, or field_0x13c while it is unset.
static inline s32 Depth(const UnkLabel* self) {
    s32 depth = self->field_0x140;
    if (depth == 0x7fffffff)
        depth = self->field_0x13c;
    return depth;
}

static inline void SetViewPos(UnkLabelViewport* viewport, const CVector2D& pos) {
    UnkMinimapView* v = viewport->view;
    v->pos.x = pos.x;
    v->pos.y = pos.y;
    fn_71001c36c0(v, &v->pos, &v->size);
    viewport->node.flag_0xfb = true;
}
static inline void SetViewSize(UnkLabelViewport* viewport, const CVector2D& size) {
    UnkMinimapView* v = viewport->view;
    v->size.x = size.x;
    v->size.y = size.y;
    fn_71001c36c0(v, &v->pos, &v->size);
    viewport->node.flag_0xfb = true;
}

// Refreshes every entry of every item of the label's groups for the current frame (nothing without
// one): an item flagged at +0x10 first updates its object at +0x68; an unresolved entry gets its
// viewport from the GUI root, placed, sized and scaled; then each viewport is shown at the label's
// depth (field_0x140, else field_0x13c) less one. The current frame is the label's derived state's
// (fn_71011cde14, with the label seen as the display object it is).
void fn_71011ee5a8(UnkLabel* self) {
    UnkGuiObjectState* composition = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
    if (composition->frame == 0x7fffffff)
        return;
    for (UnkLabelGroupNode* g = self->groups; g; g = g->next) {
        for (UnkLabelItemNode* n = g->group->items; n; n = n->next) {
            UnkLabelItem* item = n->item;
            if (item->field_0x10 && *item->field_0x10 != 0) {
                UnkLabelItemSub* sub = item->field_0x68;
                fn_71001c4418(sub->field_0x130, item->field_0x10);
                sub->field_0x11b = true;
            }
            for (u64 i = 0; i < item->count; ++i) {
                UnkLabelEntry* e = &item->entries[i];
                if (e->field_0x0 == 0) {
                    if (UnkLabelSource* source = fn_71011b9fac(g_unk_717271e8f8, e->key)) {
                        fn_71011baa38(g_unk_717271e8f8, source->field_0x18 + 8, &e->viewport, composition->frame);
                        if (e->viewport) {
                            SetViewPos(e->viewport, source->pos);
                            SetViewSize(e->viewport, source->size);
                            s32 frame = composition->frame;
                            UnkLabelVec2 scaled = fn_71011b8160(frame, fn_7100b82b18(source, frame));
                            UnkLabelVec2 scale = fn_71011b7f48(composition->frame, scaled);
                            // The scale is passed by value: a copy made after the viewport is read.
                            UnkLabelViewport* viewport = e->viewport;
                            UnkLabelVec2 arg = scale;
                            fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(&viewport->node),
                                          reinterpret_cast<const CVector2D*>(&arg));
                            fn_71001c3770(e->viewport->view, true);
                        }
                    }
                }
                if (e->viewport) {
                    fn_710020ac34(&e->viewport->node, false);
                    e->frame = composition->frame;
                    UnkLabelViewport* viewport = e->viewport;
                    s32 depth = Depth(self) - 1;
                    fn_710020ad90(&viewport->node, depth);
                }
            }
        }
    }
}
