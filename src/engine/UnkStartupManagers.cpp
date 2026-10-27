// Unit 1643 (0x7100adfb20-0x7100ae044c, no class hint): the end of the game's start-up, which the
// game's main function (src/engine/HostArguments.cpp) calls after the render manager is set up. It
// creates the game's managers that don't exist yet (each from the default allocator, built by its
// constructor function) and runs their start-up step, sets an empty string through fn_710109cc50,
// gives the game manager a first step, and ends the console's start-up logo. Names and layouts are
// placeholders inferred from the code.
#include <nn/oe.h>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkRenderManager.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;

// Creates the object on first use: storage from the default allocator (with the given flags), built
// by its constructor function.
template <class T, u64 Size, u32 Flags, void (*Ctor)(T*)>
__attribute__((always_inline)) static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, Flags));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The managers, with the types their other users declare them with, their constructors and start-up
// steps (the types are defined in the files named).
struct UnkUserDataOwner;  // src/game/CGameManager.cpp
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);
void fn_7101203248(UnkUserDataOwner* self);

struct UnkDropTarget;  // src/game/CLogicDroppedItemManager.cpp
extern UnkDropTarget* g_unk_7101d47318;
void fn_7101023378(UnkDropTarget* self);
// Its start-up step is virtual (slot 0x18).
struct UnkDropTargetView {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

extern void* g_unk_71726bc560;  // the music manager (game/sound/CMusicManager.h)
void fn_7100fd4db8(void* self, bool flag);

extern void* g_unk_7172729910;
void fn_710119aee8(void* self);
void fn_710119b048(void* self);

extern void* g_unk_71726b1688;
void fn_7100359118(void* self);
void fn_7100e09794(void* self);

struct UnkGameObject_0x150;  // src/engine/HostArguments.cpp
extern UnkGameObject_0x150* g_unk_7101d47370;
void fn_7100dde0c8(UnkGameObject_0x150* self);
void fn_7100dde0e4(UnkGameObject_0x150* self);

struct UnkStruct_0x128;  // the scene state, src/game/UnkLoadingScreen.cpp
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100ef378c(UnkStruct_0x128* self);

struct UnkGameObject_0x60;  // src/engine/HostArguments.cpp
extern UnkGameObject_0x60* g_unk_7101d47378;
void fn_7100dde718(UnkGameObject_0x60* self);
void fn_7100dde79c(UnkGameObject_0x60* self);

void fn_710109cc50(u32 mode, const CRntString& str);

extern void* g_unk_7172758b18;
void fn_7100db23bc(void* self);
void fn_7100db256c(void* self);

void fn_7100de4b98(UnkGameManager* self);
void fn_7100deacd0(UnkGameManager* self);
void fn_7100e00900(UnkGameManager* self, bool a, bool b);
void fn_7100dfff10(UnkGameManager* self);

extern void* g_unk_71726af0b0;
void fn_7100ed4454(void* self);

struct UnkManager_7100863a8c;  // src/game_lo/fn_7100863a8c.cpp
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);

void fn_7100ebf558(UnkNavMeshManager* self);

extern void* g_unk_71726da048;
void fn_7100e1bca4(void* self);
void fn_7100e1bf64(void* self);

extern void* g_unk_7101d47368;  // the environment manager
void fn_7100d8ced8(void* self);
void fn_7100d8d0c4(void* self);

extern void* g_unk_7101d47360;  // the sub-area manager
void fn_7100ee1f34(void* self);
void fn_7100ee2764(void* self);

void fn_71011b1f30(UnkGuiRoot* self);
void fn_71011b2f58(UnkGuiRoot* self);

extern void* g_unk_7172758b20;
void fn_7101192628(void* self);
void fn_710119265c(void* self);

extern void* g_unk_717274ea80;
void fn_7100fb790c(void* self);
void fn_7100fb7a14(void* self);

struct UnkLoadingScreen;  // src/game/UnkLoadingScreen.cpp
extern UnkLoadingScreen* g_unk_7101d47380;
void fn_7100e0f89c(UnkLoadingScreen* self);
void fn_7100e0f984(UnkLoadingScreen* self);

void fn_7100219974(UnkRenderManager* self, u32 value);

// The two flags the main function passes (src/engine/HostArguments.cpp).
struct UnkStartupFlags {
    bool field_0x0;
    u8 field_0x1;
};

// The root path is not used.
void fn_7100adfb20(const UnkStartupFlags* flags, CFilePathStrId root) {
    fn_7101203248(Singleton<UnkUserDataOwner, 0x2b8, 0x3f, fn_710120320c>(g_unk_7101d47308));
    reinterpret_cast<UnkDropTargetView*>(Singleton<UnkDropTarget, 0x98, 0xb, fn_7101023378>(g_unk_7101d47318))
        ->vf_0x18();
    fn_7100fd4db8(Singleton<void, 0xf18, 0x3f, fn_7100fd3804>(g_unk_71726bc560), flags->field_0x0);
    fn_710119b048(Singleton<void, 0x2208, 0x3f, fn_710119aee8>(g_unk_7172729910));
    fn_7100e09794(Singleton<void, 0x198, 0x3f, fn_7100359118>(g_unk_71726b1688));
    fn_7100dde0e4(Singleton<UnkGameObject_0x150, 0x150, 0x3f, fn_7100dde0c8>(g_unk_7101d47370));
    fn_7100ef378c(Singleton<UnkStruct_0x128, 0x150, 0x3f, fn_7100ef3494>(g_unk_7101d47320));
    fn_7100dde79c(Singleton<UnkGameObject_0x60, 0x60, 0x3f, fn_7100dde718>(g_unk_7101d47378));
    fn_710109cc50(0, CRntString(g_unk_710162e2ed));
    fn_7100db256c(Singleton<void, 0x60, 0x3f, fn_7100db23bc>(g_unk_7172758b18));
    fn_7100deacd0(Singleton<UnkGameManager, 0x2b38, 0x3f, fn_7100de4b98>(g_unk_7101d47310));
    fn_7100e00900(g_unk_7101d47310, true, false);
    Singleton<void, 0x1278, 0x3f, fn_7100ed4454>(g_unk_71726af0b0);
    Singleton<UnkManager_7100863a8c, 0x6b0, 0x3f, fn_7100d46804>(g_unk_71726a4788);
    fn_7100ebf558(Singleton<UnkNavMeshManager, 0x62ec8, 0x3f, fn_7100ebf278>(g_unk_71726a4510));
    fn_7100e1bf64(Singleton<void, 0xe0, 0x3f, fn_7100e1bca4>(g_unk_71726da048));
    fn_7100d8d0c4(Singleton<void, 0x4c8, 0x3f, fn_7100d8ced8>(g_unk_7101d47368));
    fn_7100ee2764(Singleton<void, 0xae8, 0x3f, fn_7100ee1f34>(g_unk_7101d47360));
    fn_71011b2f58(Singleton<UnkGuiRoot, 0xe38, 0xc, fn_71011b1f30>(g_unk_717271e8f8));
    fn_710119265c(Singleton<void, 0x40, 0x3f, fn_7101192628>(g_unk_7172758b20));
    fn_7100fb7a14(Singleton<void, 0xa0, 0x3f, fn_7100fb790c>(g_unk_717274ea80));
    fn_7100e0f984(Singleton<UnkLoadingScreen, 0x220, 0x3f, fn_7100e0f89c>(g_unk_7101d47380));
    if (!flags->field_0x1) {
        fn_7100219974(g_unk_7101d47300, 1);
        fn_7100dfff10(g_unk_7101d47310);
        fn_7100219974(g_unk_7101d47300, 0);
    }
    nn::oe::FinishStartupLogo();
}
