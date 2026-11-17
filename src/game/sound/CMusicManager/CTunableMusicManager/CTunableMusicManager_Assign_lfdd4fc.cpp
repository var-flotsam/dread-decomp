// sound::CMusicManager::CTunableMusicManager's unit: copy-assignment of the music entries' file-path list
// (game/sound/UnkMusicPathVec.h): destroys its records, then reserves the other's capacity and appends a
// copy of each. Names and layouts are placeholders.
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

UnkPathVec_b0019* fn_7100fdd4fc(UnkPathVec_b0019* self, const UnkPathVec_b0019* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkPathRecord_b0019();
        self->count = 0;
        self->AppendAll(*other);
    }
    return self;
}
