// CBigkranXAIComponent's unit (tools/tu_map.py unit 600): sets the flag +0x55 of the part at +0x14b4c;
// turning it on from off also clears the part's first word. Names are placeholders.
#include "types.h"

struct UnkBigkranXPart600 {
    s32 field_0x0;
    u8 pad_0x4[0x51];
    bool active;  // 0x55

    void SetActive(bool value) {
        if (!active && value) {
            field_0x0 = 0;
        }
        active = value;
    }
};

struct UnkBigkranXAI600 {
    u8 pad_0x0[0x14b4c];
    UnkBigkranXPart600 part;  // 0x14b4c
};

void fn_71003f4f0c(UnkBigkranXAI600* self, bool value) {
    self->part.SetActive(value);
}
