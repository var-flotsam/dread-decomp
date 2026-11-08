#pragma once

// A minimal WebM (Matroska) demuxer for the movie runtime (movie.cpp): the segment's timecode scale
// and duration, its tracks (number, type, codec, frame size, default frame duration) and every frame
// of every cluster (SimpleBlock and BlockGroup/Block, without lacing, which VP9 video never uses).
// The whole file is in memory; frames point into it. Element ids are Matroska's.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dread::webm {

struct Track {
    std::uint64_t number = 0;
    std::uint32_t type = 0;  // 1 video, 2 audio
    std::string codec;       // "V_VP9", ...
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint64_t default_duration_ns = 0;  // per frame; 0 when the file does not say
};

struct Frame {
    std::size_t offset = 0;  // into the file's bytes
    std::size_t size = 0;
    std::int64_t time_us = 0;
    bool key = false;
};

struct File {
    std::vector<std::uint8_t> bytes;
    std::uint64_t timecode_scale_ns = 1000000;
    double duration_ticks = 0;  // in timecode_scale_ns units
    std::vector<Track> tracks;
    std::vector<std::vector<Frame>> frames;  // per entry of `tracks`

    std::int64_t DurationUs() const {
        return static_cast<std::int64_t>(duration_ticks * static_cast<double>(timecode_scale_ns) / 1000.0);
    }
};

// Parses `bytes` (moved in). False with a reason in `error` when it is not a WebM file this reads.
bool Parse(std::vector<std::uint8_t> bytes, File* out, std::string* error);

}  // namespace dread::webm
