// nn::atk unit: sets up a wave file reader. In mode 1 the data is kept as it is; in mode 0 the "FWAV"
// header is checked and the bodies of its INFO and DATA blocks are found. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkTable;
char* fn_710121371c(char* p0);
void* fn_7101213600(UnkAtkTable* self);
void* fn_7101213640(UnkAtkTable* self);

struct UnkAtkFileHeader_l2132e8 {
    u32 signature;  // "FWAV"
    s16 byteOrder;  // 0xfeff
    u16 headerSize;
    u32 version;
};

struct UnkAtkBlock_l2132e8 {
    u32 signature;
    u32 size;
    u8 body[1];
};

struct UnkAtkWaveSource_l2132e8 {
    const void* data;
};

struct UnkAtkWaveReader_l2132e8 {
    const UnkAtkFileHeader_l2132e8* header;
    const void* infoBody;
    const void* dataBody;
    UnkAtkWaveSource_l2132e8 source;
    u8 mode;
};

static const u32 kWaveSignature = 0x56415746;  // "FWAV"
static const u32 kInfoSignature = 0x4f464e49;  // "INFO"
static const u32 kDataSignature = 0x41544144;  // "DATA"

static inline bool IsValidWaveFile(const UnkAtkFileHeader_l2132e8* header) {
    if (header->signature != kWaveSignature)
        return false;
    if (header->byteOrder != static_cast<s16>(0xfeff))
        return false;
    if (header->version < 0x10000 || header->version > 0x10200)
        return false;
    return true;
}

void fn_71012132e8(UnkAtkWaveReader_l2132e8* self, const UnkAtkFileHeader_l2132e8* file, s8 mode) {
    self->header = nullptr;
    self->infoBody = nullptr;
    self->dataBody = nullptr;
    fn_710121371c(reinterpret_cast<char*>(&self->source));
    self->mode = mode;
    switch (mode) {
    case 0:
        if (!IsValidWaveFile(file))
            return;
        break;
    case 1:
        self->source.data = file;
        return;
    default:
        return;
    }
    self->header = file;
    UnkAtkBlock_l2132e8* info = static_cast<UnkAtkBlock_l2132e8*>(
        fn_7101213600(reinterpret_cast<UnkAtkTable*>(const_cast<UnkAtkFileHeader_l2132e8*>(file))));
    UnkAtkBlock_l2132e8* data = static_cast<UnkAtkBlock_l2132e8*>(
        fn_7101213640(reinterpret_cast<UnkAtkTable*>(const_cast<UnkAtkFileHeader_l2132e8*>(self->header))));
    if (info == nullptr || data == nullptr)
        return;
    if (info->signature != kInfoSignature || data->signature != kDataSignature)
        return;
    self->infoBody = info->body;
    self->dataBody = data->body;
}
