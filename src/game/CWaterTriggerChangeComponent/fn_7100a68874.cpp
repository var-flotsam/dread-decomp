// CWaterTriggerChangeComponent unit (unit 1579 of tools/tu_map.py): the water level at the current
// time of a change, eased (fn_7100076850) between the start and end levels. Names and layouts are
// placeholders.
#include "game/UnkTU53.h"
#include "types.h"

struct UnkWaterLevelChange {
    u8 pad_0x0[0x40];
    f32 elapsed;
    u8 pad_0x44[0x124];
    f32 duration;
    u8 pad_0x16c[0x4];
    f32 endLevel;
    u8 pad_0x174[0x4];
    f32 startLevel;
};

f32 fn_7100a68874(char* object) {
    UnkWaterLevelChange* change = reinterpret_cast<UnkWaterLevelChange*>(object);
    f32 t = SmoothStepCubic(change->elapsed / change->duration);
    f32 start = change->startLevel;
    f32 end = change->endLevel;
    if (t >= 1.0f)
        return end;
    if (t < 0.0f)
        return start;
    f32 level = (1.0f - t) * start;
    level += t * end;
    return level;
}
