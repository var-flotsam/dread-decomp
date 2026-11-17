// GUI::CGlobalMapComposition (unit 1680): lays out the global map. The five area labels are read once
// and placed again with small offsets; the glow and highlight of one area are reset and two lines are
// nudged from their first positions. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2755.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

struct UnkMapComposition_ovb0109;
void fn_7100b2af40(UnkMapComposition_ovb0109* self, const CStrId& name, const base::math::CVector2D* position);
UnkVec2Result fn_7100b2b074(UnkMapComposition_ovb0109* self, const CStrId& name);

struct UnkGuiItemState;
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

struct UnkGlobalMapLayout_lb259c8 {
    u8 pad_0x0[0x5e0];
    UnkGuiDisplayObject* areas;  // 0x5e0: the "AreasComposition" container
};

template <const base::reflection::CClass* (*GetBase)()>
static inline bool IsA(UnkGuiDisplayObject* o) {
    const base::reflection::CClass* cls = o->GetClass();
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                         reinterpret_cast<const base::reflection::CType*>(GetBase()));
}

static inline UnkGuiDisplayObject* AsContainer(UnkGuiDisplayObject* o) {
    return o && IsA<fn_71011d7b84>(o) ? o : nullptr;
}

static inline UnkGuiDisplayObject* AsSprite(UnkGuiDisplayObject* o) {
    return o && IsA<fn_71011f6890>(o) ? o : nullptr;
}

// The anchor state of a display object (+0x78), as fn_71011f6240 takes it.
static inline UnkGuiItemState* AnchorState(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkGuiItemState*>(reinterpret_cast<u8*>(object) + 0x78);
}

static inline void Place(UnkGuiDisplayObject* object, const CVector2D& position) {
    fn_71011cde64(object, &position, 0, 0);
    fn_71011cd5a0(object);
}

// Where a label is now, kept as a plain vector.
static inline void ReadLabel(UnkMapComposition_ovb0109* map, const CStrId& name, CVector2D* out) {
    UnkVec2Result p = fn_7100b2b074(map, name);
    *out = {p.x, p.y};
}

// Places a label at where it first was, moved by (dx, dy).
static inline void PlaceLabel(UnkMapComposition_ovb0109* map, const CStrId& name, const CVector2D& first, f32 dx,
                              f32 dy) {
    CVector2D pos = {first.x + dx, first.y + dy};
    fn_7100b2af40(map, name, &pos);
}

void fn_7100b259c8(UnkGlobalMapLayout_lb259c8* self) {
    UnkMapComposition_ovb0109* map = reinterpret_cast<UnkMapComposition_ovb0109*>(self);

    static const CStrId s_cave("MAP_CAV", true);
    static const CStrId s_magma("MAP_MAG", true);
    static const CStrId s_forest("MAP_FOR", true);
    static const CStrId s_sanctuary("MAP_SANC", true);
    static const CStrId s_ship("MAP_SHIP", true);
    static bool s_read = false;
    static CVector2D s_labels[5];  // where each label first was
    if (!s_read) {
        s_read = true;
        ReadLabel(map, s_cave, &s_labels[0]);
        ReadLabel(map, s_magma, &s_labels[1]);
        ReadLabel(map, s_forest, &s_labels[2]);
        ReadLabel(map, s_sanctuary, &s_labels[3]);
        ReadLabel(map, s_ship, &s_labels[4]);
    }

    PlaceLabel(map, s_cave, s_labels[0], 0.001875f, 0.001875f);
    PlaceLabel(map, s_magma, s_labels[1], 0.005f, -0.001875f);
    PlaceLabel(map, s_forest, s_labels[2], -0.001875f, -0.005f);
    PlaceLabel(map, s_sanctuary, s_labels[3], -0.003125f, 0.0f);
    PlaceLabel(map, s_ship, s_labels[4], 0.001875f, 0.0f);

    {
        static const CStrId s_glow("Glow", true);
        static const CStrId s_highlight("Highlight", true);
        if (UnkGuiDisplayObject* magma = AsContainer(fn_71011d94d4(self->areas, s_magma))) {
            CVector2D zero = {0.0f, 0.0f};
            if (UnkGuiDisplayObject* glow = AsSprite(fn_71011d94d4(magma, s_glow)))
                Place(glow, zero);
            if (UnkGuiDisplayObject* highlight = AsSprite(fn_71011d94d4(magma, s_highlight)))
                Place(highlight, zero);
        }
    }

    {
        static const CStrId s_area("C_CAV-AQUA-3", true);
        static const CStrId s_line("Line (1)", true);
        if (UnkGuiDisplayObject* area = AsContainer(fn_71011d94d4(self->areas, s_area))) {
            if (UnkGuiDisplayObject* line = AsSprite(fn_71011d94d4(area, s_line))) {
                static bool s_lineRead = false;
                static CVector2D s_linePos;
                if (!s_lineRead) {
                    s_lineRead = true;
                    UnkGuiPos p = fn_71011f6240(AnchorState(line), 0, 0);
                    s_linePos = {p.x, p.y};
                }
                CVector2D linePos = {s_linePos.x + 0.0f, s_linePos.y + 0.00125f};
                Place(line, linePos);
            }
        }
    }

    {
        static const CStrId s_area("C_CAV-MAG-1", true);
        static const CStrId s_line("Line (1)", true);
        if (UnkGuiDisplayObject* area = AsContainer(fn_71011d94d4(self->areas, s_area))) {
            if (UnkGuiDisplayObject* line = AsSprite(fn_71011d94d4(area, s_line))) {
                static bool s_lineRead = false;
                static CVector2D s_linePos;
                if (!s_lineRead) {
                    s_lineRead = true;
                    UnkGuiPos p = fn_71011f6240(AnchorState(line), 0, 0);
                    s_linePos = {p.x, p.y};
                }
                CVector2D linePos = {s_linePos.x + 0.0f, s_linePos.y + 0.00125f};
                Place(line, linePos);
            }
        }
    }
}
