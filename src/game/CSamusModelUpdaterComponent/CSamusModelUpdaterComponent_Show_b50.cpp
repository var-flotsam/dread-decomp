// CSamusModelUpdaterComponent unit (tools/tu_map.py, the name from reflection): resolves a part name
// for the entry (fn_710093a130, into a string with 0x40 bytes of inline storage) and, when the model
// updater has its component at +0x50, passes the name's hash to fn_7100199420 on the component's owner,
// with the flag clear (fn_710093a6d4) or set (fn_710093a780). Names are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a1b678[];

void fn_710093a130(char* self, char* entry, CRntString* out);
u64* fn_7100066630(u64* out, const CRntString* str);

static inline __attribute__((always_inline)) void ShowPart(char* self, char* entry, bool value) {
    UnkBufferString<0x40, g_unk_7101a1b678> name;
    fn_710093a130(self, entry + 0x10, name.Str());
    if (char* c = *reinterpret_cast<char**>(self + 0x50)) {
        u64 hash;
        CRntString copy(*name.Str());
        fn_7100066630(&hash, &copy);
        fn_7100199420(*reinterpret_cast<void**>(*reinterpret_cast<char**>(c + 0x138) + 0x78), &hash, value);
    }
}

void fn_710093a6d4(char* self, char* entry) {
    ShowPart(self, entry, false);
}

void fn_710093a780(char* self, char* entry) {
    ShowPart(self, entry, true);
}
