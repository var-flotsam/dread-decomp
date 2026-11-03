// CCharClassPlayCaptionVoiceEvent (unit 1825 of tools/tu_map.py, the name from reflection): plays
// the event's caption (the string id at +0x10, a float at +0x18 and three flags at +0x1c..+0x1e)
// through the global caption player (0x40 bytes, created on first use). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7172758b20;
void fn_7101192628(void* self);

struct UnkCaptionId;
void fn_7101192838(void* player, UnkCaptionId* caption, bool a, bool b, bool c, f32 value);

struct UnkCaptionVoiceEvent {
    u8 unk_0x0[0x10];
    u8 caption[0x8];
    f32 value;
    bool a;
    bool b;
    bool c;
};

void fn_7100bc4d0c(UnkCaptionVoiceEvent* self) {
    void* player = g_unk_7172758b20;
    if (!player) {
        player = fn_71000839a0(0x40, 0x3f);
        fn_7101192628(player);
        g_unk_7172758b20 = player;
    }
    fn_7101192838(player, reinterpret_cast<UnkCaptionId*>(self->caption), self->a, self->b, self->c, self->value);
}
