// UnkTU2772 (nn::atk): fills a wave-info record (format, loop, rate, frames, per-channel data and ADPCM
// parameters) from a wave file's info block, or from the other reader kind. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkTable;

char* fn_7101213684(char* p0, u32 p1);
char* fn_71012136ac(char* p0, char* p1);
char* fn_71012136b8(char* p0);
void* fn_7101213640(UnkAtkTable* self);
u32 fn_7101213724(char* p0, char* p1);

struct UnkAtkAdpcmParam_2133d8 {
    u16 coef[16];  // 0x0
    u16 predScale;  // 0x20
    u16 yn1;  // 0x22
    u16 yn2;  // 0x24
};
struct UnkAtkAdpcmLoopParam_2133d8 {
    u16 loopPredScale;  // 0x0
    u16 loopYn1;  // 0x2
    u16 loopYn2;  // 0x4
};
struct UnkAtkAdpcmInfo_2133d8 {
    UnkAtkAdpcmParam_2133d8 param;  // 0x0
    UnkAtkAdpcmLoopParam_2133d8 loopParam;  // 0x26
};
struct UnkAtkChannelInfo_2133d8 {
    u32 field_0x0;
    s32 dataOffset;  // 0x4
    u32 field_0x8;
    s32 adpcmInfoOffset;  // 0xc
};
struct UnkAtkInfoBlock_2133d8 {
    u8 encoding;  // 0x0
    u8 isLoop;  // 0x1
    u8 pad_0x2[2];
    u32 sampleRate;  // 0x4
    u32 loopStart;  // 0x8
    u32 loopEnd;  // 0xc
    u32 originalLoopStart;  // 0x10
    s32 channelCount;  // 0x14
};
struct UnkAtkFileHeader_2133d8 {
    u8 pad_0x0[8];
    u32 version;  // 0x8
    u32 fileSize;  // 0xc
    u8 pad_0x10[0x10];
};
struct UnkAtkBlock_2133d8 {
    u32 kind;  // 0x0
    u32 size;  // 0x4
};
struct UnkAtkChannelParam_2133d8 {
    void* dataAddress;  // 0x0
    u32 dataSize;  // 0x8
    UnkAtkAdpcmParam_2133d8 adpcmParam;  // 0xc
    UnkAtkAdpcmLoopParam_2133d8 adpcmLoopParam;  // 0x32
};
struct UnkAtkWaveInfo_2133d8 {
    u32 sampleFormat;  // 0x0
    bool isLoop;  // 0x4
    s32 channelCount;  // 0x8
    u32 sampleRate;  // 0xc
    u64 loopStartFrame;  // 0x10
    u64 loopEndFrame;  // 0x18
    u64 originalLoopStartFrame;  // 0x20
    u64 dataSize;  // 0x28
    UnkAtkChannelParam_2133d8 channelParam[2];  // 0x30
};
static_assert(__builtin_offsetof(UnkAtkWaveInfo_2133d8, channelParam[1].adpcmLoopParam) == 0x9a, "");

struct UnkAtkWaveReader_2133d8 {
    UnkAtkFileHeader_2133d8* header;  // 0x0
    UnkAtkInfoBlock_2133d8* info;  // 0x8
    char* dataBody;  // 0x10
    char other[8];  // 0x18
    s8 kind;  // 0x20
};

// Encodings past the last known one read as the last format.
static inline u32 ToSampleFormat(u8 encoding) {
    return encoding < 2 ? encoding : 2;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_71012133d8(UnkAtkWaveReader_2133d8* self, UnkAtkWaveInfo_2133d8* out) {
    switch (self->kind) {
    case 0: {
        s32 channelCount = self->info->channelCount;
        out->sampleFormat = ToSampleFormat(self->info->encoding);
        out->channelCount = channelCount;
        out->sampleRate = self->info->sampleRate;
        out->isLoop = self->info->isLoop == 1;
        out->loopStartFrame = self->info->loopStart;
        out->loopEndFrame = self->info->loopEnd;
        out->dataSize = self->header->fileSize - sizeof(UnkAtkFileHeader_2133d8);
        out->originalLoopStartFrame =
            self->header->version >= 0x00010200 ? self->info->originalLoopStart : self->info->loopStart;

        for (s32 i = 0; i < channelCount; i++) {
            if (i >= 2)
                continue;
            UnkAtkChannelInfo_2133d8* channel = reinterpret_cast<UnkAtkChannelInfo_2133d8*>(
                fn_7101213684(reinterpret_cast<char*>(self->info), i));
            UnkAtkChannelParam_2133d8& param = out->channelParam[i];
            if (channel->adpcmInfoOffset != 0) {
                UnkAtkAdpcmInfo_2133d8* adpcm = reinterpret_cast<UnkAtkAdpcmInfo_2133d8*>(
                    fn_71012136b8(reinterpret_cast<char*>(channel)));
                param.adpcmParam = adpcm->param;
                param.adpcmLoopParam = adpcm->loopParam;
            }
            param.dataAddress = fn_71012136ac(reinterpret_cast<char*>(channel), self->dataBody);
            UnkAtkChannelInfo_2133d8* last = reinterpret_cast<UnkAtkChannelInfo_2133d8*>(
                fn_7101213684(reinterpret_cast<char*>(self->info), channelCount - 1));
            UnkAtkBlock_2133d8* data = static_cast<UnkAtkBlock_2133d8*>(
                fn_7101213640(reinterpret_cast<UnkAtkTable*>(self->header)));
            param.dataSize = data->size - last->dataOffset - 8;
        }
        break;
    }
    case 1:
        fn_7101213724(self->other, reinterpret_cast<char*>(out));
        break;
    }
    return true;
}
