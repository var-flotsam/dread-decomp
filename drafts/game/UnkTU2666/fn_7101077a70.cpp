#include "fn_7101077a70_shared.h"
#include <string.h>
#include "base/global/CRntString.h"
#include "../../../src/game_aca57c/UnkBufferString.h"

using base::global::CRntString;

extern void* g_unk_7101a1b750[];
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
void fn_710119cb44(UnkLanguage* language, const char* path, bool flag);
void fn_710007f564(void* self, const char* str);  // appends a string
void* fn_71000839a0(u64 size, u32 flags);

s32 fn_7101077a70(lua_State* L) {
    const char* name;
    char lower[0x80];
    if (fn_71010c0270(L, "s", &name)) {
        UnkBufferString<0x80, g_unk_7101a1b750> path;
        fn_710007e77c(path.Str(), "system/localization/");
        fn_710007f564(path.Str(), name);
        fn_710007f564(path.Str(), ".txt");
        const char* text = path.Str()->data;
        lower[0] = 0;
        u32 n = strlen(text);
        u64 length = n;
        if (length) {
            for (u64 i = 0; i < length; ++i) {
                char c = text[i];
                lower[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
            }
            lower[length] = 0;
        }
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
