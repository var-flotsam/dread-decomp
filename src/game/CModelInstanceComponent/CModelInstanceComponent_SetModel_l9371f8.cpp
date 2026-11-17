// CModelInstanceComponent's unit (tools/tu_map.py): sets the component's model path (+0x48). A new path
// releases the current instance (+0x50) from the render manager; a path the game manager cannot load
// (fn_7100e0484c) falls back to a default one, and the instance is then created again when the
// component allows it (fn_710024fbc8, fn_7100936da0). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CActorComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkModelInstanceComponent_mic;

void fn_710021949c(UnkRenderManager* self, u64 object, u32 arg2, u32 arg3);
bool fn_7100e0484c(UnkGameManager* manager, const CFilePathStrId* path);
void fn_7100936da0(UnkModelInstanceComponent_mic* self);

// The model used when the path asked for cannot be loaded.
extern CFilePathStrId g_unk_7172738f90 __attribute__((visibility("hidden")));

struct UnkModelInstanceComponent_l9371f8 {
    u8 pad_0x0[0x48];
    CFilePathStrId model;  // 0x48
    u64 instance;          // 0x50, 0xffffffff when none
};

void fn_71009371f8(UnkModelInstanceComponent_l9371f8* self, const CFilePathStrId* path) {
    if (self->model == *path)
        return;
    if (self->instance != 0xffffffffu) {
        fn_710021949c(g_unk_7101d47300, self->instance, 0, 0);
        fn_7100219e10(g_unk_7101d47300, self->instance);
        self->instance = 0xffffffffu;
    }
    self->model = *path;
    if (self->model.data == &g_unk_7101d47200->empty)
        return;
    if (!fn_7100e0484c(g_unk_7101d47310, &self->model)) {
        self->model = g_unk_7172738f90;
        fn_7100e0484c(g_unk_7101d47310, &self->model);
    }
    if (fn_710024fbc8(self))
        fn_7100936da0(reinterpret_cast<UnkModelInstanceComponent_mic*>(self));
}
