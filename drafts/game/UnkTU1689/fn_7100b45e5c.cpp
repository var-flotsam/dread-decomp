// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): opens a map page: closes
// the menu's pending entry when asked, centres the map on its box, refreshes the menu with the empty id,
// sets the zoom and logs the page's message (kind 2) to the game manager's callback set at +0x2128.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapMenuController;
struct UnkMlogMessage_b42a1;
struct UnkMmOwner_b0199;

// The base handler takes a flag here, where src/game/GUI/CMapMenuController_3.cpp passes an argument pointer: the
// same symbol under a flag signature.
void fn_7100b43b30_flag(UnkMapMenuController* self, const CStrId* id, u32 flag) asm("_Z13fn_7100b43b30P20UnkMapMenuControllerPKN4base6global6CStrIdEPv");
namespace unk_gui {
void fn_7100eb7358(void* object, u32 a, u32 b);
}
void fn_7100e910fc(UnkMmOwner_b0199* self, const f32* pos);

extern CStrId g_unk_7172763df0 __attribute__((visibility("hidden")));

struct UnkMapMenuPending_lb45e5c {
    u8 pad_0x0[0x10];
    s32 entry;  // 0x10
};

struct UnkMapMenu_lb45e5c {
    u8 pad_0x0[0x234];
    s32 field_0x234;
    u8 pad_0x238[0x27c];
    bool field_0x4b4;
    bool field_0x4b5;
    u8 pad_0x4b6[0x162];
    u8 field_0x618[0x310];
    UnkMapMenuPending_lb45e5c* pending;  // 0x928
};

struct UnkMapEntry_lb45e5c {
    u8 pad_0x0[0x10];
    UnkMapMenu_lb45e5c* menu;  // 0x10
};

struct UnkVec2_lb45e5c {
    f32 x, y;
};

static inline UnkVec2_lb45e5c operator+(const UnkVec2_lb45e5c& a, const UnkVec2_lb45e5c& b) {
    UnkVec2_lb45e5c r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

static inline UnkVec2_lb45e5c operator*(const UnkVec2_lb45e5c& a, f32 s) {
    UnkVec2_lb45e5c r;
    r.x = a.x * s;
    r.y = a.y * s;
    return r;
}

// The map's view: its box and the menu entry it shows.
struct UnkMapView_lb45e5c {
    UnkVec2_lb45e5c min;  // 0x0
    UnkVec2_lb45e5c max;  // 0x8
    u8 pad_0x10[0x1d8];
    UnkMapEntry_lb45e5c* entry;  // 0x1e8
};

struct UnkMapData_lb45e5c {
    u8 pad_0x0[0xc8];
    f32 zoom;  // 0xc8
    u8 pad_0xcc[0xd63d58 - 0xcc];
    UnkMapView_lb45e5c view;  // 0xd63d58
};
static_assert(__builtin_offsetof(UnkMapData_lb45e5c, view.entry) == 0xd63f40, "UnkMapData_lb45e5c layout");

struct UnkWorld_lb45e5c {
    u8 pad_0x0[0xbc];
    s32 page;  // 0xbc
};

struct UnkMapPage_lb45e5c {
    u8 pad_0x0[0x8];
    bool field_0x8;
    u8 pad_0x9[0xdf];
    s32 page;  // 0xe8
    bool closePending;  // 0xec
    u8 pad_0xed[0x3];
    s32 state;  // 0xf0
};

struct UnkMlogMessage_lb45e5c {
    u32 kind;
    CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    UnkMlogMessage_lb45e5c(u32 k, const CStrId& t) : kind(k), text(t), args(0xc) {}
};

void fn_7100b45cf8(UnkMapPage_lb45e5c* self, f32 a, f32 b, f32 c);

static inline UnkMapData_lb45e5c* MapData() {
    return reinterpret_cast<UnkMapData_lb45e5c*>(fn_7100e8d8b0());
}

// Refreshes the menu with `id` (as fn_7100b3d16c does).
static inline void Refresh(UnkMapMenu_lb45e5c* menu, const CStrId* id) {
    fn_7100b43b30_flag(reinterpret_cast<UnkMapMenuController*>(menu), id, 0);
    menu->field_0x4b4 = true;
    if (id->data == &g_unk_7101d47108->empty)
        unk_gui::fn_7100eb7358(menu->field_0x618, 0, 1);
}

void fn_7100b45e5c(UnkMapPage_lb45e5c* self) {
    self->page = static_cast<UnkWorld_lb45e5c*>(g_unk_7101d47310->field_0x2228)->page;
    if (self->closePending) {
        fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_7172763d98);
        UnkMapMenu_lb45e5c* menu = MapData()->view.entry->menu;
        menu->field_0x234 = 2;
        UnkMapMenuPending_lb45e5c* pending = menu->pending;
        fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(menu), pending ? pending->entry : 0x7fffffff, 1);
    }
    self->field_0x8 = false;
    UnkMapData_lb45e5c* map = MapData();
    UnkMapView_lb45e5c* view = &map->view;
    {
        const UnkVec2_lb45e5c offset = {0.0f, 0.0f};
        UnkVec2_lb45e5c center = (view->min + view->max) * 0.5f + offset;
        fn_7100e910fc(reinterpret_cast<UnkMmOwner_b0199*>(map), &center.x);
    }
    UnkMapMenu_lb45e5c* menu = view->entry ? view->entry->menu : nullptr;
    Refresh(menu, &g_unk_7101d470e0);
    map->view.entry->menu->field_0x4b5 = true;
    fn_7100b45cf8(self, 1.0f, MapData()->zoom, 1.0f);
    self->state = 1;
    UnkMlogMessage_lb45e5c message(2, CStrId(g_unk_7172763df0));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    message.args.Free();
}
