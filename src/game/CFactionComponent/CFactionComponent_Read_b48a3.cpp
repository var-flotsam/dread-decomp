// CFactionComponent unit (tools/tu_map.py): reads a string from the stream (fn_710007cee4) after the
// empty base step (fn_710024fc08), builds a keyed string from it (fn_710007e704), hands that to
// fn_710113906c and destroys it (fn_710007ecd4). Names and layout are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "types.h"

void fn_710024fc08();
s64 fn_710007cee4(void* stream, const char** out);

struct UnkFactionKey_b48a3 {
    alignas(8) u8 bytes[0x30];
};

struct UnkFactionComponent_b48a3;
void fn_710113906c(UnkFactionComponent_b48a3* self, UnkFactionKey_b48a3* key);

void fn_7101139014(UnkFactionComponent_b48a3* self, void* stream) {
    fn_710024fc08();
    const char* name;
    fn_710007cee4(stream, &name);
    UnkFactionKey_b48a3 key;
    fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(&key), name);
    fn_710113906c(self, &key);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&key));
}
