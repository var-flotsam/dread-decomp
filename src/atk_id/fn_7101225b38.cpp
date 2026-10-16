// nn::atk::detail::driver::StreamSoundLoader::StreamDataLoadTask vtable slot 0x10:
// forwards its stored arguments to the loader.
#include "types.h"

struct UnkStreamLoader;
struct UnkLoadArgs;

s32 fn_71012245fc(UnkStreamLoader* loader, UnkLoadArgs* args, s32 a, u64 b, u64 c, void* extra);

struct UnkStreamDataLoadTask {
    u8 unk_0x0[0x48];
    UnkLoadArgs* args_storage;
    u8 unk_0x50[0xc8 - 0x50];
    s32 field_0xc8;
    u8 unk_0xcc[0x4];
    u64 field_0xd0;
    u64 field_0xd8;
    UnkStreamLoader* loader;
};

s32 fn_7101225b38(UnkStreamDataLoadTask* self, void* extra) {
    return fn_71012245fc(self->loader, reinterpret_cast<UnkLoadArgs*>(&self->args_storage),
                         self->field_0xc8, self->field_0xd0, self->field_0xd8, extra);
}
