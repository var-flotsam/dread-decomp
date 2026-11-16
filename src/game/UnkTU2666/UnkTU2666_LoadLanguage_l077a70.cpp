// Unit 2666 (Lua bindings): loads the language text file "system/localization/<name>.txt" (in lower
// case) into the language singleton, creating it on first use. Names and layouts are placeholders.
#include <string.h>
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU57.h"
#include "types.h"
#include "../../game_aca57c/UnkBufferString.h"

extern void* g_unk_7101a1b750[];

// Copies `src` into `dst` with A-Z turned into a-z.
static inline void CopyLower(char* dst, const char* src) {
    dst[0] = 0;
    u32 length = static_cast<u32>(strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'A') < 26)
                c += 0x20;
            dst[i] = c;
        }
        dst[length] = 0;
    }
}

s32 fn_7101077a70(lua_State* L) {
    const char* name;
    char lower[0x80];
    if (fn_71010c0270(L, "s", &name)) {
        UnkBufferString<0x80, g_unk_7101a1b750> path;
        fn_710007e77c(path.Str(), "system/localization/");
        fn_710007f564(path.Str(), name);
        fn_710007f564(path.Str(), ".txt");
        CopyLower(lower, path.Str()->data);
        {
            UnkBufferString<0x80, g_unk_7101a1b750> lowered(lower);
            fn_710007eaa0(path.Str(), lowered.Str());
        }
        UnkLanguage* language = g_unk_7101d47358;
        if (!language) {
            language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
            fn_710119c020(language);
            g_unk_7101d47358 = language;
        }
        fn_710119cb44(language, path.Str()->data, false);
    }
    return 0;
}