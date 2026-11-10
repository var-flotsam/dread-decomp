// Unit 2744 (gui/scripts/guimanager.lua): builds a string, looks its key up in the registry
// (g_unk_7101d4b5a0, created on first use) and, when found, hands the key to fn_71011ba714.
// Names and layouts are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern UnkRegistry_0x70* g_unk_7101d4b5a0;

struct UnkKeyTemp_l1ba660 {
    void* id;
    void* path;
    ~UnkKeyTemp_l1ba660() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};
struct UnkTextTemp_l1ba660 {
    u64 words[41];
    ~UnkTextTemp_l1ba660() {
        fn_710007ed40(reinterpret_cast<base::global::CRntString*>(this));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};
struct UnkEntry_0x30;
struct UnkStrPair_0x10;
UnkTextTemp_l1ba660 fn_71011ba4fc();
void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e);
void fn_71011ba714(void* self, UnkKeyTemp_l1ba660* key);

void fn_71011ba660(void* self) {
    UnkTextTemp_l1ba660 text = fn_71011ba4fc();
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    bool found;
    {
        UnkKeyTemp_l1ba660 key;
        fn_710008d4c0(reinterpret_cast<UnkStrPair_0x10*>(&key), reinterpret_cast<const UnkEntry_0x30*>(&text));
        found = fn_7100087850(reg, reinterpret_cast<const UnkStrPair_0x10*>(&key));
    }
    if (found) {
        UnkKeyTemp_l1ba660 key;
        fn_710008d4c0(reinterpret_cast<UnkStrPair_0x10*>(&key), reinterpret_cast<const UnkEntry_0x30*>(&text));
        fn_71011ba714(self, &key);
    }
}
