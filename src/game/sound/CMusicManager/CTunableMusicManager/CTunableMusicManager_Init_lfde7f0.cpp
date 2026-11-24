// sound::CMusicManager::CTunableMusicManager's unit: its static initialiser (the unit header, the
// CMusicManager class, the tunable, and the music definition and test stream paths).
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;

// The unit's data (its statics, merged into one block).
struct UnkTunableMusicManagerTU {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    u64 field_0x18;
    CFilePathStrId music_defs;       // 0x20
    CFilePathStrId test_music[4];    // 0x28
    UnkFnRecord fns[3];              // 0x48
    UnkReflType type;                // 0xd8
};

extern UnkTunableMusicManagerTU g_unk_71729f0888 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729f0718;
extern char g_unk_71729f0850;
extern char g_unk_71729f0851;
extern char g_unk_71729f0852;
extern UnkTunable g_unk_71729f0858;
extern void* g_unk_7101cac220[];
void fn_7100fd1c7c();
void fn_7100fd2a50(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

static inline void InitFilePath(CFilePathStrId* path, const char* str) {
    new (path) CFilePathStrId(str, true);
    fn_7100000250(fn_7100000c7c, path, &g_unk_7101a19000);
}

void CTunableMusicManager_InitStatics() {
    UnkTunableMusicManagerTU& tu = g_unk_71729f0888;
    InitHeaderFns(tu);
    InitClass(&g_unk_71729f0718, "sound::CMusicManager", fn_7100fd2a50, fn_7100fd1c7c);
    fn_710009ec00(&g_unk_71729f0850, &g_unk_71729f0718);
    fn_710009ec00(&g_unk_71729f0851, const_cast<base::reflection::CType*>(fn_7100ad6644()));
    fn_7100fd318c();
    fn_710009ec00(&g_unk_71729f0852, &tu.type);
    InitTunable(&g_unk_71729f0858, fn_7100136ecc, g_unk_7101cac220, fn_7100075104);
    InitFilePath(&tu.music_defs, "system/snd/scenariomusicdefs.bmdefs");
    InitFilePath(&tu.test_music[0], "streams/music/test_music_01.wav");
    InitFilePath(&tu.test_music[1], "streams/music/test_music_02.wav");
    InitFilePath(&tu.test_music[2], "streams/music/test_music_03.wav");
    InitFilePath(&tu.test_music[3], "streams/music/test_music_04.wav");
}
