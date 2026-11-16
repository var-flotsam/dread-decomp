// sound::CMusicManager::CTunableMusicManager unit: the copy constructor of the music entries' file-path list
// (game/sound/UnkMusicPathVec.h). Names and layouts are placeholders.
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

UnkMusicPathList::UnkMusicPathList(const UnkMusicPathList& other) { CopyFrom(other); }
