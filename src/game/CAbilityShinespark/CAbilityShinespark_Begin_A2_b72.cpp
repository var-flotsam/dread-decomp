// CAbilityShinespark's unit (tools/tu_map.py): runs once (flag at +0x441): starts the game manager's fade
// (fn_7100df6c78) with the given target and duration, then adds four key/value words to the keyed list at
// +0x400 (fn_710033140c): 3 and 2 with 1, 7 and 6 with 8. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkKeyedWords_b52;
u32* fn_710033140c(UnkKeyedWords_b52* self, const u32* key, const u32* value);

struct UnkShinesparkBegin_A2_b72 {
    u8 pad_0x0[0x400];
    u8 field_0x400[0x41];
    bool field_0x441;
};

static inline __attribute__((always_inline)) void Add_A2_b72(UnkKeyedWords_b52* list, u32 k, u32 v) {
    u32 key = k;
    u32 value = v;
    fn_710033140c(list, &key, &value);
}

void fn_7100331350(UnkShinesparkBegin_A2_b72* self, f32 a, f32 b) {
    if (self->field_0x441)
        return;
    self->field_0x441 = true;
    fn_7100df6c78(g_unk_7101d47310, a, b);
    UnkKeyedWords_b52* list = reinterpret_cast<UnkKeyedWords_b52*>(self->field_0x400);
    Add_A2_b72(list, 3, 1);
    Add_A2_b72(list, 2, 1);
    Add_A2_b72(list, 7, 8);
    Add_A2_b72(list, 6, 8);
}
