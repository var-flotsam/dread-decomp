// From unit 277 of tools/tu_map.py (0x71001f903c-0x71001faa40, engine::scene::sceneItems::
// CSceneGroup's unit; its initialiser is src/game_s1/fn_71001fa720.cpp): an item of a scene group
// passes a flag on to what it refers to, either directly (kind 1) or by its id through the scene's
// item map. Names and layouts are placeholders.
// Holds fn_71001f9e34.
#include "types.h"

// The scene (only its functions are used here).
struct UnkScene;

// Kind 1: the word is the object itself; otherwise it is the id the scene finds it by.
struct UnkSceneGroupItem {
    u64 field_0x0;
    u8 pad_0x8[0x8];
    u32 field_0x10;
};

void fn_71001eeff4(UnkScene* scene, u64 object, bool flag);
bool fn_71001efb44(UnkScene* scene, bool flag, const u64* id);

void fn_71001f9e34(const UnkSceneGroupItem* self, UnkScene* scene, bool flag) {
    if (self->field_0x10 == 1) {
        fn_71001eeff4(scene, self->field_0x0, flag);
        return;
    }
    u64 id = self->field_0x0;
    fn_71001efb44(scene, flag, &id);
}
