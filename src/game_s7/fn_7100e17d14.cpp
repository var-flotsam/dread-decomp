// Registers the loading-screen tips: an id, a category (general, EMMI zone, EMMI phase 2, boss)
// and two lists of values (enum-like) per tip, each passed as a pointer and a count.

#include <initializer_list>

#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

struct UnkTipList;

void fn_7100e1b2e0(UnkTipList* self, const CStrId& id, u32 category, std::initializer_list<u32> a,
                   std::initializer_list<u32> b, f32 unk = 7.0f);

void fn_7100e17d14(UnkTipList* self) {
    fn_7100e1b2e0(self, CStrId("#TIP_000_GENERAL_PARKOUR_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_001_GENERAL_WALL_JUMP_000", true), 0, {}, {15});
    fn_7100e1b2e0(self, CStrId("#TIP_002_GENERAL_POWER_GRIP_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_003_GENERAL_MELEE_COUNTER_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_004_GENERAL_DASH_MELEE_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_005_GENERAL_FREE_AIM_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_006_GENERAL_RUNNING_FREE_AIM_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_007_GENERAL_HIDDEN_BLOCKS_000", true), 0, {}, {0});
    fn_7100e1b2e0(self, CStrId("#TIP_008_GENERAL_HIDDEN_BLOCKS_001", true), 0, {0}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_009_GENERAL_MISSILE_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_010_GENERAL_CHARGE_BEAM_000", true), 0, {2}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_011_GENERAL_CHARGE_BEAM_001", true), 0, {2}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_014_GENERAL_SPIDER_MAGNET_000", true), 0, {11}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_017_GENERAL_PHANTOM_CLOAK_000", true), 0, {14}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_018_GENERAL_MORPH_BALL_000", true), 0, {13}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_020_GENERAL_SPEED_BOOSTER_000", true), 0, {16}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_021_GENERAL_SPEED_BOOSTER_001", true), 0, {16}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_022_GENERAL_SHINE_SPARK_000", true), 0, {16}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_023_GENERAL_BALL_SPARK_000", true), 0, {16}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_024_GENERAL_ICE_BREAKER_000", true), 0, {9}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_025_GENERAL_CROSS_BOMB_000", true), 0, {10}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_026_GENERAL_ICON_HIGHLIGHT_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_027_GENERAL_MAP_MARKERS_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_028_GENERAL_HIDDEN_ITEM_HINTS_000", true), 0, {8}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_029_GENERAL_ROOM_COLORS_000", true), 0, {12}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_030_GENERAL_MINIMAP_RANGE_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_032_GENERAL_EXPANDING_THE_MINIMAP_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_033_GENERAL_SAMUS_SCREEN_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_034_GENERAL_MISSION_LOG_SCREEN_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_035_GENERAL_OPTIONS_SCREEN_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_036_GENERAL_RECHARGE_STATIONS_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_039_GENERAL_TRANSFER_PORTALS_000", true), 0, {17}, {18});
    fn_7100e1b2e0(self, CStrId("#TIP_040_GENERAL_METROID_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_041_GENERAL_X_PARASITES_000", true), 0, {19}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_042_GENERAL_X_PARASITES_001", true), 0, {19}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_043_GENERAL_X_PARASITES_002", true), 0, {19}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_044_GENERAL_X_PARASITES_003", true), 0, {19}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_057_GENERAL_LOST_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_058_GENERAL_FLASH_SHIFT_000", true), 0, {7}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_059_GENERAL_EMMI_000", true), 0, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_015_EMMI_ZONE_PHANTOM_CLOAK_000", true), 1, {14}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_016_EMMI_ZONE_PHANTOM_CLOAK_001", true), 1, {14}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_031_EMMI_ZONE_EMMI_AND_THE_MINIMAP_000", true), 1, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_037_EMMI_ZONE_EMMI_ZONE_DOORS_000", true), 1, {6}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_038_EMMI_ZONE_EMMI_ZONE_DOORS_001", true), 1, {6}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_045_EMMI_ZONE_EMMI_CAPTURE_000", true), 1, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_046_EMMI_ZONE_EMMI_CAPTURE_001", true), 1, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_047_EMMI_ZONE_MAP_000", true), 1, {5}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_048_EMMI_ZONE_PHANTOM_CLOAK_002", true), 1, {14}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_049_EMMI_ZONE_SPIDER_MAGNET_000", true), 1, {14}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_050_EMMI_PHASE2_EMMI_CONFRONTATION_000", true), 2, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_051_EMMI_PHASE2_EMMI_CONFRONTATION_001", true), 2, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_052_EMMI_PHASE2_EMMI_CONFRONTATION_002", true), 2, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_012_BOSS_BEAM_000", true), 3, {3}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_013_BOSS_BEAM_001", true), 3, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_019_BOSS_FLASH_SHIFT_000", true), 3, {7}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_053_BOSS_BOSS_FIGHTS_000", true), 3, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_054_BOSS_GRAB_SEQUENCE_000", true), 3, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_055_BOSS_CHARGE_BEAM_000", true), 3, {}, {});
    fn_7100e1b2e0(self, CStrId("#TIP_056_BOSS_FINISH_COUNTER_000", true), 3, {3}, {});
}
