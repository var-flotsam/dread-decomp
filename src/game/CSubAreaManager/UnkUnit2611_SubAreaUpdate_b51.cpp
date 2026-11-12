// Unit 2611 (tools/tu_map.py; no reflected class): fn_7100faca00 and fn_7100faca84 hand every entry of a list (at +0xb0 of
// `self`, or at +0x78 of the second argument)
// to the sub-area manager (g_unk_7101d47360, 0xae8 bytes, created on first use with fn_7100ee1f34)
// through fn_7100ee2864, then runs fn_7100d59dd8 on the manager's object at +0x930.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkScenarioSub;
struct UnkManager;

struct UnkSubAreaManager_b51 {
    u8 unk_0x0[0x930];
    UnkScenarioSub* field_0x930;
};

struct UnkSubAreaEntry_b51;
void fn_7100ee2864(UnkSubAreaManager_b51* self, UnkSubAreaEntry_b51* entry);

struct UnkSubAreaEntry_b51 {
    void* value;
};

struct UnkSubAreaList_b51 {
    UnkSubAreaEntry_b51* entries;
    u32 count;
};

static inline void ApplySubAreaEntries(const UnkSubAreaList_b51* list) {
    void* instance = g_unk_7101d47360;
    if (!instance) {
        instance = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(instance);
        g_unk_7101d47360 = static_cast<UnkManager*>(instance);
    }
    UnkSubAreaManager_b51* manager = static_cast<UnkSubAreaManager_b51*>(instance);
    for (UnkSubAreaEntry_b51 *it = list->entries, *end = it + list->count; it != end; ++it)
        fn_7100ee2864(manager, it);
    fn_7100d59dd8(manager->field_0x930);
}

// The list at +0xb0 of `self`.
void fn_7100faca00(char* self) { ApplySubAreaEntries(reinterpret_cast<UnkSubAreaList_b51*>(self + 0xb0)); }

// The list at +0x78 of the second argument.
void fn_7100faca84(void* self, char* source) {
    ApplySubAreaEntries(reinterpret_cast<UnkSubAreaList_b51*>(source + 0x78));
}
