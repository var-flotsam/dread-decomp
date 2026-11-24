// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): makes an area marker the current one:
// the previous one and the current-area highlight are told first, the world is told the area's name
// and the areas container is placed (centred on the marker for the sky map). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CGlobalMapComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiPos.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2755.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CStrId;

// An area marker on the map.
struct UnkMapMarker_lb26470 {
    u8 pad_0x0[0xb5];
    bool selectable;  // 0xb5
    u8 pad_0xb6[0x7a];
    CStrId map;  // 0x130
    u8 pad_0x138[0x158];
    CStrId name;  // 0x290
};
static_assert(__builtin_offsetof(UnkMapMarker_lb26470, name) == 0x290, "UnkMapMarker_lb26470 layout");

void fn_7100b22750(UnkMapMarker_lb26470* marker);
void fn_7100b226dc(UnkMapMarker_lb26470* marker);
// The world takes the current area's name.
void fn_7100e98658(void* world, const CStrId& area);

struct UnkMapSoundState_lb26470 {
    u8 pad_0x0[0x10];
    s32 value;  // 0x10
};

struct UnkMapComposition_lb26470 {
    UnkGuiDisplayObject base;
    u8 pad_0x198[0x248 - sizeof(UnkGuiDisplayObject)];
    UnkMapSoundState_lb26470* sound_state;  // 0x248
    u8 pad_0x250[0x390];
    UnkGuiDisplayObject* areas;              // 0x5e0
    UnkMapMarker_lb26470* current;           // 0x5e8
    UnkGuiDisplayObject* highlight;          // 0x5f0
    UnkGuiDisplayObject* view;               // 0x5f8
};
static_assert(__builtin_offsetof(UnkMapComposition_lb26470, view) == 0x5f8, "UnkMapComposition_lb26470 layout");

// Plays an event of a display object's animation (vtable slot 0x1e8).
static inline void PlayEvent(UnkGuiDisplayObject* object, const char* event, bool flag) {
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, const char*, bool)>(object))[0x1e8 / 8](object, event, flag);
}

// The anchor state of a display object (+0x78), as fn_71011f6240 takes it.
static inline UnkGuiItemState* AnchorState(UnkGuiDisplayObject* object) {
    return &object->field_0x78;
}

// An anchor point is the same pair of floats as the point type fn_71011cf0a8 takes.
static inline const UnkVec2Result& AsPoint(const UnkGuiPos& pos) {
    return reinterpret_cast<const UnkVec2Result&>(pos);
}

void fn_7100b26470(UnkGlobalMapComposition_b2b868* composition, void* object, bool flag) {
    UnkMapComposition_lb26470* self = reinterpret_cast<UnkMapComposition_lb26470*>(composition);
    UnkMapMarker_lb26470* marker = static_cast<UnkMapMarker_lb26470*>(object);
    if (!marker || !marker->selectable)
        return;
    if (self->current && flag)
        fn_7100b22750(self->current);
    if (self->highlight && flag) {
        fn_71011cdc30(self->highlight, true);
        PlayEvent(self->highlight, "OnAreaCurrent", false);
    }
    self->current = marker;
    if (flag) {
        fn_7100b226dc(marker);
        if (self->sound_state && self->sound_state->value != 0x7fffffff)
            fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0188, self);
    }
    if (self->base.field_0xb4) {
        void* world = g_unk_7101d47310->field_0x2228;
        CStrId name(self->current->name);
        fn_7100e98658(world, name);
    }

    UnkMapMarker_lb26470* current = self->current;
    if (current && current->map == CStrId("MAP_SKYB", true)) {
        // Centre the areas on the marker.
        UnkVec2Result point = fn_71011cf0a8(self->areas, AsPoint(fn_71011f6240(AnchorState(self->view), 1, 1)));
        f32 dx = 0.5f - point.x;
        f32 dy = 0.5f - point.y;
        UnkGuiDisplayObject* areas = self->areas;
        base::math::CVector2D position;
        UnkGuiPos origin = fn_71011f6240(AnchorState(areas), 0, 0);
        position.x = dx + origin.x;
        position.y = dy + origin.y;
        fn_71011cde64(areas, &position, 0, 0);
    } else {
        UnkGuiDisplayObject* areas = self->areas;
        base::math::CVector2D position = {0.0f, 0.05f};
        fn_71011cde64(areas, &position, 0, 0);
    }
    fn_71011cd5a0(self->areas);
}
