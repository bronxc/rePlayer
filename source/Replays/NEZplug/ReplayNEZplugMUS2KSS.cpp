#include "ReplayNEZplug.h"

#include <Core/String.h>

namespace rePlayer
{
    // MUS2KSS is based on mus_converter.js written by Juergen Wothke.
    // Ported for rePlayer (https://github.com/arnaud-neny/rePlayer).
    // The original source can be found at https://bitbucket.org/wothke/webnez

    static void ConvertProData(uint8_t* data)
    {
        uint8_t value = 0x00;
        for (size_t i = 0; i < 0x3F00; i++)
        {
            data[7 + i] ^= value;
            value = (value + 1) & 0xff;
        }
        data[0x3f93] = 2; // make it SoundTracker V2.0
    }

    static bool NoMusDrums(const uint8_t* data)
    {
        // see MUS_HEADER format in https://gitlab.com/torihino/roboplay/-/blob/master/players/src/mus.h?ref_type=heads

        // at offset 0x3f8b the MUS file names the used "drumkit" files (SM1/SM2).. (some files seem to contain
        // total garbage in this area - inconsistent with the above struct def, e.g. including trailing "SM1" extension
        // etc as well. probably the idiots just did not know how to use the file format, like putting "drumkit" comments
        // into the "title" field rather that just setting the correct info in the "drumkit" field). It is unclear if at
        // least the 'NO DRUMS' marker is used consistently - or if NULL strings might have the same meaning.

        const char str[] = "NO DRUMS";

        for (size_t i = 0; i < sizeof(str) - 1; i++)
        {
            if (data[0x3f8b + i] != str[i])
                return false;
        }
        return true;
    }

    static uint8_t* CopyData(uint8_t* data, const void* src, size_t size)
    {
        memcpy(data, src, size);
        return data + size;
    }

    NEZ_PLAY* ReplayNEZplug::LoadMUS(const uint8_t* data, uint32_t size, io::Stream* stream)
    {
        if (size < 0x4007)
            return nullptr;

        auto ext = strrchr(stream->GetName().c_str(), '.');
        if (!ext || _stricmp(ext, ".mus"))
            return nullptr;

        Array<uint8_t> kssbuf, convData;

        auto isPro = data[0x3f93] == 3;
        if (isPro)
        {
            convData.Add(data, size);
            ConvertProData(convData.Items());
            data = convData.Items();
        }

        static const uint8_t fac_player[] = {	// FAC Soundtracker 2.0 player routine
            0xc3, 0x70, 0xd0, 0xc3, 0x2f, 0xd8, 0xc3, 0xf2, 0xd0,
            0xc3, 0xdd, 0xd1, 0x00, 0xff, 0x00, 0x01, 0x21, 0x1d, 0xd0, 0x7e, 0xfe, 0x00, 0xc8, 0xcd, 0xa2,
            0x00, 0x23, 0x18, 0xf6, 0x0d, 0x46, 0x41, 0x43, 0x20, 0x53, 0x6f, 0x75, 0x6e, 0x64, 0x74, 0x72,
            0x61, 0x63, 0x6b, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x30, 0x20, 0x6d, 0x75, 0x73, 0x69, 0x63, 0x20,
            0x72, 0x6f, 0x75, 0x74, 0x69, 0x6e, 0x65, 0x20, 0x56, 0x31, 0x2e, 0x30, 0x0d, 0x0a, 0x28, 0x43,
            0x29, 0x31, 0x39, 0x39, 0x30, 0x2f, 0x31, 0x39, 0x39, 0x31, 0x20, 0x46, 0x41, 0x43, 0x20, 0x2f,
            0x20, 0x41, 0x6c, 0x6c, 0x20, 0x72, 0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 0x72, 0x65, 0x73, 0x65,
            0x72, 0x76, 0x65, 0x64, 0x0d, 0x0a, 0x00, 0xf3, 0xcd, 0x8e, 0xd0, 0xcd, 0xa1, 0xd0, 0x21, 0x00,
            0x80, 0x11, 0x00, 0x40, 0x01, 0x00, 0x40, 0xed, 0xb0, 0x3a, 0xc1, 0xfc, 0x26, 0x00, 0xcd, 0x24,
            0x00, 0xcd, 0x97, 0xd0, 0xc9, 0x3a, 0x42, 0xf3, 0x26, 0x40, 0xcd, 0x24, 0x00, 0xc9, 0x3a, 0xc0,
            0xfc, 0x26, 0x40, 0xcd, 0x24, 0x00, 0xfb, 0xc9, 0x3a, 0x8c, 0xbf, 0xfe, 0x02, 0xc8, 0x21, 0x00,
            0x80, 0x11, 0x00, 0xbf, 0x06, 0x09, 0x7e, 0xe6, 0xc0, 0xfe, 0x40, 0xcc, 0xc3, 0xd0, 0x23, 0x10,
            0xf5, 0x01, 0x03, 0x00, 0x09, 0xe7, 0x28, 0x05, 0x18, 0xea, 0x36, 0x40, 0xc9, 0x21, 0x0b, 0x80,
            0x11, 0x0c, 0x00, 0x7e, 0xe6, 0x0f, 0x28, 0x14, 0x47, 0xcb, 0x27, 0xcb, 0x27, 0xcb, 0x27, 0xcb,
            0x27, 0xb0, 0x77, 0xe5, 0x2b, 0x7e, 0xa7, 0x20, 0x02, 0x36, 0xff, 0xe1, 0x19, 0x7c, 0xfe, 0xbf,
            0x20, 0xe1, 0xc9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x8e, 0xd0, 0x3e, 0x00, 0x32, 0x0e,
            0xd0, 0xf6, 0x01, 0x32, 0x0f, 0xd0, 0x3e, 0xff, 0x32, 0x0d, 0xd0, 0x21, 0x0d, 0xd5, 0x11, 0x0e,
            0xd5, 0x01, 0x2d, 0x00, 0x36, 0x00, 0xed, 0xb0, 0x21, 0x3b, 0xd5, 0x11, 0x3c, 0xd5, 0x01, 0x08,
            0x00, 0x36, 0x40, 0xed, 0xb0, 0x21, 0x00, 0x40, 0x22, 0xec, 0xd0, 0x3a, 0xf8, 0x7f, 0x3c, 0x47,
            0x11, 0xc0, 0x00, 0x19, 0x10, 0xfd, 0x22, 0xee, 0xd0, 0x3a, 0x0c, 0xd0, 0xfe, 0x01, 0xcc, 0x40,
            0xd2, 0x3a, 0x0c, 0xd0, 0xfe, 0x00, 0xcc, 0x16, 0xd2, 0x3a, 0x82, 0x7f, 0xcd, 0x6a, 0xd1, 0x32,
            0xf0, 0xd0, 0x3d, 0x32, 0xf1, 0xd0, 0xf3, 0x21, 0x9f, 0xfd, 0x11, 0xa1, 0xd1, 0x01, 0x05, 0x00,
            0xed, 0xb0, 0x21, 0x76, 0xd1, 0x22, 0xa0, 0xfd, 0x3e, 0xc3, 0x32, 0x9f, 0xfd, 0xcd, 0x97, 0xd0,
            0xc9, 0xf5, 0x3a, 0xe8, 0xff, 0xcb, 0x3f, 0xe6, 0x01, 0x47, 0xf1, 0x90, 0xc9, 0xcd, 0x8e, 0xd0,
            0x3a, 0xf0, 0xd0, 0x47, 0x3a, 0xf1, 0xd0, 0x3c, 0x32, 0xf1, 0xd0, 0xb8, 0x28, 0x2c, 0xcd, 0x44,
            0xd5, 0x3a, 0x82, 0xd2, 0xfe, 0x01, 0x28, 0x05, 0xd6, 0x02, 0x32, 0x82, 0xd2, 0x3e, 0x20, 0x32,
            0x86, 0xd2, 0xcd, 0x5f, 0xd2, 0xcd, 0x97, 0xd0, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0x3a, 0x0e, 0xd0,
            0x3c, 0x32, 0x0e, 0xd0, 0x3e, 0x01, 0x32, 0x0f, 0xd0, 0xc9, 0x3a, 0x0f, 0xd0, 0x3c, 0x32, 0x0f,
            0xd0, 0xfe, 0x11, 0xcc, 0xa6, 0xd1, 0xaf, 0x32, 0xf1, 0xd0, 0x2a, 0xec, 0xd0, 0xed, 0x5b, 0xee,
            0xd0, 0xe7, 0xca, 0xdd, 0xd1, 0xcd, 0x93, 0xd2, 0x2a, 0xec, 0xd0, 0x11, 0x0c, 0x00, 0x19, 0x22,
            0xec, 0xd0, 0x18, 0xc1, 0xf3, 0x21, 0xa1, 0xd1, 0x11, 0x9f, 0xfd, 0x01, 0x05, 0x00, 0xed, 0xb0,
            0xaf, 0x32, 0x0d, 0xd0, 0xcd, 0xf4, 0xd1, 0xcd, 0x97, 0xd0, 0xc9, 0x3a, 0x0c, 0xd0, 0x1f, 0x38,
            0x0e, 0x21, 0x64, 0xd7, 0x06, 0x12, 0x4e, 0xaf, 0xcd, 0x80, 0xd3, 0x23, 0x10, 0xf8, 0xc9, 0x21,
            0x52, 0xd7, 0x06, 0x0c, 0x4e, 0xaf, 0xcd, 0x80, 0xd3, 0x23, 0x10, 0xf8, 0xc9, 0xf3, 0x21, 0x6f,
            0xd6, 0x11, 0x00, 0x7f, 0x06, 0x63, 0x4e, 0x1a, 0xcd, 0x80, 0xd3, 0x23, 0x13, 0x10, 0xf7, 0x3a,
            0xce, 0x7f, 0x0e, 0xbd, 0xcd, 0x80, 0xd3, 0x3e, 0x08, 0x0e, 0x18, 0xcd, 0x80, 0xd3, 0x3e, 0x08,
            0x0e, 0x19, 0xcd, 0x80, 0xd3, 0xfb, 0xc9, 0xf3, 0x01, 0x00, 0x08, 0x21, 0x6c, 0x7f, 0x7e, 0xcd,
            0x80, 0xd3, 0x23, 0x0c, 0x10, 0xf8, 0x01, 0x30, 0x06, 0x21, 0x74, 0x7f, 0x7e, 0xcd, 0x80, 0xd3,
            0x23, 0x0c, 0x10, 0xf8, 0xfb, 0xc9, 0x3a, 0x0c, 0xd0, 0x1f, 0xd0, 0x21, 0x73, 0xd2, 0x06, 0x0a,
            0x4e, 0x23, 0x7e, 0x23, 0xcd, 0x80, 0xd3, 0x10, 0xf7, 0xc9, 0x16, 0x20, 0x17, 0x57, 0x18, 0xaa,
            0x36, 0xf0, 0x37, 0x70, 0x38, 0x0f, 0x26, 0x05, 0x27, 0x01, 0x28, 0x00, 0x0e, 0x20, 0x11, 0x64,
            0xd7, 0x3a, 0x0c, 0xd0, 0x1f, 0xd0, 0x11, 0x52, 0xd7, 0xc9, 0xf3, 0x06, 0x09, 0xcd, 0x87, 0xd2,
            0xc5, 0x78, 0x32, 0x2c, 0xd8, 0x7e, 0xfe, 0x00, 0x28, 0x4d, 0xcb, 0x7f, 0xc2, 0xf3, 0xd2, 0xfe,
            0x40, 0x38, 0x1c, 0xca, 0x6d, 0xd5, 0xfe, 0x41, 0xca, 0x9b, 0xd3, 0xfe, 0x42, 0xca, 0xd1, 0xd3,
            0xfe, 0x4d, 0xda, 0xac, 0xd3, 0xfe, 0x57, 0xda, 0xc9, 0xd3, 0xfe, 0x71, 0xda, 0xfc, 0xd3, 0xcd,
            0xa0, 0xd4, 0x23, 0x13, 0xc1, 0x3a, 0x0c, 0xd0, 0x1f, 0x38, 0x0a, 0x10, 0xc3, 0xcd, 0x0b, 0xd4,
            0xcd, 0x44, 0xd5, 0xfb, 0xc9, 0x78, 0xfe, 0x04, 0x28, 0x02, 0x10, 0xb4, 0xcd, 0x31, 0xd4, 0xcd,
            0x44, 0xd5, 0xcd, 0x5f, 0xd2, 0xfb, 0xc9, 0x13, 0x18, 0xd8, 0xe5, 0xd5, 0xcb, 0x77, 0x20, 0x36,
            0xe6, 0x3f, 0xee, 0x3f, 0xf5, 0x3a, 0x2c, 0xd8, 0x47, 0x3a, 0x0c, 0xd0, 0x1f, 0x38, 0x08, 0x21,
            0x66, 0x00, 0x11, 0x0b, 0x00, 0x18, 0x34, 0x3e, 0x09, 0x90, 0x4f, 0x06, 0x00, 0x21, 0x74, 0x7f,
            0x09, 0x7e, 0xe6, 0xf0, 0x47, 0xf1, 0xcb, 0x3f, 0xcb, 0x3f, 0xb0, 0x47, 0x3e, 0x30, 0x81, 0x4f,
            0x78, 0xcd, 0x80, 0xd3, 0x18, 0x2c, 0xe6, 0x3f, 0xee, 0x3f, 0xf5, 0x3a, 0x2c, 0xd8, 0x47, 0x3a,
            0x0c, 0xd0, 0x1f, 0x38, 0x25, 0x21, 0x65, 0x00, 0x11, 0x0b, 0x00, 0xa7, 0xed, 0x52, 0x10, 0xfb,
            0xeb, 0x21, 0x00, 0x7f, 0x19, 0x7e, 0xe6, 0xc0, 0xc1, 0xb0, 0x21, 0x6f, 0xd6, 0x19, 0x4e, 0xcd,
            0x80, 0xd3, 0xd1, 0xe1, 0xc3, 0xf0, 0xd2, 0xf1, 0x18, 0xf8, 0x3e, 0x09, 0x90, 0x4f, 0x06, 0x00,
            0x21, 0x74, 0x7f, 0x09, 0x7e, 0xe6, 0xf0, 0x20, 0xee, 0x3a, 0x6e, 0x7f, 0xe6, 0xc0, 0xc1, 0xb0,
            0x0e, 0x02, 0xcd, 0x80, 0xd3, 0x18, 0xdb, 0xf5, 0x3a, 0x0c, 0xd0, 0x1f, 0x79, 0x38, 0x07, 0xd3,
            0xc0, 0xf1, 0xd3, 0xc1, 0x18, 0x09, 0xd3, 0x7c, 0xe3, 0xe3, 0xf1, 0xd3, 0x7d, 0xe3, 0xe3, 0xe3,
            0xe3, 0xc9, 0xe5, 0x21, 0x16, 0xd5, 0xcd, 0xed, 0xd3, 0x34, 0x3e, 0x01, 0xcd, 0xb5, 0xd5, 0xe1,
            0xc3, 0xf0, 0xd2, 0xd6, 0x43, 0x28, 0x30, 0x06, 0x01, 0xe5, 0xc5, 0x21, 0x0d, 0xd5, 0xcd, 0xed,
            0xd3, 0x77, 0x21, 0x31, 0xd5, 0x09, 0x7e, 0xe6, 0xfc, 0xc1, 0xb0, 0x77, 0xe1, 0xc3, 0xf0, 0xd2,
            0xd6, 0x4d, 0x28, 0x13, 0x06, 0x02, 0x18, 0xe1, 0xe5, 0x21, 0x16, 0xd5, 0xcd, 0xed, 0xd3, 0x35,
            0x3e, 0x01, 0xcd, 0xd3, 0xd5, 0x18, 0xc8, 0xe5, 0x21, 0x31, 0xd5, 0xcd, 0xed, 0xd3, 0x7e, 0xe6,
            0xfc, 0x77, 0x18, 0xd8, 0xed, 0x4b, 0x2c, 0xd8, 0x09, 0xc9, 0xe6, 0x3f, 0x3d, 0x87, 0x4f, 0x06,
            0x00, 0x09, 0xc9, 0xe5, 0x47, 0x3e, 0x72, 0x90, 0xcd, 0x6a, 0xd1, 0x32, 0xf0, 0xd0, 0xe1, 0xc3,
            0xf0, 0xd2, 0x7e, 0xfe, 0x00, 0x28, 0x1d, 0xcd, 0x8b, 0xd4, 0x23, 0x7e, 0x0e, 0x12, 0xcd, 0x80,
            0xd3, 0x3e, 0x01, 0x0e, 0x07, 0xcd, 0x80, 0xd3, 0x23, 0x7e, 0xe6, 0x0f, 0xcd, 0x37, 0xd6, 0xcd,
            0x83, 0xd4, 0x23, 0xc9, 0x23, 0x23, 0x23, 0xc9, 0x01, 0x05, 0x00, 0x09, 0xe5, 0x2b, 0x7e, 0xcb,
            0x3f, 0xcb, 0x3f, 0xcb, 0x3f, 0xcb, 0x3f, 0xf5, 0x23, 0x7e, 0xcb, 0x3f, 0xcb, 0x3f, 0xcb, 0x3f,
            0xcb, 0x3f, 0x28, 0x33, 0x3d, 0x4f, 0x21, 0x5f, 0xd6, 0x09, 0x7e, 0x32, 0x86, 0xd2, 0xcb, 0x5f,
            0x28, 0x05, 0x3e, 0x05, 0x32, 0x82, 0xd2, 0xc1, 0x3a, 0x7a, 0xd2, 0xe6, 0xf0, 0xb0, 0xee, 0x0f,
            0x32, 0x7a, 0xd2, 0x78, 0xcb, 0x3f, 0xc6, 0x07, 0x47, 0x3a, 0x7c, 0xd2, 0xe6, 0xf0, 0xb0, 0xee,
            0x0f, 0x32, 0x7c, 0xd2, 0xe1, 0x23, 0xc9, 0xf1, 0x18, 0xfa, 0x3e, 0xa0, 0x0e, 0x07, 0xcd, 0x80,
            0xd3, 0xc9, 0xe5, 0x21, 0x76, 0xd7, 0xcd, 0xf5, 0xd3, 0x7e, 0x0e, 0x11, 0xcd, 0x80, 0xd3, 0x23,
            0x7e, 0x0d, 0xcd, 0x80, 0xd3, 0xe1, 0xc9, 0xe5, 0xd5, 0xf5, 0x21, 0x3a, 0xd5, 0xcd, 0xed, 0xd3,
            0x7e, 0x32, 0x0d, 0xd5, 0xaf, 0x77, 0x21, 0x31, 0xd5, 0x09, 0x7e, 0xe6, 0xfc, 0x77, 0xf1, 0x21,
            0xd2, 0xd6, 0xcd, 0xf3, 0xd3, 0xeb, 0x21, 0x16, 0xd5, 0xcd, 0xed, 0xd3, 0x7e, 0xf5, 0x21, 0x1e,
            0xd5, 0x09, 0x09, 0x1a, 0xc1, 0x80, 0x77, 0x32, 0x2a, 0xd8, 0x13, 0x23, 0x1a, 0x77, 0x32, 0x2b,
            0xd8, 0xcd, 0xfb, 0xd4, 0xe1, 0x4e, 0x3a, 0x2a, 0xd8, 0xcd, 0x80, 0xd3, 0x23, 0x4e, 0x3a, 0x0d,
            0xd5, 0xfe, 0x40, 0x3a, 0x2b, 0xd8, 0x28, 0x03, 0xcd, 0x80, 0xd3, 0xb0, 0xcd, 0x80, 0xd3, 0xeb,
            0xe1, 0xc9, 0x3a, 0x0c, 0xd0, 0x06, 0x20, 0x1f, 0xd0, 0x21, 0x2b, 0xd8, 0xcb, 0x1e, 0x2b, 0xcb,
            0x1e, 0x06, 0x10, 0xc9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x06, 0x09, 0x21, 0x3a, 0xd5,
            0xcd, 0x87, 0xd2, 0xc5, 0xe5, 0x78, 0x32, 0x2c, 0xd8, 0xe5, 0x21, 0x0d, 0xd5, 0xcd, 0xed, 0xd3,
            0x7e, 0xe1, 0xcb, 0x46, 0xc4, 0xb5, 0xd5, 0xcb, 0x4e, 0xc4, 0xd3, 0xd5, 0xe1, 0x2b, 0x13, 0x13,
            0xc1, 0x10, 0xe0, 0xc9, 0xe5, 0x21, 0x3a, 0xd5, 0xcd, 0xed, 0xd3, 0x36, 0x40, 0x21, 0x1e, 0xd5,
            0x09, 0x09, 0x7e, 0x32, 0x2a, 0xd8, 0x23, 0x7e, 0x32, 0x2b, 0xd8, 0xcd, 0xfb, 0xd4, 0x1a, 0x4f,
            0x3a, 0x2a, 0xd8, 0xcd, 0x80, 0xd3, 0x13, 0x1a, 0x4f, 0x3a, 0x2b, 0xd8, 0xcd, 0x80, 0xd3, 0xe1,
            0xc3, 0xcb, 0xd2, 0x21, 0x1e, 0xd5, 0xcd, 0xed, 0xd3, 0x09, 0x4f, 0x7e, 0x23, 0x46, 0xf5, 0x78,
            0xa7, 0x28, 0x06, 0x79, 0xa7, 0x28, 0x02, 0xf1, 0xc9, 0xf1, 0xf1, 0xc9, 0xcd, 0x9c, 0xd5, 0xcb,
            0x40, 0x20, 0x08, 0x81, 0xfe, 0xb2, 0x30, 0x69, 0xa7, 0x18, 0x01, 0x81, 0x4f, 0x78, 0x30, 0x26,
            0xee, 0x03, 0xcb, 0x47, 0x28, 0x20, 0xc6, 0x04, 0x18, 0x1c, 0xcd, 0x9c, 0xd5, 0xcb, 0x40, 0x28,
            0x08, 0x91, 0xfe, 0x46, 0x38, 0x3e, 0xa7, 0x18, 0x01, 0x91, 0x4f, 0x78, 0x30, 0x08, 0xee, 0x03,
            0xcb, 0x47, 0x20, 0x02, 0xd6, 0x04, 0x77, 0x32, 0x2b, 0xd8, 0x2b, 0x71, 0x79, 0x32, 0x2a, 0xd8,
            0xcd, 0xfb, 0xd4, 0x1a, 0x4f, 0x3a, 0x2a, 0xd8, 0xcd, 0x80, 0xd3, 0x13, 0x1a, 0x4f, 0xc5, 0x21,
            0x3a, 0xd5, 0xcd, 0xed, 0xd3, 0xc1, 0x7e, 0xe6, 0x40, 0x3a, 0x2b, 0xd8, 0x20, 0x01, 0xb0, 0xcd,
            0x80, 0xd3, 0x1b, 0xc9, 0xd6, 0x45, 0x0e, 0x8b, 0x81, 0x4f, 0x78, 0xee, 0x03, 0xd6, 0x04, 0x18,
            0xc5, 0xd6, 0xb2, 0x0e, 0x59, 0x81, 0x4f, 0x78, 0xee, 0x03, 0xc6, 0x04, 0x18, 0xb8, 0xe5, 0x21,
            0xee, 0xd7, 0x3d, 0x87, 0x87, 0x4f, 0x06, 0x00, 0x09, 0x7e, 0x0e, 0x0a, 0xcd, 0x80, 0xd3, 0x23,
            0x7e, 0x0e, 0x09, 0xcd, 0x80, 0xd3, 0x23, 0x7e, 0x0e, 0x0c, 0xcd, 0x80, 0xd3, 0x23, 0x7e, 0x0e,
            0x0b, 0xcd, 0x80, 0xd3, 0xe1, 0xc9, 0x30, 0x2a, 0x21, 0x24, 0x22, 0x23, 0x3a, 0x28, 0x32, 0x31,
            0x2e, 0x2b, 0x29, 0x27, 0x3f, 0x00, 0x20, 0x23, 0x40, 0x43, 0x60, 0x63, 0x80, 0x83, 0xa0, 0xb0,
            0xc0, 0x21, 0x24, 0x41, 0x44, 0x61, 0x64, 0x81, 0x84, 0xa1, 0xb1, 0xc1, 0x22, 0x25, 0x42, 0x45,
            0x62, 0x65, 0x82, 0x85, 0xa2, 0xb2, 0xc2, 0x28, 0x2b, 0x48, 0x4b, 0x68, 0x6b, 0x88, 0x8b, 0xa3,
            0xb3, 0xc3, 0x29, 0x2c, 0x49, 0x4c, 0x69, 0x6c, 0x89, 0x8c, 0xa4, 0xb4, 0xc4, 0x2a, 0x2d, 0x4a,
            0x4d, 0x6a, 0x6d, 0x8a, 0x8d, 0xa5, 0xb5, 0xc5, 0x30, 0x33, 0x50, 0x53, 0x70, 0x73, 0x90, 0x93,
            0xa6, 0xb6, 0xc6, 0x31, 0x34, 0x51, 0x54, 0x71, 0x74, 0x91, 0x94, 0xa7, 0xb7, 0xc7, 0x32, 0x35,
            0x52, 0x55, 0x72, 0x75, 0x92, 0x95, 0xa8, 0xb8, 0xc8, 0x59, 0x09, 0x6d, 0x09, 0x83, 0x09, 0x9a,
            0x09, 0xb2, 0x09, 0xcc, 0x09, 0xe8, 0x09, 0x05, 0x0a, 0x23, 0x0a, 0x44, 0x0a, 0x67, 0x0a, 0x8b,
            0x0a, 0x59, 0x0d, 0x6d, 0x0d, 0x83, 0x0d, 0x9a, 0x0d, 0xb2, 0x0d, 0xcc, 0x0d, 0xe8, 0x0d, 0x05,
            0x0e, 0x23, 0x0e, 0x44, 0x0e, 0x67, 0x0e, 0x8b, 0x0e, 0x59, 0x11, 0x6d, 0x11, 0x83, 0x11, 0x9a,
            0x11, 0xb2, 0x11, 0xcc, 0x11, 0xe8, 0x11, 0x05, 0x12, 0x23, 0x12, 0x44, 0x12, 0x67, 0x12, 0x8b,
            0x12, 0x59, 0x15, 0x6d, 0x15, 0x83, 0x15, 0x9a, 0x15, 0xb2, 0x15, 0xcc, 0x15, 0xe8, 0x15, 0x05,
            0x16, 0x23, 0x16, 0x44, 0x16, 0x67, 0x16, 0x8b, 0x16, 0x59, 0x19, 0x6d, 0x19, 0x83, 0x19, 0x9a,
            0x19, 0xb2, 0x19, 0xcc, 0x19, 0xe8, 0x19, 0x05, 0x1a, 0x23, 0x1a, 0x44, 0x1a, 0x67, 0x1a, 0x8b,
            0x1a, 0x59, 0x1d, 0x6d, 0x1d, 0x83, 0x1d, 0x9a, 0x1d, 0x10, 0x20, 0x11, 0x21, 0x12, 0x22, 0x13,
            0x23, 0x14, 0x24, 0x15, 0x25, 0x16, 0x26, 0x17, 0x27, 0x18, 0x28, 0xa0, 0xb0, 0xa1, 0xb1, 0xa2,
            0xb2, 0xa3, 0xb3, 0xa4, 0xb4, 0xa5, 0xb5, 0xa6, 0xb6, 0xa7, 0xb7, 0xa8, 0xb8, 0x05, 0x22, 0x05,
            0x6a, 0x05, 0xba, 0x06, 0x12, 0x06, 0x73, 0x06, 0xd3, 0x07, 0x3b, 0x07, 0xab, 0x08, 0x1b, 0x08,
            0x9c, 0x09, 0x24, 0x09, 0xac, 0x0a, 0x3d, 0x0a, 0xd5, 0x0b, 0x7d, 0x0c, 0x2d, 0x0c, 0xe6, 0x0d,
            0xa6, 0x0e, 0x7f, 0x0f, 0x57, 0x10, 0x3f, 0x11, 0x38, 0x12, 0x40, 0x13, 0x51, 0x14, 0x7a, 0x15,
            0xaa, 0x16, 0xfb, 0x18, 0x5b, 0x19, 0xc4, 0x1b, 0x4d, 0x1c, 0xfe, 0x1e, 0xb6, 0x20, 0x77, 0x22,
            0x70, 0x24, 0x81, 0x26, 0xaa, 0x28, 0xfc, 0x2b, 0x5d, 0x2d, 0xf6, 0x30, 0xaf, 0x33, 0x89, 0x36,
            0xa2, 0x39, 0xf4, 0x3d, 0x66, 0x40, 0xf7, 0x44, 0xe1, 0x49, 0x0b, 0x4d, 0x4d, 0x51, 0xf0, 0x56,
            0xb2, 0x5b, 0xec, 0x61, 0x5f, 0x67, 0x1a, 0x6d, 0x45, 0x73, 0xe8, 0x7a, 0xcb, 0x81, 0xf0, 0x89,
            0xc4, 0x92, 0x18, 0x9a, 0xa4, 0x00, 0x00, 0x03, 0xff, 0x04, 0x00, 0x07, 0xff, 0x08, 0x00, 0x0b,
            0xff, 0x0c, 0x00, 0x0f, 0xff, 0x10, 0x00, 0x13, 0xff, 0x14, 0x00, 0x17, 0xff, 0x18, 0x00, 0x1b,
            0xff, 0x1c, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x07, 0xff, 0x08, 0x00, 0x0f, 0xff, 0x10, 0x00, 0x17,
            0xff, 0x18, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x10, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x1f,
            0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3a, 0x2e, 0xd8, 0xee, 0x01, 0x32, 0x2e, 0xd8, 0xcc, 0x5b,
            0xd8, 0x3a, 0x2e, 0xd8, 0xfe, 0x01, 0xcc, 0x6b, 0xd8, 0xf3, 0x01, 0x00, 0x40, 0x21, 0x00, 0x80,
            0x3e, 0x0f, 0xd3, 0xc0, 0x7e, 0xd3, 0xc1, 0x23, 0xe3, 0xe3, 0x0b, 0x78, 0xb1, 0xc2, 0x49, 0xd8,
            0xfb, 0xc9, 0x06, 0x09, 0x21, 0x72, 0xd8, 0x7e, 0xd3, 0xc0, 0x23, 0x7e, 0xd3, 0xc1, 0x23, 0x10,
            0xf6, 0xc9, 0x21, 0x84, 0xd8, 0x06, 0x09, 0x18, 0xee, 0x04, 0x78, 0x07, 0x01, 0x08, 0x00, 0x09,
            0x00, 0x0a, 0x00, 0x0b, 0xff, 0x0c, 0x0f, 0x10, 0xf0, 0x07, 0x60, 0x04, 0x78, 0x07, 0x01, 0x08,
            0x00, 0x09, 0x00, 0x0a, 0x10, 0x0b, 0xff, 0x0c, 0x1f, 0x10, 0xf0, 0x07, 0x60, 0xa5, 0x90, 0x91,
            0x1a, 0x0c, 0x79, 0x81, 0xa1, 0x89, 0x2d, 0x4c, 0x29, 0x18, 0x01, 0xd6, 0x99, 0x01, 0x2d, 0x10,
            0x89, 0x00, 0x80, 0xd3, 0x08, 0x90, 0x10, 0x09, 0x95, 0xa0, 0x4d, 0x4b, 0x00, 0x89, 0x03, 0xe2,
            0x09, 0xb4, 0x3f, 0x3b, 0x28, 0x81, 0x0b, 0x3b, 0x48, 0xb9, 0x20, 0x2f, 0x11, 0xd2, 0x01, 0xe2,
            0x1d, 0x4a, 0x18, 0x88, 0x02, 0x8b, 0x48, 0xb1, 0x4f, 0x28, 0xa0, 0x3a, 0xa3, 0x9c, 0x79, 0x09,
            0x59, 0xb6, 0x98, 0x81, 0x0a, 0x3c, 0x10, 0x98, 0x03, 0xf2, 0x88, 0x2a, 0x0a, 0x48, 0xc3, 0x90,
            0x82, 0xb8, 0x49, 0xa3, 0x5c, 0x83, 0x8d, 0x04, 0xb0, 0x2d, 0x30, 0xa9, 0x5c, 0x10, 0x88, 0x81
        };

        if (NoMusDrums(data))
        {
            static const uint8_t kssHead[] = {
              0x4B, 0x53, 0x53, 0x58, 0x00, 0x40, 0xFF, 0xFF, 0x10, 0x80, 0x38, 0x00, 0x00, 0x00, 0x10, 0x08,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // to force only 1 subsong
            };

            kssbuf.Resize(0x4000 + sizeof(fac_player) + sizeof(kssHead));
            auto* kssptr = CopyData(kssbuf.Items(), kssHead, sizeof(kssHead));
            kssptr = CopyData(kssptr, data + 7, 0x4000);
            CopyData(kssptr, fac_player, sizeof(fac_player));
            kssptr[0xC] = 0;
            kssptr[0xA3] = 0x7F;

            static const uint8_t noDrumPatch[] = {
              0x21, 0x00, 0x80, 0x11, 0x00, 0xD0, 0x01, 0x96, 0x08, 0xED, 0xB0, 0x21, 0x47, 0xD0, 0x11, 0x20,
              0x00, 0x01, 0x06, 0x00, 0xED, 0xB0, 0x3E, 0xC3, 0x21, 0x9A, 0xFD, 0x32, 0x38, 0x00, 0x22, 0x39,
              0x00, 0x3E, 0x40, 0x32, 0xA9, 0xD0, 0x32, 0xC8, 0xD0, 0x3E, 0x7F, 0x32, 0xAC, 0xD0, 0x32, 0xE8,
              0xD0, 0xCD, 0xA1, 0xD0, 0xC3, 0x06, 0xD0, 0x7C, 0x92, 0xC0, 0x7D, 0x93, 0xC9
            };
            CopyData(kssptr + 0x10, noDrumPatch, sizeof(noDrumPatch));
        }
        else
        {
            char drumKitName[13];
            char* drumKitExt;

            memcpy(drumKitName, data + 0x3f8b, 8);
            for (drumKitExt = drumKitName + 7;;)
            {
                if (drumKitExt < drumKitName || (*drumKitExt != ' ' && *drumKitExt != 0))
                {
                    drumKitExt[1] = '.'; drumKitExt[2] = 'S'; drumKitExt[3] = 'M'; drumKitExt[4] = '1'; drumKitExt[5] = 0;
                    break;
                }
                else
                    drumKitExt--;
            }

            SmartPtr<io::Stream> drumStreams[2];
            drumStreams[0] = stream->Open(drumKitName);
            if (drumStreams[0] && drumStreams[0]->Read().NumItems() >= 0x4007)
            {
                drumKitExt[4] = '2';
                drumStreams[1] = stream->Open(drumKitName);
                if (drumStreams[1] && drumStreams[1]->Read().NumItems() < 0x4007)
                    drumStreams[1].Reset();
            }
            if (drumStreams[0] && drumStreams[1])
            {
                static const uint8_t kssHead[] = {
                    0x4B, 0x53, 0x53, 0x58, 0x00, 0x00, 0xFF, 0xFF, 0x10, 0xC0, 0x38, 0x00, 0x00, 0x00, 0x10, 0x08,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // to force only 1 subsong
                };

                kssbuf.Resize(0x4000 + 0x8000 + sizeof(fac_player) + sizeof(kssHead));
                auto* kssptr = CopyData(kssbuf.Items(), kssHead, sizeof(kssHead));
                kssptr = CopyData(kssptr, drumStreams[0]->Read().Items(7), 0x4000);
                kssptr = CopyData(kssptr, drumStreams[1]->Read().Items(7), 0x4000);
                kssptr = CopyData(kssptr, data + 7, 0x4000);
                CopyData(kssptr, fac_player, sizeof(fac_player));
                kssptr[0xC] = 0;

                static const uint8_t drumPatch[] = {
                    0x21, 0x00, 0xC0, 0x11, 0x00, 0xD0, 0x01, 0x96, 0x08, 0xED, 0xB0, 0xAF, 0x32, 0x48, 0xD8, 0xCD,
                    0x03, 0xD0, 0x3E, 0x40, 0x32, 0x48, 0xD8, 0xCD, 0x03, 0xD0, 0x21, 0x00, 0x00, 0x11, 0x01, 0x00,
                    0x01, 0xFF, 0x3F, 0x36, 0xC9, 0xED, 0xB0, 0x21, 0x6D, 0xD0, 0x11, 0x20, 0x00, 0x01, 0x06, 0x00,
                    0xED, 0xB0, 0x3E, 0xC3, 0x21, 0x9A, 0xFD, 0x32, 0x38, 0x00, 0x22, 0x39, 0x00, 0x21, 0x00, 0x80,
                    0x11, 0x00, 0x40, 0xD5, 0xC1, 0xED, 0xB0, 0x3E, 0x40, 0x32, 0xA9, 0xD0, 0x32, 0xC8, 0xD0, 0x3E,
                    0x7F, 0x32, 0xAC, 0xD0, 0x32, 0xE8, 0xD0, 0xCD, 0xA1, 0xD0, 0xC3, 0x06, 0xD0, 0x7C, 0x92, 0xC0,
                    0x7D, 0x93, 0xC9
                };
                CopyData(kssptr + 0x10, drumPatch, sizeof(drumPatch));
            }
        }

        if (kssbuf.IsNotEmpty())
        {
            auto nezPlay = NEZNew();
            NEZLoad(nezPlay, kssbuf.Items(), kssbuf.NumItems());
            NEZSetFrequency(nezPlay, kSampleRate);
            NEZSetChannel(nezPlay, 2);
            NEZSetLength(nezPlay, 0);

            uint32_t strSize = isPro ? 36 : 41;
            free(nezPlay->songinfodata.title);
            nezPlay->songinfodata.title = reinterpret_cast<char*>(malloc(strSize + 1));
            memcpy(nezPlay->songinfodata.title, data + 0x3fad, strSize);
            nezPlay->songinfodata.title[strSize] = 0;
            free(nezPlay->songinfodata.artist);
            nezPlay->songinfodata.artist = reinterpret_cast<char*>(malloc(strSize + 1));
            memcpy(nezPlay->songinfodata.artist, data + 0x3fd6, strSize);
            nezPlay->songinfodata.artist[strSize] = 0;
            return nezPlay;
        }
        return nullptr;
    }
}
// namespace rePlayer