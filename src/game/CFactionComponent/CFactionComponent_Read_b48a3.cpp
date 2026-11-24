// CFactionComponent unit (tools/tu_map.py): reads a string from the stream (fn_710007cee4) after the
// empty base step (fn_710024fc08), builds a keyed string from it (fn_710007e704), hands that to
// fn_710113906c and destroys it (fn_710007ecd4). Names and layout are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "game/CActorComponent.h"
#include "game/UnkTU56.h"
#include "types.h"

struct UnkFactionKey_b48a3 {
    alignas(8) u8 bytes[0x30];
};

struct UnkFactionComponent_b48a3;
bool CFactionComponent_TrySetFactionFromName(UnkFactionComponent_b48a3* self, UnkFactionKey_b48a3* key);

void CFactionComponent_ReadFactionName(UnkFactionComponent_b48a3* self, void* stream) {
    fn_710024fc08();
    const char* name;
    CRntFile_ReadCString(stream, &name);
    UnkFactionKey_b48a3 key;
    CRntString_ConstructFromCString(reinterpret_cast<base::UnkNamedEntry*>(&key), name);
    CFactionComponent_TrySetFactionFromName(self, &key);
    CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(&key));
}
