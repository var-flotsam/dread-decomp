// CRailCameraCtrl unit (tools/tu_map.py, the name from reflection): after the base step (fn_7100764388),
// both points at +0x238 and +0x244 start at the pair at +0x68 (z cleared) and the word at +0x70 is copied
// to +0x258; when there is a player (fn_7100dec788 of the game manager), both take its x and y instead.
// Names and layouts are placeholders.
#include "game/CCameraBoundaryCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;
struct UnkPureFollowCameraCtrl;

struct UnkRailPoint {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkRailPlayerView {
    u8 pad_0x0[0x44];
    u32 x;
    u8 pad_0x48[0x54 - 0x48];
    u32 y;
};

struct UnkRailCameraCtrl {
    u8 pad_0x0[0x68];
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u8 pad_0x74[0x238 - 0x74];
    UnkRailPoint field_0x238;
    UnkRailPoint field_0x244;
    u8 pad_0x250[0x258 - 0x250];
    u32 field_0x258;
};

void fn_71009d65c0(UnkRailCameraCtrl* self) {
    fn_7100764388(reinterpret_cast<UnkPureFollowCameraCtrl*>(self));
    u32 x = self->field_0x68;
    u32 y = self->field_0x6c;
    self->field_0x238.x = x;
    self->field_0x238.y = y;
    self->field_0x238.z = 0;
    self->field_0x244.x = x;
    self->field_0x244.y = y;
    self->field_0x244.z = 0;
    self->field_0x258 = self->field_0x70;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        const UnkRailPlayerView* view = reinterpret_cast<const UnkRailPlayerView*>(player);
        u32 px = view->x;
        u32 py = view->y;
        self->field_0x238.x = px;
        self->field_0x238.y = py;
        self->field_0x238.z = 0;
        self->field_0x244.x = px;
        self->field_0x244.y = py;
        self->field_0x244.z = 0;
    }
}
