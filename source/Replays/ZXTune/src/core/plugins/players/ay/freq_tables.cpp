/**
 *
 * @file
 *
 * @brief  Frequency tables for AYM-based plugins
 *
 * @author vitamin.caig@gmail.com
 *
 **/

// local includes
#include "core/plugins/players/ay/freq_tables_internal.h"
// common includes
#include <error_tools.h>
// library includes
#include <l10n/api.h>
// std includes
#include <algorithm>

namespace Module
{
  const L10n::TranslateFunctor translate = L10n::TranslateFunctor("core_players");

  // prefix for reverted frequency tables
  const Char REVERT_TABLE_MARK = '~';

  struct FreqTableEntry
  {
    const Char* const Name;
    const FrequencyTable Table;
  };

  // clang-format off
  const FreqTableEntry TABLES[] =
  {
    //SoundTracker
    {
      TABLE_SOUNDTRACKER,
      { {
        0xef8, 0xe10, 0xd60, 0xc80, 0xbd8, 0xb28, 0xa88, 0x9f0, 0x960, 0x8e0, 0x858, 0x7e0,
        0x77c, 0x708, 0x6b0, 0x640, 0x5ec, 0x594, 0x544, 0x4f8, 0x4b0, 0x470, 0x42c, 0x3f0,
        0x3be, 0x384, 0x358, 0x320, 0x2f6, 0x2ca, 0x2a2, 0x27c, 0x258, 0x238, 0x216, 0x1f8,
        0x1df, 0x1c2, 0x1ac, 0x190, 0x17b, 0x165, 0x151, 0x13e, 0x12c, 0x11c, 0x10b, 0x0fc,
        0x0ef, 0x0e1, 0x0d6, 0x0c8, 0x0bd, 0x0b2, 0x0a8, 0x09f, 0x096, 0x08e, 0x085, 0x07e,
        0x077, 0x070, 0x06b, 0x064, 0x05e, 0x059, 0x054, 0x04f, 0x04b, 0x047, 0x042, 0x03f,
        0x03b, 0x038, 0x035, 0x032, 0x02f, 0x02c, 0x02a, 0x027, 0x025, 0x023, 0x021, 0x01f,
        0x01d, 0x01c, 0x01a, 0x019, 0x017, 0x016, 0x015, 0x013, 0x012, 0x011, 0x010, 0x00f
      } }
    },
    //ProTracker2.x
    {
      TABLE_PROTRACKER2,
      { {
        0xef8, 0xe10, 0xd60, 0xc80, 0xbd8, 0xb28, 0xa88, 0x9f0, 0x960, 0x8e0, 0x858, 0x7e0,
        0x77c, 0x708, 0x6b0, 0x640, 0x5ec, 0x594, 0x544, 0x4f8, 0x4b0, 0x470, 0x42c, 0x3fd,
        0x3be, 0x384, 0x358, 0x320, 0x2f6, 0x2ca, 0x2a2, 0x27c, 0x258, 0x238, 0x216, 0x1f8,
        0x1df, 0x1c2, 0x1ac, 0x190, 0x17b, 0x165, 0x151, 0x13e, 0x12c, 0x11c, 0x10a, 0x0fc,
        0x0ef, 0x0e1, 0x0d6, 0x0c8, 0x0bd, 0x0b2, 0x0a8, 0x09f, 0x096, 0x08e, 0x085, 0x07e,
        0x077, 0x070, 0x06b, 0x064, 0x05e, 0x059, 0x054, 0x04f, 0x04b, 0x047, 0x042, 0x03f,
        0x03b, 0x038, 0x035, 0x032, 0x02f, 0x02c, 0x02a, 0x027, 0x025, 0x023, 0x021, 0x01f,
        0x01d, 0x01c, 0x01a, 0x019, 0x017, 0x016, 0x015, 0x013, 0x012, 0x011, 0x010, 0x00f
      } }
    },
    //ProTracker3.3-3.4r
    {
      TABLE_PROTRACKER3_3,
      { {
        0x0c21, 0x0b73, 0x0ace, 0x0a33, 0x09a0, 0x0916, 0x0893, 0x0818, 0x07a4, 0x0736, 0x06ce, 0x066d,
        0x0610, 0x05b9, 0x0567, 0x0519, 0x04d0, 0x048b, 0x0449, 0x040c, 0x03d2, 0x039b, 0x0367, 0x0336,
        0x0308, 0x02dc, 0x02b3, 0x028c, 0x0268, 0x0245, 0x0224, 0x0206, 0x01e9, 0x01cd, 0x01b3, 0x019b,
        0x0184, 0x016e, 0x0159, 0x0146, 0x0134, 0x0122, 0x0112, 0x0103, 0x00f4, 0x00e6, 0x00d9, 0x00cd,
        0x00c2, 0x00b7, 0x00ac, 0x00a3, 0x009a, 0x0091, 0x0089, 0x0081, 0x007a, 0x0073, 0x006c, 0x0066,
        0x0061, 0x005b, 0x0056, 0x0051, 0x004d, 0x0048, 0x0044, 0x0040, 0x003d, 0x0039, 0x0036, 0x0033,
        0x0030, 0x002d, 0x002b, 0x0028, 0x0026, 0x0024, 0x0022, 0x0020, 0x001e, 0x001c, 0x001b, 0x0019,
        0x0018, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d, 0x000c
      } }
    },
    //ProTracker3.4-3.5
    {
      TABLE_PROTRACKER3_4,
      { {
        0x0c22, 0x0b73, 0x0acf, 0x0a33, 0x09a1, 0x0917, 0x0894, 0x0819, 0x07a4, 0x0737, 0x06cf, 0x066d,
        0x0611, 0x05ba, 0x0567, 0x051a, 0x04d0, 0x048b, 0x044a, 0x040c, 0x03d2, 0x039b, 0x0367, 0x0337,
        0x0308, 0x02dd, 0x02b4, 0x028d, 0x0268, 0x0246, 0x0225, 0x0206, 0x01e9, 0x01ce, 0x01b4, 0x019b,
        0x0184, 0x016e, 0x015a, 0x0146, 0x0134, 0x0123, 0x0112, 0x0103, 0x00f5, 0x00e7, 0x00da, 0x00ce,
        0x00c2, 0x00b7, 0x00ad, 0x00a3, 0x009a, 0x0091, 0x0089, 0x0082, 0x007a, 0x0073, 0x006d, 0x0067,
        0x0061, 0x005c, 0x0056, 0x0052, 0x004d, 0x0049, 0x0045, 0x0041, 0x003d, 0x003a, 0x0036, 0x0033,
        0x0031, 0x002e, 0x002b, 0x0029, 0x0027, 0x0024, 0x0022, 0x0020, 0x001f, 0x001d, 0x001b, 0x001a,
        0x0018, 0x0017, 0x0016, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d, 0x000c
      } }
    },
    //ProTracker3.x
    {
      TABLE_PROTRACKER3_ST,
      { {
        0x0EF8, 0x0E10, 0x0D60, 0x0C80, 0x0BD8, 0x0B28, 0x0A88, 0x09F0, 0x0960, 0x08E0, 0x0858, 0x07E0,
        0x077C, 0x0708, 0x06B0, 0x0640, 0x05EC, 0x0594, 0x0544, 0x04F8, 0x04B0, 0x0470, 0x042C, 0x03FD,
        0x03BE, 0x0384, 0x0358, 0x0320, 0x02F6, 0x02CA, 0x02A2, 0x027C, 0x0258, 0x0238, 0x0216, 0x01F8,
        0x01DF, 0x01C2, 0x01AC, 0x0190, 0x017B, 0x0165, 0x0151, 0x013E, 0x012C, 0x011C, 0x010A, 0x00FC,
        0x00EF, 0x00E1, 0x00D6, 0x00C8, 0x00BD, 0x00B2, 0x00A8, 0x009F, 0x0096, 0x008E, 0x0085, 0x007E,
        0x0077, 0x0070, 0x006B, 0x0064, 0x005E, 0x0059, 0x0054, 0x004F, 0x004B, 0x0047, 0x0042, 0x003F,
        0x003B, 0x0038, 0x0035, 0x0032, 0x002F, 0x002C, 0x002A, 0x0027, 0x0025, 0x0023, 0x0021, 0x001F,
        0x001D, 0x001C, 0x001A, 0x0019, 0x0017, 0x0016, 0x0015, 0x0013, 0x0012, 0x0011, 0x0010, 0x000F
      } }
    },
    //ProTracker3.4r
    {
      TABLE_PROTRACKER3_3_ASM,
      { {
        0x0d3e, 0x0c80, 0x0bcc, 0x0b22, 0x0a82, 0x09ec, 0x095c, 0x08d6, 0x0858, 0x07e0, 0x076e, 0x0704,
        0x069f, 0x0640, 0x05e6, 0x0591, 0x0541, 0x04f6, 0x04ae, 0x046b, 0x042c, 0x03f0, 0x03b7, 0x0382,
        0x034f, 0x0320, 0x02f3, 0x02c8, 0x02a1, 0x027b, 0x0257, 0x0236, 0x0216, 0x01f8, 0x01dc, 0x01c1,
        0x01a8, 0x0190, 0x0179, 0x0164, 0x0150, 0x013d, 0x012c, 0x011b, 0x010b, 0x00fc, 0x00ee, 0x00e0,
        0x00d4, 0x00c8, 0x00bd, 0x00b2, 0x00a8, 0x009f, 0x0096, 0x008d, 0x0085, 0x007e, 0x0077, 0x0070,
        0x006a, 0x0064, 0x005e, 0x0059, 0x0054, 0x0050, 0x004b, 0x0047, 0x0043, 0x003f, 0x003c, 0x0038,
        0x0035, 0x0032, 0x002f, 0x002d, 0x002a, 0x0028, 0x0026, 0x0024, 0x0022, 0x0020, 0x001e, 0x001d,
        0x001b, 0x001a, 0x0019, 0x0018, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e
      } }
    },
    //ProTracker3.4-3.5
    {
      TABLE_PROTRACKER3_4_ASM,
      { {
        0x0d10, 0x0c55, 0x0ba4, 0x0afc, 0x0a5f, 0x09ca, 0x093d, 0x08b8, 0x083b, 0x07c5, 0x0755, 0x06ec,
        0x0688, 0x062a, 0x05d2, 0x057e, 0x052f, 0x04e5, 0x049e, 0x045c, 0x041d, 0x03e2, 0x03ab, 0x0376,
        0x0344, 0x0315, 0x02e9, 0x02bf, 0x0298, 0x0272, 0x024f, 0x022e, 0x020f, 0x01f1, 0x01d5, 0x01bb,
        0x01a2, 0x018b, 0x0174, 0x0160, 0x014c, 0x0139, 0x0128, 0x0117, 0x0107, 0x00f9, 0x00eb, 0x00dd,
        0x00d1, 0x00c5, 0x00ba, 0x00b0, 0x00a6, 0x009d, 0x0094, 0x008c, 0x0084, 0x007c, 0x0075, 0x006f,
        0x0069, 0x0063, 0x005d, 0x0058, 0x0053, 0x004e, 0x004a, 0x0046, 0x0042, 0x003e, 0x003b, 0x0037,
        0x0034, 0x0031, 0x002f, 0x002c, 0x0029, 0x0027, 0x0025, 0x0023, 0x0021, 0x001f, 0x001d, 0x001c,
        0x001a, 0x0019, 0x0017, 0x0016, 0x0015, 0x0014, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d
      } }
    },
    //ProTracker3.3
    {
      TABLE_PROTRACKER3_3_REAL,
      { {
        0x0cda, 0x0c22, 0x0b73, 0x0acf, 0x0a33, 0x09a1, 0x0917, 0x0894, 0x0819, 0x07a4, 0x0737, 0x06cf,
        0x066d, 0x0611, 0x05ba, 0x0567, 0x051a, 0x04d0, 0x048b, 0x044a, 0x040c, 0x03d2, 0x039b, 0x0367,
        0x0337, 0x0308, 0x02dd, 0x02b4, 0x028d, 0x0268, 0x0246, 0x0225, 0x0206, 0x01e9, 0x01ce, 0x01b4,
        0x019b, 0x0184, 0x016e, 0x015a, 0x0146, 0x0134, 0x0123, 0x0113, 0x0103, 0x00f5, 0x00e7, 0x00da,
        0x00ce, 0x00c2, 0x00b7, 0x00ad, 0x00a3, 0x009a, 0x0091, 0x0089, 0x0082, 0x007a, 0x0073, 0x006d,
        0x0067, 0x0061, 0x005c, 0x0056, 0x0052, 0x004d, 0x0049, 0x0045, 0x0041, 0x003d, 0x003a, 0x0036,
        0x0033, 0x0031, 0x002e, 0x002b, 0x0029, 0x0027, 0x0024, 0x0022, 0x0020, 0x001f, 0x001d, 0x001b,
        0x001a, 0x0018, 0x0017, 0x0016, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d
      } }
    },
    //ProTracker3.4
    {
      TABLE_PROTRACKER3_4_REAL,
      { {
        0x0cda, 0x0c22, 0x0b73, 0x0acf, 0x0a33, 0x09a1, 0x0917, 0x0894, 0x0819, 0x07a4, 0x0737, 0x06cf,
        0x066d, 0x0611, 0x05ba, 0x0567, 0x051a, 0x04d0, 0x048b, 0x044a, 0x040c, 0x03d2, 0x039b, 0x0367,
        0x0337, 0x0308, 0x02dd, 0x02b4, 0x028d, 0x0268, 0x0246, 0x0225, 0x0206, 0x01e9, 0x01ce, 0x01b4,
        0x019b, 0x0184, 0x016e, 0x015a, 0x0146, 0x0134, 0x0123, 0x0112, 0x0103, 0x00f5, 0x00e7, 0x00da,
        0x00ce, 0x00c2, 0x00b7, 0x00ad, 0x00a3, 0x009a, 0x0091, 0x0089, 0x0082, 0x007a, 0x0073, 0x006d,
        0x0067, 0x0061, 0x005c, 0x0056, 0x0052, 0x004d, 0x0049, 0x0045, 0x0041, 0x003d, 0x003a, 0x0036,
        0x0033, 0x0031, 0x002e, 0x002b, 0x0029, 0x0027, 0x0024, 0x0022, 0x0020, 0x001f, 0x001d, 0x001b,
        0x001a, 0x0018, 0x0017, 0x0016, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d
      } }
    },
    //ASC
    {
      TABLE_ASM,
      { {
        0xedc, 0xe07, 0xd3e, 0xc80, 0xbcc, 0xb22, 0xa82, 0x9ec, 0x95c, 0x8d6, 0x858, 0x7e0,
        0x76e, 0x704, 0x69f, 0x640, 0x5e6, 0x591, 0x541, 0x4f6, 0x4ae, 0x46b, 0x42c, 0x3f0,
        0x3b7, 0x382, 0x34f, 0x320, 0x2f3, 0x2c8, 0x2a1, 0x27b, 0x257, 0x236, 0x216, 0x1f8,
        0x1dc, 0x1c1, 0x1a8, 0x190, 0x179, 0x164, 0x150, 0x13d, 0x12c, 0x11b, 0x10b, 0x0fc,
        0x0ee, 0x0e0, 0x0d4, 0x0c8, 0x0bd, 0x0b2, 0x0a8, 0x09f, 0x096, 0x08d, 0x085, 0x07e,
        0x077, 0x070, 0x06a, 0x064, 0x05e, 0x059, 0x054, 0x050, 0x04b, 0x047, 0x043, 0x03f,
        0x03c, 0x038, 0x035, 0x032, 0x02f, 0x02d, 0x02a, 0x028, 0x026, 0x024, 0x022, 0x020,
        0x01e, 0x01c,
        0x01a, 0x019, 0x017, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010
      } }
    },
    //SoundTrackerPro
    {
      TABLE_SOUNDTRACKER_PRO,
      { {
        0x0ef8, 0x0e10, 0x0d60, 0x0c80, 0x0bd8, 0x0b28, 0x0a88, 0x09f0, 0x0960, 0x08e0, 0x0858, 0x07e0,
        0x077c, 0x0708, 0x06b0, 0x0640, 0x05ec, 0x0594, 0x0544, 0x04f8, 0x04b0, 0x0470, 0x042c, 0x03f0,
        0x03be, 0x0384, 0x0358, 0x0320, 0x02f6, 0x02ca, 0x02a2, 0x027c, 0x0258, 0x0238, 0x0216, 0x01f8,
        0x01df, 0x01c2, 0x01ac, 0x0190, 0x017b, 0x0165, 0x0151, 0x013e, 0x012c, 0x011c, 0x010b, 0x00fc,
        0x00ef, 0x00e1, 0x00d6, 0x00c8, 0x00bd, 0x00b2, 0x00a8, 0x009f, 0x0096, 0x008e, 0x0085, 0x007e,
        0x0077, 0x0070, 0x006b, 0x0064, 0x005e, 0x0059, 0x0054, 0x004f, 0x004b, 0x0047, 0x0042, 0x003f,
        0x003b, 0x0038, 0x0035, 0x0032, 0x002f, 0x002c, 0x002a, 0x0027, 0x0025, 0x0023, 0x0021, 0x001f,
        0x001d, 0x001c, 0x001a, 0x0019, 0x0017, 0x0016, 0x0015, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f
      } }
    },
    //NaturalScaled
    {
      TABLE_NATURAL_SCALED,
      { {
        2880, 2700, 2560, 2400, 2304, 2160, 2025, 1920, 1800, 1728, 1620, 1536,
        1440, 1350, 1280, 1200, 1152, 1080, 1013,  960,  900,  864,  810,  768,
         720,  675,  640,  600,  576,  540,  506,  480,  450,  432,  405,  384,
         360,  338,  320,  300,  288,  270,  253,  240,  225,  216,  203,  192,
         180,  169,  160,  150,  144,  135,  127,  120,  113,  108,  101,   96,
          90,   84,   80,   75,   72,   68,   63,   60,   56,   54,   51,   48,
          45,   42,   40,   38,   36,   34,   32,   30,   28,   27,   25,   24,
          23,   21,   20,   19,   18,   17,   16,   15,   14,   14,   13,   12
      } }
    },
    //ProSoundMaker
    {
      TABLE_PROSOUNDMAKER,
      { {
        0xD3D, 0xC7F, 0xBCB, 0xB22, 0xA82, 0x9EB, 0x95D, 0x8D6, 0x857, 0x7DF, 0x76E, 0x703,
        0x69F, 0x63F, 0x5E6, 0x591, 0x541, 0x4F6, 0x4AE, 0x46B, 0x42C, 0x3F0, 0x3B7, 0x382,
        0x34F, 0x320, 0x2F3, 0x2C8, 0x2A1, 0x27B, 0x257, 0x236, 0x216, 0x1F8, 0x1DC, 0x1C1,
        0x1A8, 0x190, 0x179, 0x164, 0x150, 0x13D, 0x12C, 0x11B, 0x10B, 0x0FC, 0x0EE, 0x0E0,
        0x0D4, 0x0C8, 0x0BD, 0x0B2, 0x0A8, 0x09F, 0x096, 0x08D, 0x085, 0x07E, 0x077, 0x070,
        0x06A, 0x064, 0x05E, 0x059, 0x054, 0x04F, 0x04B, 0x047, 0x043, 0x03F, 0x03B, 0x038,
        0x035, 0x032, 0x02F, 0x02D, 0x02A, 0x028, 0x025, 0x023, 0x021, 0x01F, 0x01E, 0x01C,
        0x01A, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00F, 0x00E
      } }
    },
    //SQTracker
    {
      TABLE_SQTRACKER,
      { {
        0xd5d, 0xc9c, 0xbe7, 0xb3c, 0xa9b, 0xa02, 0x973, 0x8eb, 0x86b, 0x7f2, 0x780, 0x714,
        0x6ae, 0x64e, 0x5f4, 0x59e, 0x54f, 0x501, 0x4b9, 0x475, 0x435, 0x3f9, 0x3c0, 0x38a,
        0x357, 0x327, 0x2fa, 0x2cf, 0x2a7, 0x281, 0x25d, 0x23b, 0x21b, 0x1fc, 0x1e0, 0x1c5,
        0x1ac, 0x194, 0x17d, 0x168, 0x153, 0x140, 0x12e, 0x11d, 0x10d, 0x0fe, 0x0f0, 0x0e2,
        0x0d6, 0x0ca, 0x0be, 0x0b4, 0x0aa, 0x0a0, 0x097, 0x08f, 0x087, 0x07f, 0x078, 0x071,
        0x06b, 0x065, 0x05f, 0x05a, 0x055, 0x050, 0x04c, 0x047, 0x043, 0x040, 0x03c, 0x039,
        0x035, 0x032, 0x030, 0x02d, 0x02a, 0x028, 0x026, 0x024, 0x022, 0x020, 0x01e, 0x01c,
        0x01b, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00f, 0x00e
      } }
    },
    //Fuxoft
    {
      TABLE_FUXOFT,
      { {
        0xfbf, 0xedc, 0xe07, 0xd3d, 0xc7f, 0xbcc, 0xb22, 0xa82, 0x9eb, 0x95d, 0x8d6, 0x857, 0x7df, 0x76e, 0x703,
        0x69f, 0x640, 0x5e6, 0x591, 0x541, 0x4f6, 0x4ae, 0x46b, 0x42c, 0x3f0, 0x3b7, 0x382, 0x34f, 0x320, 0x2f3,
        0x2c8, 0x2a1, 0x27b, 0x257, 0x236, 0x216, 0x1f8, 0x1dc, 0x1c1, 0x1a8, 0x190, 0x179, 0x164, 0x150, 0x13d,
        0x12c, 0x11b, 0x10b, 0x0fc, 0x0ee, 0x0e0, 0x0d4, 0x0c8, 0x0bd, 0x0b2, 0x0a8, 0x09f, 0x096, 0x08d, 0x085,
        0x07e, 0x077, 0x070, 0x06a, 0x064, 0x05e, 0x059, 0x054, 0x04f, 0x04b, 0x047, 0x043, 0x03f, 0x03b, 0x038,
        0x035, 0x032, 0x02f, 0x02d, 0x02a, 0x028, 0x025, 0x023, 0x021
      } }
    },
    //FastTracker
    {
      TABLE_FASTTRACKER,
      { {
        0xD10, 0xC58, 0xBA0, 0xB00, 0xA60, 0x9C8, 0x940, 0x8B8, 0x840, 0x7C0, 0x750, 0x6F0,
        0x688, 0x62C, 0x5D0, 0x580, 0x530, 0x4E4, 0x4A0, 0x45C, 0x420, 0x3E0, 0x3A8, 0x378,
        0x344, 0x316, 0x2E8, 0x2C0, 0x298, 0x272, 0x250, 0x22E, 0x210, 0x1F0, 0x1D4, 0x1BC,
        0x1A2, 0x18B, 0x174, 0x160, 0x14C, 0x139, 0x128, 0x117, 0x108, 0xF8,  0xEA,  0xDE,
        0xD1,  0xC5,  0xBA,  0xB0,  0xA6,  0x9C,  0x94,  0x8B,  0x84,  0x7C,  0x75,  0x6F,
        0x68,  0x62,  0x5D,  0x58,  0x53,  0x4E,  0x4A,  0x45,  0x42,  0x3E,  0x3A,  0x37,
        0x34,  0x31,  0x2E,  0x2C,  0x29,  0x27,  0x25,  0x22,  0x21,  0x1F,  0x1D,  0x1B,
        0x1A,  0x18,  0x17,  0x16,  0x14,  0x13,  0x12,  0x11,  0x10,  0xF,   0xE,   0xD
      } }
    }
  };
  // clang-format on

  void GetFreqTable(const String& id, FrequencyTable& result)
  {
    // check if required to revert table
    const bool doRevert = !id.empty() && *id.begin() == REVERT_TABLE_MARK;
    const String idNormal = doRevert ? id.substr(1) : id;
    // find if table is supported
    const auto* entry = std::find_if(TABLES, std::end(TABLES),
                                     [&idNormal](const FreqTableEntry& entry) { return entry.Name == idNormal; });
    if (entry == std::end(TABLES))
    {
      throw MakeFormattedError(THIS_LINE, translate("Invalid frequency table '{}'."), id);
    }
    // copy result forward (normal) or backward (reverted)
    if (doRevert)
    {
      std::copy(entry->Table.rbegin(), entry->Table.rend(), result.begin());
    }
    else
    {
      std::copy(entry->Table.begin(), entry->Table.end(), result.begin());
    }
  }
}  // namespace Module

