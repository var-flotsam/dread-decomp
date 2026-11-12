// The GUI::CMapMenuController unit (tools/tu_map.py, unit 1689). Names are placeholders.
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0190;
extern UnkEnumType g_unk_71729f0198;

// Sends one of two state keys for `self`.
void fn_7100b3d140(void* self, bool first) {
    fn_7100fc9d78(g_unk_717275c380, first ? reinterpret_cast<UnkStateKey*>(&g_unk_71729f0190) : reinterpret_cast<UnkStateKey*>(&g_unk_71729f0198), self);
}
