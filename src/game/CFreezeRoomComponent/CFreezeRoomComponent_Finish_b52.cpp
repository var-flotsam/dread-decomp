// CFreezeRoomComponent: once `value` drops to 0, disables the component, runs fn_7100a238f4 and
// fn_7100a232d4 (as fn_7100a241c4 in CFreezeRoomComponent_b34.cpp does when it reaches 1), then, when
// the environment manager's current id (fn_7100d8efc8) is the id at +0xc38 and the id at +0xc40 is
// set, switches the manager to that one (fn_7100d8e32c). The manager g_unk_7101d47368 is created
// lazily (0x4c8 bytes with flags 0x3f, constructed by fn_7100d8ced8). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/CActorComponent.h"
#include "game/CFreezeRoomComponent.h"
#include "types.h"

using base::global::CStrId;

// Signatures as src/game/CFreezeRoomComponent/CFreezeRoomComponent_b34.cpp and
// src/game/CSubAreaManager.cpp declare them.
struct UnkStruct_710024fa0c;
struct CFreezeRoomComponent;
struct UnkEnvironmentManager;
extern void* g_unk_7101d47368;
void fn_7100d8e32c(UnkEnvironmentManager* self, const CStrId* id, bool a, bool b);  // slot 6
const CStrId* fn_7100d8efc8(UnkEnvironmentManager* self);

struct UnkFreezeRoom_b52 {
    u8 pad_0x0[0xc38];
    CStrId id_0xc38;
    CStrId id_0xc40;
};

__attribute__((always_inline)) static inline UnkEnvironmentManager* GetEnvironmentManager_b52() {
    if (!g_unk_7101d47368) {
        void* m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return static_cast<UnkEnvironmentManager*>(g_unk_7101d47368);
}

void fn_7100a24204(UnkFreezeRoom_b52* self, f32 value) {
    if (!(value <= 0.0f))
        return;
    CFreezeRoomComponent* c = reinterpret_cast<CFreezeRoomComponent*>(self);
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
    fn_7100a238f4(c);
    fn_7100a232d4(c);
    if (self->id_0xc38.data == &g_unk_7101d47108->empty)
        return;
    if (fn_7100d8efc8(GetEnvironmentManager_b52())->data != self->id_0xc38.data)
        return;
    if (self->id_0xc40.data == &g_unk_7101d47108->empty)
        return;
    fn_7100d8e32c(GetEnvironmentManager_b52(), &self->id_0xc40, false, false);
}
