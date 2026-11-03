// CChozoCommanderXLifeComponent unit (tools/tu_map.py, the name from reflection): after the base step
// (fn_710115fc5c), clears the state at +0x269..+0x270 and builds the table at +0x278 named after a copy
// of the id at +0x30 of the object at +0x18 (refl_RegistryMapInit). Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflRegistryMap;

void fn_710115fc5c(char*);

struct UnkRefl_fn_71008bd878;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_71008bd878>(UnkReflRegistryMap*,
                                                                                      const CStrId&);

struct UnkChozoCommanderXLife {
    u8 unk_0x0[0x18];
    char* owner;  // 0x18
    u8 unk_0x20[0x249];
    bool field_0x269;
    u32 field_0x26c;
    bool field_0x270;
    u8 unk_0x271[7];
    UnkReflRegistryMap table;  // 0x278
};

void fn_71008bd7d4(UnkChozoCommanderXLife* self) {
    fn_710115fc5c(reinterpret_cast<char*>(self));
    self->field_0x269 = false;
    self->field_0x26c = 0;
    self->field_0x270 = false;
    UnkReflRegistryMap* table = &self->table;
    CStrId name = *reinterpret_cast<const CStrId*>(self->owner + 0x30);
    base::reflection::refl_RegistryMapInit<UnkRefl_fn_71008bd878>(table, name);
}
