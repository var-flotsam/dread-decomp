// CBatalloonAIComponent: its per-frame update, skipped while the game manager says so
// (fn_7100deabe4). The base update (fn_71005a2558), fn_71003ddeec, a countdown at +0x146dc that
// clears the flag at +0x146d9 when it runs out, then fn_71003de48c and fn_71003de5f4 with the
// frame time. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkBatalloonAIComponent {
    u8 pad_0x0[0x146d9];
    bool field_0x146d9;
    u8 pad_0x146da[0x2];
    f32 field_0x146dc;
};

void fn_71005a2558(UnkBatalloonAIComponent* self, f32 dt);
void fn_71003ddeec(UnkBatalloonAIComponent* self);
void fn_71003de48c(UnkBatalloonAIComponent* self, f32 dt);
void fn_71003de5f4(UnkBatalloonAIComponent* self, f32 dt);

void fn_71003dde34(UnkBatalloonAIComponent* self, f32 dt) {
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    fn_71005a2558(self, dt);
    fn_71003ddeec(self);
    f32 t = self->field_0x146dc;
    if (t > 0.001f) {
        t = t - dt;
        self->field_0x146dc = t;
    }
    if (t < 0.001f)
        self->field_0x146d9 = false;
    fn_71003de48c(self, dt);
    fn_71003de5f4(self, dt);
}
