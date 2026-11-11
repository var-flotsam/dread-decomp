// From unit 281 of tools/tu_map.py (no reflected class): two queries forwarded to the scene at +0xa0
// when there is one: whether it holds the given key (fn_71001ef3f8), and passing a flag on to the
// item with the given id (fn_71001efb44). Names and layouts are placeholders.
#include "base/reflection.h"
#include "types.h"

// Declared as src/engine/render/anim/CSkeletalAnimTree/fn_710014bdec.cpp defines it.
struct UnkKeySetOwner108;
bool fn_71001ef3f8(UnkKeySetOwner108* self, const u64* key);
// Declared as src/game/UnkSceneGroupItem.cpp does.
struct UnkScene;

struct UnkSceneKeySetHolder {
    u8 pad_0x0[0xa0];
    void* scene;  // 0xa0
};

bool fn_7100202e18(UnkSceneKeySetHolder* self, u64 key) {
    void* scene = self->scene;
    return scene && fn_71001ef3f8(static_cast<UnkKeySetOwner108*>(scene), &key);
}

bool fn_7100202e48(UnkSceneKeySetHolder* self, u64 id, bool flag) {
    void* scene = self->scene;
    return scene && fn_71001efb44(static_cast<UnkScene*>(scene), flag, &id);
}
