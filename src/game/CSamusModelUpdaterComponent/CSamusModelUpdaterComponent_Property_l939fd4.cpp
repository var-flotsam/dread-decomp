// CSamusModelUpdaterComponent's unit (tools/tu_map.py unit 1427): the property handler. One key reads a
// model name from the property's stream, hands its id to fn_7100938ebc and clears the flag at +0x219;
// other keys go to the multi-model updater's handler (fn_710093881c). Names and layouts are
// placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "game/UnkTU56.h"
#include "types.h"

using namespace unk_prop;

struct UnkMultiModelUpdater_l93881c;
u32 CMultiModelUpdaterComponent_ReadProperty(UnkMultiModelUpdater_l93881c* self, UnkProp* p);

struct UnkSamusModelUpdater_l939fd4 {
    u8 pad_0x0[0x219];
    bool field_0x219;
};

u32 CSamusModelUpdaterComponent_ReadProperty(UnkSamusModelUpdater_l939fd4* self, UnkProp* p) {
    if (!Is(p, Key(0x1ff8)))
        return CMultiModelUpdaterComponent_ReadProperty(reinterpret_cast<UnkMultiModelUpdater_l93881c*>(self), p);
    {
        const char* name;
        CRntFile_ReadCString(p->stream, &name);
        CStrId id(name, true);
        CMultiModelUpdaterComponent_SelectModelByName(self, reinterpret_cast<const u64*>(&id));
    }
    self->field_0x219 = false;
    return true;
}
