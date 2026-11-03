// CHangableGrappleSurfaceComponent unit (tools/tu_map.py). fn_7100825538 takes a grapple message's
// point (+0x14) as the hang point (+0x88), then, when the definition (+0x98) names a grapple point
// (+0x70, not the empty id) and the message comes from it, the point fn_7100825258 works out for
// the message instead. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A point copied member by member (a user-written copy: fn_7100825258 returns it in memory).
struct UnkHangPoint_b50 {
    f32 x, y, z;

    UnkHangPoint_b50(const UnkHangPoint_b50& other) : x(other.x), y(other.y), z(other.z) {}
    UnkHangPoint_b50& operator=(const UnkHangPoint_b50& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkHangSource_b50 {
    u8 pad_0x0[0x20];
    UnkStringData* field_0x20;
};

struct UnkHangSender_b50 {
    u8 pad_0x0[0x10];
    UnkHangSource_b50* field_0x10;
};

struct UnkHangMessage_b50 {
    UnkHangSender_b50* sender;
    u8 pad_0x8[0xc];
    UnkHangPoint_b50 point;  // 0x14
};

struct UnkHangDef_b50 {
    u8 pad_0x0[0x70];
    UnkStringData* field_0x70;  // CStrId
};

struct UnkHangable_b50 {
    u8 pad_0x0[0x88];
    UnkHangPoint_b50 point;  // 0x88
    u8 pad_0x94[0x4];
    UnkHangDef_b50* def;  // 0x98
};

UnkHangPoint_b50 fn_7100825258(UnkHangable_b50* self, UnkHangMessage_b50* msg);

void fn_7100825538(UnkHangable_b50* self, UnkHangMessage_b50* msg) {
    self->point = msg->point;
    UnkStringData* id = self->def->field_0x70;
    if (id == &g_unk_7101d47108->empty || msg->sender->field_0x10->field_0x20 != id)
        return;
    self->point = fn_7100825258(self, msg);
}
