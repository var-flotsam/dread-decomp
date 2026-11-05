// base::snd::CSoundSystem's unit (tools/tu_map.py): sets the sound system up. The mutex at
// +0x400920 is initialised (fn_71000668f8), the ATK system at +8 is set up (fn_71000f7800), then
// the system package "packs/system/system.pkg" is loaded into it (fn_7100100804). Names and
// layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::global::CFilePathStrId;
using base::global::UnkStrIdInline;
using base::snd::UnkSoundSystemATK;

bool fn_71000f7800(UnkSoundSystemATK* self);
void fn_7100100804(UnkSoundSystemATK* self, const CFilePathStrId& package);

void fn_7100107a04(u8* self) {
    fn_71000668f8(reinterpret_cast<nn::os::MutexType*>(self + 0x400920));
    UnkSoundSystemATK* atk = reinterpret_cast<UnkSoundSystemATK*>(self + 8);
    fn_71000f7800(atk);
    CFilePathStrId package(UnkStrIdInline(), "packs/system/system.pkg", true);
    fn_7100100804(atk, package);
}
