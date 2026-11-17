// Static initialiser of the music manager's reflection unit: the header ids, then the enums
// SMusicPlayFlag, SMusicStopFlag, SMusicPriority, SMusicManagerState, EMusicFadeType and
// EMusicManagerInGameState with their int conversions. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"

using namespace unk_static_init;

extern HeaderIds g_unk_71729f12d8 __attribute__((visibility("hidden")));

void fn_7100fdeaa4(void* e);
void fn_7100fdf010(void* e);
void fn_7100fdf39c(void* e);
void fn_7100fdf908(void* e);
void fn_7100fdfe14(void* e);
void fn_7100fe0140(void* e);

void fn_7100fdee90(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100fdf21c(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100fdf788(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100fdfc94(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100fdffc0(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100fe04cc(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);

extern UnkEnumType g_unk_71729f0a98;
extern char g_unk_71729f0bb0[];
extern UnkConversion g_unk_71729f0bb8;
extern UnkConversion g_unk_71729f0bd8;
extern UnkEnumType g_unk_71729f0bf8;
extern char g_unk_71729f0d10[];
extern UnkConversion g_unk_71729f0d18;
extern UnkConversion g_unk_71729f0d38;
extern UnkEnumType g_unk_71729f0d58;
extern char g_unk_71729f0e70[];
extern UnkConversion g_unk_71729f0e78;
extern UnkConversion g_unk_71729f0e98;
extern UnkEnumType g_unk_71729f0eb8;
extern char g_unk_71729f0fd0[];
extern UnkConversion g_unk_71729f0fd8;
extern UnkConversion g_unk_71729f0ff8;
extern UnkEnumType g_unk_71729f1018;
extern char g_unk_71729f1130[];
extern UnkConversion g_unk_71729f1138;
extern UnkConversion g_unk_71729f1158;
extern UnkEnumType g_unk_71729f1178;
extern char g_unk_71729f1290[];
extern UnkConversion g_unk_71729f1298;
extern UnkConversion g_unk_71729f12b8;

void fn_7100fe064c() __attribute__((visibility("hidden")));
void fn_7100fe0658() __attribute__((visibility("hidden")));
void fn_7100fe0664() __attribute__((visibility("hidden")));
void fn_7100fe0670() __attribute__((visibility("hidden")));
void fn_7100fe067c() __attribute__((visibility("hidden")));
void fn_7100fe0688() __attribute__((visibility("hidden")));
void fn_7100fe0694() __attribute__((visibility("hidden")));
void fn_7100fe06a0() __attribute__((visibility("hidden")));
void fn_7100fe06ac() __attribute__((visibility("hidden")));
void fn_7100fe06b8() __attribute__((visibility("hidden")));
void fn_7100fe06c4() __attribute__((visibility("hidden")));
void fn_7100fe06d0() __attribute__((visibility("hidden")));

void fn_7100fe09a0() {
    InitHeaderIds(&g_unk_71729f12d8);
    InitEnum(&g_unk_71729f0a98, "SMusicPlayFlag", fn_7100fdee90, fn_7100fdeaa4, g_unk_71729f0bb0,
             &g_unk_71729f0bb8, fn_7100fe064c, &g_unk_71729f0bd8, fn_7100fe0658);
    InitEnum(&g_unk_71729f0bf8, "SMusicStopFlag", fn_7100fdf21c, fn_7100fdf010, g_unk_71729f0d10,
             &g_unk_71729f0d18, fn_7100fe0664, &g_unk_71729f0d38, fn_7100fe0670);
    InitEnum(&g_unk_71729f0d58, "SMusicPriority", fn_7100fdf788, fn_7100fdf39c, g_unk_71729f0e70,
             &g_unk_71729f0e78, fn_7100fe067c, &g_unk_71729f0e98, fn_7100fe0688);
    InitEnum(&g_unk_71729f0eb8, "SMusicManagerState", fn_7100fdfc94, fn_7100fdf908, g_unk_71729f0fd0,
             &g_unk_71729f0fd8, fn_7100fe0694, &g_unk_71729f0ff8, fn_7100fe06a0);
    InitEnum(&g_unk_71729f1018, "EMusicFadeType", fn_7100fdffc0, fn_7100fdfe14, g_unk_71729f1130,
             &g_unk_71729f1138, fn_7100fe06ac, &g_unk_71729f1158, fn_7100fe06b8);
    InitEnum(&g_unk_71729f1178, "EMusicManagerInGameState", fn_7100fe04cc, fn_7100fe0140, g_unk_71729f1290,
             &g_unk_71729f1298, fn_7100fe06c4, &g_unk_71729f12b8, fn_7100fe06d0);
}
