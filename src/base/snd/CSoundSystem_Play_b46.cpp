// base::snd::CSoundSystem's unit (tools/tu_map.py): forwards a request on the object at +8, by id
// (fn_71000feff8, with the value) or, for id 0, by the handle alone (fn_71000f9d20); nothing for the
// invalid id 0x7fffffff. The handle is passed by value as a non-trivially copyable word. Names and
// layouts are placeholders.
#include "types.h"

struct UnkSoundHandle_b46a1 {
    u32 value;
    UnkSoundHandle_b46a1(const UnkSoundHandle_b46a1& other) : value(other.value) {}
};

void fn_71000feff8(void* player, u32 id, UnkSoundHandle_b46a1 handle, f32 value);
void fn_71000f9d20(void* player, UnkSoundHandle_b46a1 handle);

void fn_7100108b90(char* self, u32 id, const UnkSoundHandle_b46a1& handle, f32 value) {
    if (id == 0x7fffffff)
        return;
    if (id == 0)
        fn_71000f9d20(self + 8, handle);
    else
        fn_71000feff8(self + 8, id, handle, value);
}
