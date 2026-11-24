// msapi::res::CRenderRes's unit: the load step of a render resource. After the base load it reads the
// four-character tag of the stream's header and makes the data object of that kind (model, shader, texture,
// animation, font, particle system and the others), stores the kind and answers whether data exists.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/msapi/res/CRenderRes.h"
#include "types.h"

extern char g_unk_716e454378[] __attribute__((visibility("hidden")));

struct UnkRes_l0e8fa8;
struct UnkResLoadRequest_l0e8fa8;
bool fn_71000e8fa8(UnkRes_l0e8fa8* self, UnkResLoadRequest_l0e8fa8* request);

// The stream the resource is read from.
struct UnkRenderStream_l1b27ac {
    virtual ~UnkRenderStream_l1b27ac();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual const u32* Header();  // 0x28
    virtual void vf_0x30();
    virtual void* Data();  // 0x38
};

struct UnkRenderResState_l1b27ac {
    u8 pad_0x0[0x48];
};

struct UnkRenderRes_l1b27ac;

// The data objects, by where they keep their resource.
struct UnkResAt0_l1b27ac {
    UnkRenderRes_l1b27ac* owner;
};
struct UnkResAt8_l1b27ac {
    u8 pad_0x0[0x8];
    UnkRenderRes_l1b27ac* owner;
};
struct UnkResAt10_l1b27ac {
    u8 pad_0x0[0x10];
    UnkRenderRes_l1b27ac* owner;
    u8 pad_0x18[0x10];
    u64 begin;  // 0x28
    u64 end;    // 0x30
};
struct UnkResAt20_l1b27ac {
    u8 pad_0x0[0x20];
    UnkRenderRes_l1b27ac* owner;
};
struct UnkTexture_l1b27ac {
    u8 pad_0x0[0x8];
    UnkRenderRes_l1b27ac* owner;
    void* field_0x10;
};

// A holder that takes a texture back.
struct UnkTextureHolder_l1b27ac {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Take(void* texture);  // 0x48
};

struct UnkRenderRes_l1b27ac {
    u8 pad_0x0[0x10];
    UnkRenderResState_l1b27ac state;  // 0x10
    UnkRenderStream_l1b27ac* stream;  // 0x58
    u8 pad_0x60[0x8];
    u64 size;  // 0x68
    void* data;  // 0x70
    s32 kind;  // 0x78
    u8 pad_0x7c[0x4];
    void* texture;  // 0x80
    u64 field_0x88;
};
static_assert(__builtin_offsetof(UnkRenderRes_l1b27ac, stream) == 0x58, "layout");
static_assert(__builtin_offsetof(UnkRenderRes_l1b27ac, kind) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkRenderRes_l1b27ac, texture) == 0x80, "layout");

// The constructors and linkers of the data objects.
void fn_71001accc8(void* self, void* source);
void fn_71001acd88(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001afcc4(void* self, void* source);
void fn_71001b0050(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001b3694(void* self, void* source);
void fn_71001b387c(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001b39c4(void* self, void* a, void* b);
void fn_71001aa3c0(void* self, void* source);
void fn_71001aa410(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001aae68(void* self, void* source);
void fn_71001aaf80(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001aed18(void* self, void* source);
void fn_71001aed84(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001ab928(void* self, void* source);
void fn_71001ab9e4(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001ac1e8(void* self, void* source);
void fn_71001ac238(void* self, UnkRenderResState_l1b27ac* state);
void fn_71001b2c18(UnkRenderRes_l1b27ac* self);
UnkTextureHolder_l1b27ac* fn_7100224c54(UnkPlayer* player, u32 index);

static inline void* NewData(u64 size) { return fn_7100083bc0(size, 0x3b, 0x80, nullptr); }

bool fn_71001b27ac(UnkRenderRes_l1b27ac* self, UnkResLoadRequest_l0e8fa8* request) {
    if (!fn_71000e8fa8(reinterpret_cast<UnkRes_l0e8fa8*>(self), request))
        return false;
    const u32* header = self->stream->Header();
    if (!header) {
        self->kind = -1;
        return self->data != nullptr;
    }
    void* resource;
    switch (__builtin_bswap32(*header)) {
    case 'MUCT': {
        self->kind = 7;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt8_l1b27ac*>(NewData(0x40));
        fn_71001ab928(data, source);
        data->owner = self;
        fn_71001ab9e4(data, &self->state);
        resource = data;
        break;
    }
    case 'MFNT': {
        self->kind = 4;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt8_l1b27ac*>(NewData(0x38));
        fn_71001aae68(data, source);
        data->owner = self;
        fn_71001aaf80(data, &self->state);
        resource = data;
        break;
    }
    case 'MSHD': {
        self->kind = 1;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt0_l1b27ac*>(NewData(0x1298));
        fn_71001afcc4(data, source);
        data->owner = self;
        fn_71001b0050(data, &self->state);
        resource = data;
        break;
    }
    case 'MMDL': {
        self->kind = 0;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt10_l1b27ac*>(NewData(0x38));
        fn_71001accc8(data, source);
        data->owner = self;
        fn_71001acd88(data, &self->state);
        self->data = data;
        self->size += data->end - data->begin;
        return self->data != nullptr;
    }
    default:
        self->kind = -1;
        return self->data != nullptr;
    case 'MSUR':
        self->kind = 8;
        self->data = self->stream;
        return self->data != nullptr;
    case 'MTXT': {
        self->kind = 2;
        fn_71001b2c18(self);
        void* source = self->texture;
        if (!source)
            source = self->stream->Data();
        auto* data = static_cast<UnkTexture_l1b27ac*>(NewData(0x40));
        fn_71001b3694(data, source);
        data->owner = self;
        fn_71001b387c(data, &self->state);
        if (self->field_0x88)
            self->size = self->field_0x88;
        self->data = data;
        fn_71001b39c4(data, data->field_0x10, nullptr);
        data->field_0x10 = nullptr;
        if (self->stream) {
            delete self->stream;
            self->stream = nullptr;
        }
        if (self->texture) {
            if (self->texture != g_unk_716e454378)
                fn_7100224c54(g_unk_716e554378, 0)->Take(self->texture);
            self->texture = nullptr;
            self->field_0x88 = 0;
        }
        return self->data != nullptr;
    }
    case 'MCAN': {
        self->kind = 3;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt8_l1b27ac*>(NewData(0x18));
        fn_71001aa3c0(data, source);
        data->owner = self;
        fn_71001aa410(data, &self->state);
        resource = data;
        break;
    }
    case 'MPSY': {
        self->kind = 5;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt20_l1b27ac*>(NewData(0x28));
        fn_71001aed18(data, source);
        data->owner = self;
        fn_71001aed84(data, &self->state);
        resource = data;
        break;
    }
    case 'MLTG': {
        self->kind = 6;
        void* source = self->stream->Data();
        auto* data = static_cast<UnkResAt10_l1b27ac*>(NewData(0x18));
        fn_71001ac1e8(data, source);
        data->owner = self;
        fn_71001ac238(data, &self->state);
        resource = data;
        break;
    }
    }
    self->data = resource;
    return self->data != nullptr;
}
