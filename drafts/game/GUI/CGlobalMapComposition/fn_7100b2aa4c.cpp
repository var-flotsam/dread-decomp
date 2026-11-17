// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): sets the transport's area and target
// scenario, takes the target's "#GUI_TRANSPORT_*" label (none for an unknown scenario) and moves to state 2.
// Names and layouts are placeholders.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
#include "types.h"

using namespace unk_static_init;

// The fade times: built by fn_71001d81bc, then given the unit's values.
struct UnkMapFadeTimes {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[4];
    f32 field_0x18;
};

struct __attribute__((packed, aligned(4))) UnkStatics_7100b2bc80 {
    HeaderIds4 ids;
    UnkStrIdBase field_0x14;
    UnkStrIdBase field_0x1c;
    UnkStrIdBase field_0x24;
    UnkStrIdBase field_0x2c;
    UnkStrIdBase field_0x34;
    UnkStrIdBase field_0x3c;
    UnkStrIdBase field_0x44;
    UnkStrIdBase field_0x4c;
    UnkStrIdBase field_0x54;
    u8 pad_0x5c[0xf0];
    f32 field_0x14c;
    f32 field_0x150;
    bool field_0x154;
    u8 pad_0x155[0x3];
    UnkMapFadeTimes field_0x158;
    u8 field_0x174[8];
};
static_assert(__builtin_offsetof(UnkStatics_7100b2bc80, field_0x14c) == 0x14c, "layout");
static_assert(__builtin_offsetof(UnkStatics_7100b2bc80, field_0x174) == 0x174, "layout");
extern UnkStatics_7100b2bc80 g_unk_7172762d04 __attribute__((visibility("hidden")));

struct UnkGlobalMapTransport_lb2aa4c {
    u8 pad_0x0[0x588];
    s32 state;  // 0x588
    u8 pad_0x58c[0x6f8 - 0x58c];
    CStrId area;    // 0x6f8
    CStrId target;  // 0x700
    CStrId label;   // 0x708
};
static_assert(__builtin_offsetof(UnkGlobalMapTransport_lb2aa4c, label) == 0x708, "layout");

// A label of the unit's statics, as the id it is.
static inline const CStrId& Label(const UnkStrIdBase& id) { return static_cast<const CStrId&>(id); }

void fn_7100b2aa4c(UnkGlobalMapTransport_lb2aa4c* self, const CStrId& area, const CStrId& target) {
    self->area = area;
    self->target = target;
    // The target scenario's transport label; none for an unknown scenario.
    base::global::UnkStringData* scenario = self->target.data;
    if (g_unk_71727c8000.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x14);  // aqua
    else if (g_unk_71727c7fe8.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x1c);  // cave
    else if (g_unk_71727c8008.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x24);  // forest
    else if (g_unk_71727c7ff8.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x2c);  // lab
    else if (g_unk_71727c7ff0.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x34);  // magma
    else if (g_unk_71727c8010.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x3c);  // quarantine
    else if (g_unk_71727c8018.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x44);  // sanctuary
    else if (g_unk_71727c8020.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x4c);  // shipyard
    else if (g_unk_71727c8028.data == scenario)
        self->label = Label(g_unk_7172762d04.field_0x54);  // sky base
    self->state = 2;
}
