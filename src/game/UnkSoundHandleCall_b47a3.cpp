// Unit of 0x7100fb94d4 (tools/tu_map.py, no name hint): hands the sound manager (g_unk_7101d4d820,
// fn_7100108acc) the member at +0x90 of the object the weak handle at +8 still refers to (or 0x90 when it
// is gone, as the original computes it). Names and layouts are placeholders.
#include "game/UnkTU23.h"
#include "types.h"

struct UnkSoundMgr;
char* fn_7100108acc(char* p0, char* p1);

struct UnkSoundRef_b47a3 {
    char* object;
    s64 serial;
};

struct UnkSoundHandleHolder_b47a3 {
    u8 unk_0x0[8];
    UnkSoundRef_b47a3* ref;
    s64 serial;

    char* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->object;
        return nullptr;
    }
};

void fn_7100fb94d4(UnkSoundHandleHolder_b47a3* self) {
    fn_7100108acc(reinterpret_cast<char*>(g_unk_7101d4d820), self->Get() + 0x90);
}
