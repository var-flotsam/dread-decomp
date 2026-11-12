// Unit 2523 (entity script commands): reads a file-path string and a float from the argument stream and
// applies them to the entity (or the player it defers to). Names and layouts are placeholders.

#include <new>

#include "base/global/CFilePathStrId.h"
#include "game/CAudioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU56.h"
#include "types.h"

struct UnkCueEntity {
    u8 pad_0x0[0x208];
    void* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0xa0];
    void* field_0x2b8;
};

struct UnkPlayer;
struct UnkEntitySoundComponent;

bool fn_7100d7cd4c(UnkCueEntity* self, void* stream) {
    f32 value;
    char buffer[128];
    fn_710007cf30(stream, "sf", buffer, &value);
    base::global::CFilePathStrId path(buffer, true);
    f32 amount = value;
    UnkCueEntity* entity = self;
    for (;;) {
        if (entity->field_0x2b8) {
            fn_710071a524(static_cast<UnkEntitySoundComponent*>(entity->field_0x2b8), &path, amount, 0.0f);
            break;
        }
        if (!entity->field_0x208 && !entity->field_0x210)
            break;
        entity = reinterpret_cast<UnkCueEntity*>(fn_7100dec788(g_unk_7101d47310));
        if (!entity)
            break;
    }
    return true;
}
