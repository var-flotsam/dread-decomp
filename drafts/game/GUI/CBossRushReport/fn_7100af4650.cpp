// GUI::CBossRushReport: the text id of a boss's name in the boss rush report, by its 1-based
// index, from the practice list or the main one.
// Names and layouts are placeholders.

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

const CStrId& fn_7100af4650(u32 boss, bool practice) {
    static const CStrId s_names[] = {
        CStrId("#GUI_BOSSRUSH_BOSS_CORPIUS", true),
        CStrId("#GUI_BOSSRUSH_BOSS_KRAID", true),
        CStrId("#GUI_BOSSRUSH_BOSS_CU_ARTARIA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_DROGYGA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_RCS", true),
        CStrId("#GUI_BOSSRUSH_BOSS_ESCUE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_EXPERIMENT_NO_Z57", true),
        CStrId("#GUI_BOSSRUSH_BOSS_TWIN_RCS", true),
        CStrId("#GUI_BOSSRUSH_BOSS_GOLZUNA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_ELITE_CS", true),
        CStrId("#GUI_BOSSRUSH_BOSS_CU_FERENIA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_RAVEN_BEAK", true),
    };
    static const CStrId s_practiceNames[] = {
        CStrId("#GUI_BOSSRUSH_BOSS_CORPIUS", true),
        CStrId("#GUI_BOSSRUSH_BOSS_KRAID", true),
        CStrId("#GUI_BOSSRUSH_BOSS_CU_ARTARIA_PRACTICE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_DROGYGA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_RCS_PRACTICE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_ESCUE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_EXPERIMENT_NO_Z57", true),
        CStrId("#GUI_BOSSRUSH_BOSS_TWIN_RCS_PRACTICE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_GOLZUNA", true),
        CStrId("#GUI_BOSSRUSH_BOSS_ELITE_CS_PRACTICE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_CU_FERENIA_PRACTICE", true),
        CStrId("#GUI_BOSSRUSH_BOSS_RAVEN_BEAK", true),
    };
    s32 index = boss - 1;
    return practice ? s_practiceNames[index] : s_names[index];
}
