#include <math.h>
#include "meta.h"
#include "../layout/layout.h"
#include "../coding/coding.h"
#include "../util/endianness.h"
#include "ea_eaac_streamfile.h"

/* EAAudioCore (aka SND10) formats, EA's current audio middleware.
 * Due to the big number of sub-formats that use EAAC, this file doesn't parse anything by itself
 * and is called with config externally instead. */

#define EAAC_VERSION_V0                 0x00 /* SNR/SNS */
#define EAAC_VERSION_V1                 0x01 /* SPS */

#define EAAC_CODEC_NONE                 0x00
#define EAAC_CODEC_RESERVED             0x01 /* EALAYER3 V1a? MP30/P6L0/P2B0/P2L0/P8S0/P8U0/PFN0? */
#define EAAC_CODEC_PCM16BE              0x02
#define EAAC_CODEC_EAXMA                0x03
#define EAAC_CODEC_XAS1                 0x04
#define EAAC_CODEC_EALAYER3_V1          0x05
#define EAAC_CODEC_EALAYER3_V2_PCM      0x06
#define EAAC_CODEC_EALAYER3_V2_SPIKE    0x07
#define EAAC_CODEC_GCADPCM              0x08
#define EAAC_CODEC_EASPEEX              0x09
#define EAAC_CODEC_EATRAX               0x0a
#define EAAC_CODEC_EAMP3                0x0b
#define EAAC_CODEC_EAOPUS               0x0c
#define EAAC_CODEC_EAATRAC9             0x0d
#define EAAC_CODEC_EAOPUSM              0x0e
#define EAAC_CODEC_EAOPUSMU             0x0f

#define EAAC_TYPE_RAM                   0x00
#define EAAC_TYPE_STREAM                0x01
#define EAAC_TYPE_GIGASAMPLE            0x02

#define EAAC_BLOCKID0_DATA              0x00
#define EAAC_BLOCKID0_END               0x80 /* maybe meant to be a bitflag? */

#define EAAC_BLOCKID1_HEADER            0x48 /* 'H' */
#define EAAC_BLOCKID1_DATA              0x44 /* 'D' */
#define EAAC_BLOCKID1_END               0x45 /* 'E' */

static VGMSTREAM* init_vgmstream_eaaudiocore_header(STREAMFILE* sf_head, STREAMFILE* sf_data, off_t header_offset, off_t start_offset, meta_t meta_type, bool standalone, bool is_sps);

VGMSTREAM* load_vgmstream_ea_eaac(eaac_meta_t* info) {
    info->is_sps = info->is_sps || info->type == meta_EA_SPS;
    return init_vgmstream_eaaudiocore_header(info->sf_head, info->sf_body, info->head_offset, info->body_offset, info->type, info->standalone, info->is_sps);
}


typedef struct {
    int version;
    int codec;
    int channel_config;
    int sample_rate;
    int type;

    int streamed;
    int channels;

    int num_samples;
    int loop_start;
    int loop_end;
    int loop_flag;
    int prefetch_samples;

    off_t stream_offset;
    off_t loop_offset;
    off_t prefetch_offset;

    uint32_t header_size;
} eaac_header_t;

static VGMSTREAM* init_vgmstream_eaaudiocore_main(eaac_header_t* eaac, STREAMFILE* sf_head, STREAMFILE* sf_data, off_t header_offset, off_t start_offset, meta_t meta_type, bool standalone);


/* EA newest header from RwAudioCore (RenderWare?) / EAAudioCore library (still generated by sx.exe).
 * Audio "assets" come in separate RAM headers (.SNR/SPH) and raw blocked streams (.SNS/SPS),
 * or together in pseudoformats (.SNU, .SBR+.SBS banks, .AEMS, .MUS, etc).
 * Some .SNR include stream data, while .SPS have headers so .SPH is optional. */
static VGMSTREAM* init_vgmstream_eaaudiocore_header(STREAMFILE* sf_head, STREAMFILE* sf_data, off_t header_offset, off_t start_offset, meta_t meta_type, bool standalone, bool is_sps) {
    eaac_header_t eaac = {0};

    /* SPS put the header in the first block */
    uint32_t header_block_size = 0;
    if (is_sps) {
        uint32_t header_block_info = read_u32be(header_offset, sf_head);
        uint8_t header_block_id;

        header_block_id     = (header_block_info >> 24) & 0xFF;
        header_block_size   = (header_block_info >>  0) & 0x00FFFFFF;
        if (header_block_id != EAAC_BLOCKID1_HEADER)
            return NULL;

        header_offset += 0x04;
    }

    /* basic EAAC (SNR/SPH) header */
    uint32_t header1 = read_u32be(header_offset + 0x00, sf_head);
    uint32_t header2 = read_u32be(header_offset + 0x04, sf_head);
    eaac.version        = (header1 >> 28) & 0x0F; /* 4 bits */
    eaac.codec          = (header1 >> 24) & 0x0F; /* 4 bits */
    eaac.channel_config = (header1 >> 18) & 0x3F; /* 6 bits */
    eaac.sample_rate    = (header1 >>  0) & 0x03FFFF; /* 18 bits */
    eaac.type           = (header2 >> 30) & 0x03; /* 2 bits */
    eaac.loop_flag      = (header2 >> 29) & 0x01; /* 1 bit */
    eaac.num_samples    = (header2 >>  0) & 0x1FFFFFFF; /* 29 bits */
    /* rest is optional, depends on used flags and codec (handled below) */

    /* common channel configs are mono/stereo/quad/5.1/7.1 (from debug strings), while others are quite rare
     * [Battlefield 4 (X360)-EAXMA: 3/5/7ch, Army of Two: The Devil's Cartel (PS3)-EALayer3v2P: 11ch] */
    eaac.channels = eaac.channel_config + 1;

    /* EA 6ch channel mapping is FL FC FR BL BR LFE, but may use stereo layers for dynamic music
     * instead, so we can't re-map automatically (use TXTP) */

    /* V0: SNR+SNS, V1: SPH+SPS (no differences, other than block flags) */
    if (eaac.version != EAAC_VERSION_V0 && eaac.version != EAAC_VERSION_V1) {
        VGM_LOG("EA EAAC: unknown version\n");
        goto fail;
    }

    /* official max (some Dead Space 2 (PC) do use 96000) */
    if (eaac.sample_rate > 200000) {
        VGM_LOG("EA EAAC: unknown sample rate\n");
        goto fail;
    }

    /* catch unknown values */
    if (eaac.type != EAAC_TYPE_RAM && eaac.type != EAAC_TYPE_STREAM && eaac.type != EAAC_TYPE_GIGASAMPLE) {
        VGM_LOG("EA EAAC: unknown type 0x%02x\n", eaac.type);
        goto fail;
    }

    if (eaac.version == EAAC_VERSION_V1 && eaac.type == EAAC_TYPE_GIGASAMPLE) {
        /* probably does not exist */
        VGM_LOG("EA EAAC: bad stream type for version %x\n", eaac.version);
        goto fail;
    }

    /* Non-streamed sounds are stored as a single block (may not set block end flags) */
    eaac.streamed = (eaac.type != EAAC_TYPE_RAM);

    /* get loops (fairly involved due to the multiple layouts and mutant streamfiles)
     * full loops aren't too uncommon [Dead Space (PC) stream sfx/ambiance, FIFA 08 (PS3) RAM sfx],
     * while actual looping is very rare [Need for Speed: World (PC)-EAL3, The Simpsons Game (X360)-EAXMA] */

    /* get optional header values */
    eaac.header_size = 0x08;
    if (eaac.loop_flag) {
        eaac.header_size += 0x04;
        eaac.loop_start = read_s32be(header_offset + 0x08, sf_head);
        eaac.loop_end = eaac.num_samples;

        /* TODO: need more cases to test how layout/streamfiles react */
        if (eaac.loop_start > 0 && !(
            eaac.codec == EAAC_CODEC_EALAYER3_V1 ||
            eaac.codec == EAAC_CODEC_EALAYER3_V2_PCM ||
            eaac.codec == EAAC_CODEC_EALAYER3_V2_SPIKE ||
            eaac.codec == EAAC_CODEC_EAXMA ||
            eaac.codec == EAAC_CODEC_XAS1 ||
            eaac.codec == EAAC_CODEC_EATRAX)) {
            VGM_LOG("EA EAAC: unknown actual looping %i for codec %x\n", eaac.loop_start, eaac.codec);
            goto fail;
        }
    }

    switch (eaac.type) {
        case EAAC_TYPE_RAM:
            break;
        case EAAC_TYPE_STREAM:
            if (eaac.loop_flag) {
                eaac.header_size += 0x04;
                eaac.loop_offset = read_u32be(header_offset + 0x0c, sf_head);
            }
            break;
        case EAAC_TYPE_GIGASAMPLE: /* rarely seen [Def Jam Icon (X360)] */
            eaac.header_size += 0x04;
            eaac.prefetch_samples = read_s32be(header_offset + (eaac.loop_flag ? 0x0c : 0x08), sf_head);

            if (eaac.loop_flag && eaac.loop_start >= eaac.prefetch_samples) {
                eaac.header_size += 0x04;
                eaac.loop_offset = read_u32be(header_offset + 0x10, sf_head);
            }
            break;
        default:
            goto fail;
    }

    /* get data offsets */
    if (eaac.version == EAAC_VERSION_V0) {
        switch (eaac.type) {
            case EAAC_TYPE_RAM:
                eaac.stream_offset = header_offset + eaac.header_size;
                break;
            case EAAC_TYPE_STREAM:
                eaac.stream_offset = start_offset;
                break;
            case EAAC_TYPE_GIGASAMPLE:
                eaac.prefetch_offset = header_offset + eaac.header_size;
                eaac.stream_offset = start_offset;
                break;
            default:
                goto fail;
        }
    } else {
        eaac.stream_offset = header_offset - 0x04 + header_block_size;
    }

    /* correct loop offsets */
    if (eaac.loop_flag) {
        if (eaac.streamed) {
            /* SNR+SNS are separate so offsets are relative to the data start
             * (first .SNS block, or extra data before the .SNS block in case of .SNU)
             * SPS have headers+data together so offsets are relative to the file start [ex. FIFA 18 (PC)] */
            if (eaac.version == EAAC_VERSION_V0) {
                if (eaac.prefetch_samples != 0) {
                    if (eaac.loop_start == 0) {
                        /* loop from the beginning */
                        eaac.loop_offset = 0x00;
                    }
                    else if (eaac.loop_start < eaac.prefetch_samples) {
                        /* loop from the second RAM block */
                        eaac.loop_offset = read_u32be(eaac.prefetch_offset, sf_head) & 0x00FFFFFF;
                    }
                    else {
                        /* loop from offset within SNS */
                        eaac.loop_offset += read_u32be(eaac.prefetch_offset, sf_head) & 0x00FFFFFF;
                    }
                }
            } else {
                eaac.loop_offset -= header_block_size;
            }
        }
        else if (eaac.loop_start > 0) {
            /* RAM assets have two blocks in case of actual loops */
            /* find the second block by getting the first block size */
            eaac.loop_offset = read_u32be(eaac.stream_offset, sf_head) & 0x00FFFFFF;
        }
        else {
            /* RAM assets have only one block in case of full loops */
            eaac.loop_offset = 0x00; /* implicit */
        }
    }

    /* header done, setup decoding */
    return init_vgmstream_eaaudiocore_main(&eaac, sf_head, sf_data, header_offset, start_offset, meta_type, standalone);
fail:
    return NULL;
}

static segmented_layout_data* build_segmented_eaaudiocore_looping(STREAMFILE* sf_head, STREAMFILE* sf_data, eaac_header_t* eaac);
static layered_layout_data* build_layered_eaaudiocore(STREAMFILE* sf, eaac_header_t *eaac, off_t start_offset);
static STREAMFILE *setup_eaac_streamfile(eaac_header_t *ea, STREAMFILE* sf_head, STREAMFILE* sf_data);
static size_t calculate_eaac_size(STREAMFILE* sf, eaac_header_t *ea, uint32_t num_samples, off_t start_offset, int is_ram);


static VGMSTREAM* init_vgmstream_eaaudiocore_main(eaac_header_t* eaac, STREAMFILE* sf_head, STREAMFILE* sf_data, off_t header_offset, off_t _start_offset, meta_t meta_type, bool standalone) {
    VGMSTREAM* vgmstream = NULL;
    STREAMFILE *temp_sf = NULL, *sf = NULL, *sf_sns = NULL;

    if (eaac->version == EAAC_VERSION_V0 && eaac->streamed) {
        /* open SNS file if needed */
        if (standalone) {
            sf_sns = open_streamfile_by_ext(sf_head, "sns"); //todo clean
            sf_data = sf_sns;
        }
        if (!sf_data) goto fail;
    }

    /* build streamfile with audio data */
    sf = setup_eaac_streamfile(eaac, sf_head, sf_data);
    if (!sf) goto fail;

    /* build the VGMSTREAM */
    vgmstream = allocate_vgmstream(eaac->channels, eaac->loop_flag);
    if (!vgmstream) goto fail;

    vgmstream->sample_rate = eaac->sample_rate;
    vgmstream->num_samples = eaac->num_samples;
    vgmstream->loop_start_sample = eaac->loop_start;
    vgmstream->loop_end_sample = eaac->loop_end;
    vgmstream->meta_type = meta_type;
    vgmstream->stream_size = get_streamfile_size(sf);

    /* EA decoder list and known internal FourCCs */
    switch(eaac->codec) {

        case EAAC_CODEC_PCM16BE: /* "P6B0": PCM16BE [NBA Jam (Wii)] */
            vgmstream->coding_type = coding_PCM16_int;
            vgmstream->codec_endian = 1;
            vgmstream->layout_type = layout_blocked_ea_sns;
            break;

#ifdef VGM_USE_FFMPEG
        case EAAC_CODEC_EAXMA: { /* "EXm0": EA-XMA [Dante's Inferno (X360)] */

            /* special (if hacky) loop handling, see comments */
            if (eaac->loop_start > 0) {
                segmented_layout_data *data = build_segmented_eaaudiocore_looping(sf, sf, eaac);
                if (!data) goto fail;
                vgmstream->layout_data = data;
                vgmstream->coding_type = data->segments[0]->coding_type;
                vgmstream->layout_type = layout_segmented;
            }
            else {
                vgmstream->layout_data = build_layered_eaaudiocore(sf, eaac, 0x00);
                if (!vgmstream->layout_data) goto fail;
                vgmstream->coding_type = coding_FFmpeg;
                vgmstream->layout_type = layout_layered;
            }

            break;
        }
#endif

        case EAAC_CODEC_XAS1: /* "Xas1": EA-XAS v1 [Dead Space (PC/PS3)] */

            /* special (if hacky) loop handling, see comments */
            if (eaac->loop_start > 0) {
                segmented_layout_data *data = build_segmented_eaaudiocore_looping(sf, sf, eaac);
                if (!data) goto fail;
                vgmstream->layout_data = data;
                vgmstream->coding_type = data->segments[0]->coding_type;
                vgmstream->layout_type = layout_segmented;
            } else {
                vgmstream->coding_type = coding_EA_XAS_V1;
                vgmstream->layout_type = layout_blocked_ea_sns;
            }

            break;

#ifdef VGM_USE_MPEG
        case EAAC_CODEC_EALAYER3_V1:         /* "EL31": EALayer3 v1 [Need for Speed: Hot Pursuit (PS3)] */
        case EAAC_CODEC_EALAYER3_V2_PCM:     /* "L32P": EALayer3 v2 "PCM" [Battlefield 1943 (PS3)] */
        case EAAC_CODEC_EALAYER3_V2_SPIKE: { /* "L32S": EALayer3 v2 "Spike" [Dante's Inferno (PS3)] */
            mpeg_custom_config cfg = {0};
            mpeg_custom_t type = (eaac->codec == 0x05 ? MPEG_EAL31b : (eaac->codec == 0x06) ? MPEG_EAL32P : MPEG_EAL32S);

            /* EALayer3 needs custom IO that removes blocks on reads to fix some edge cases in L32P
             * and to properly apply discard modes (see ealayer3 decoder)
             * (otherwise, and after removing discard code, it'd work with layout_blocked_ea_sns) */

            /* special (if hacky) loop handling, see comments */
            if (eaac->loop_start > 0) {
                segmented_layout_data *data = build_segmented_eaaudiocore_looping(sf, sf, eaac);
                if (!data) goto fail;
                vgmstream->layout_data = data;
                vgmstream->coding_type = data->segments[0]->coding_type;
                vgmstream->layout_type = layout_segmented;
            }
            else {
                temp_sf = setup_eaac_audio_streamfile(sf, eaac->version, eaac->codec, eaac->streamed,0,0, 0x00, 0);
                if (!temp_sf) goto fail;

                vgmstream->codec_data = init_mpeg_custom(temp_sf, 0x00, &vgmstream->coding_type, vgmstream->channels, type, &cfg);
                if (!vgmstream->codec_data) goto fail;
                vgmstream->layout_type = layout_none;
            }

            break;
        }
#endif

        case EAAC_CODEC_GCADPCM: /* "Gca0": DSP [Need for Speed: Nitro (Wii) sfx] */
            vgmstream->coding_type = coding_NGC_DSP;
            vgmstream->layout_type = layout_blocked_ea_sns;
            /* DSP coefs are read in the blocks */
            break;

#ifdef VGM_USE_SPEEX
        case EAAC_CODEC_EASPEEX: { /* "Esp0": EASpeex (libspeex variant, base versions vary: 1.0.5, 1.2beta3) [FIFA 14 (PS4), FIFA 2020 (Switch)] */
            /* EASpeex looks normal but simplify with custom IO to avoid worrying about blocks.
             * First block samples count frames' samples subtracting encoder delay. */

            vgmstream->codec_data = init_speex_ea(eaac->channels);
            if (!vgmstream->codec_data) goto fail;
            vgmstream->coding_type = coding_SPEEX;
            vgmstream->layout_type = layout_none;

            temp_sf = setup_eaac_audio_streamfile(sf, eaac->version, eaac->codec, eaac->streamed,0,0, 0x00, 0);
            if (!temp_sf) goto fail;

            break;
        }
#endif

#ifdef VGM_USE_ATRAC9
        case EAAC_CODEC_EATRAX: { /* EATrax (unknown FourCC) [Need for Speed: Most Wanted (Vita)] */
            atrac9_config cfg = {0};

            /* EATrax is "buffered" ATRAC9, uses custom IO since it's kind of complex to add to the decoder */

#if 0
            /* For looped EATRAX, since we are using a deblocker SF no need to make segmented looping, though it works [Madden NFL 13 Demo (Vita)]
             * An issue with segmented is that AT9 state is probably not reset between loops, which segmented can't simulate */
            if (eaac->loop_start > 0) {
                segmented_layout_data *data = build_segmented_eaaudiocore_looping(sf_head, sf, eaac);
                if (!data) goto fail;
                vgmstream->layout_data = data;
                vgmstream->coding_type = data->segments[0]->coding_type;
                vgmstream->layout_type = layout_segmented;
            }
#endif

            cfg.channels = eaac->channels;
            /* sub-header after normal header */
            cfg.config_data = read_u32be(header_offset + eaac->header_size + 0x00,sf_head);
            /* 0x04: data size without blocks, LE b/c why make sense (but don't use it in case of truncated files) */
            /* 0x08: 16b frame size (same as config data) */

            vgmstream->codec_data = init_atrac9(&cfg);
            if (!vgmstream->codec_data) goto fail;
            vgmstream->coding_type = coding_ATRAC9;
            vgmstream->layout_type = layout_none;

            temp_sf = setup_eaac_audio_streamfile(sf, eaac->version, eaac->codec, eaac->streamed,0,0, 0x00, 0);
            if (!temp_sf) goto fail;

            break;
        }
#endif

#ifdef VGM_USE_MPEG
        case EAAC_CODEC_EAMP3: { /* "EM30": EA-MP3 [Need for Speed 2015 (PS4), FIFA 2021 (PC)] */
            mpeg_custom_config cfg = {0};

            temp_sf = setup_eaac_audio_streamfile(sf, eaac->version, eaac->codec, eaac->streamed,0,0, 0x00, 0);
            if (!temp_sf) goto fail;

            vgmstream->codec_data = init_mpeg_custom(temp_sf, 0x00, &vgmstream->coding_type, vgmstream->channels, MPEG_EAMP3, &cfg);
            if (!vgmstream->codec_data) goto fail;
            vgmstream->layout_type = layout_none;

            break;
        }
#endif

#ifdef VGM_USE_FFMPEG
        case EAAC_CODEC_EAOPUS: { /* "Eop0": EAOpus [FIFA 17 (PC), FIFA 19 (Switch)]*/
            vgmstream->layout_data = build_layered_eaaudiocore(sf, eaac, 0x00);
            if (!vgmstream->layout_data) goto fail;
            vgmstream->coding_type = coding_FFmpeg;
            vgmstream->layout_type = layout_layered;
            break;
        }
#endif

#ifdef VGM_USE_FFMPEG
        case EAAC_CODEC_EAOPUSM: /* "MSO0": Multi-Stream Opus [FIFA 2021 (PC)] */
        case EAAC_CODEC_EAOPUSMU: { /* "MSU0": Multi-Stream Opus Uncoupled [FIFA 2022 (PC)] */
            off_t offset = 0x00; // eaac->stream_offset;
            off_t data_size = get_streamfile_size(sf);
            opus_config cfg = {0};

            cfg.channels = eaac->channels;
            {
                uint32_t block_size = read_u32be(offset + 0x00, sf) & 0x00FFFFFF;
                uint32_t curr_samples = read_u32be(offset + 0x04, sf);
                uint32_t next_samples = read_u32be(offset + block_size + 0x04, sf);

                cfg.skip = next_samples - curr_samples;
                /* maybe should check if next block exists, but files of single packet? */
            }

            /* find coupled OPUS streams (internal streams using 2ch) */
            if (eaac->codec == EAAC_CODEC_EAOPUSMU) {
                cfg.coupled_count = 0;
            }
            else {
                switch(eaac->channels) {
                  //case 8:  cfg.coupled_count = 3; break;   /* 2ch+2ch+2ch+1ch+1ch, 5 streams */
                  //case 6:  cfg.coupled_count = 2; break;   /* 2ch+2ch+1ch+1ch, 4 streams */
                    case 4:  cfg.coupled_count = 2; break;   /* 2ch+2ch, 2 streams */
                    case 2:  cfg.coupled_count = 1; break;   /* 2ch, 1 stream */
                    case 1:  cfg.coupled_count = 0; break;   /* 1ch, 1 stream [Madden 22 (PC)] */
                    default: goto fail;                      /* possibly: streams = Nch / 2, coupled = Nch % 2 */
                }
            }

            /* total number internal OPUS streams (should be >0) */
            cfg.stream_count = cfg.channels - cfg.coupled_count;

            /* We *don't* remove EA blocks b/c in Multi Opus 1 block = 1 Opus packet
             * Regular EAOPUS uses layers to fake multichannel, this is normal multichannel Opus.
             * This can be used for stereo too, so probably replaces EAOPUS. */
            //temp_sf = setup_eaac_audio_streamfile(sf_data, eaac->version, eaac->codec, eaac->streamed,0,0, 0x00, 0);
            //if (!temp_sf) goto fail;

            vgmstream->codec_data = init_ffmpeg_ea_opusm(sf, offset, data_size, &cfg);
            if (!vgmstream->codec_data) goto fail;
            vgmstream->coding_type = coding_FFmpeg;
            vgmstream->layout_type = layout_none;
            break;
        }
#endif

        case EAAC_CODEC_EAATRAC9: /* "AT90" (possibly ATRAC9 with a saner layout than EATRAX) */
        default:
            VGM_LOG("EA EAAC: unknown codec 0x%02x\n", eaac->codec);
            goto fail;
    }

    if (!vgmstream_open_stream(vgmstream, temp_sf ? temp_sf : sf, 0x00))
        goto fail;

    close_streamfile(sf);
    close_streamfile(sf_sns);
    close_streamfile(temp_sf);
    return vgmstream;

fail:
    close_streamfile(sf);
    close_streamfile(sf_sns);
    close_streamfile(temp_sf);
    close_vgmstream(vgmstream);
    return NULL;
}

static size_t calculate_eaac_size(STREAMFILE* sf, eaac_header_t* ea, uint32_t num_samples, off_t start_offset, int is_ram) {
    uint32_t block_size;
    uint8_t block_id;
    size_t stream_size, file_size;
    off_t block_offset;
    int looped;

    file_size = get_streamfile_size(sf);
    block_offset = start_offset;
    stream_size = 0;
    looped = 0;

    while (block_offset < file_size) {
        block_id = read_u8(block_offset, sf);
        block_size = read_u32be(block_offset, sf) & 0x00FFFFFF;
        if (block_size == 0 || block_size == 0x00FFFFFF)
            goto fail;

        /* stop when we reach the end marker */
        if (ea->version == EAAC_VERSION_V0) {
            if (block_id != EAAC_BLOCKID0_DATA && block_id != EAAC_BLOCKID0_END)
                goto fail;
        } else {
            if (block_id == EAAC_BLOCKID1_END)
                break;

            if (block_id != EAAC_BLOCKID1_DATA)
                goto fail;
        }

        stream_size += block_size;
        block_offset += block_size;

        if (ea->version == EAAC_VERSION_V0) {
            if (is_ram) {
                /* RAM data only consists of one block (two for looped sounds) */
                if (ea->loop_start > 0 && ea->loop_start < num_samples && !looped) looped = 1;
                else break;
            }
            else if (block_id == EAAC_BLOCKID0_END) {
                if (ea->loop_offset > 0 && ea->loop_start >= ea->prefetch_samples && !looped) looped = 1;
                else break;
            }
        }
    }

    return stream_size;

fail:
    return 0;
}


static STREAMFILE* setup_eaac_streamfile(eaac_header_t* ea, STREAMFILE* sf_head, STREAMFILE* sf_data) {
    size_t data_size;
    STREAMFILE *new_sf = NULL;
    STREAMFILE *temp_sf = NULL;
    STREAMFILE *sf_segments[2] = { 0 };

    if (ea->version == EAAC_VERSION_V0) {
        switch (ea->type) {
            case EAAC_TYPE_RAM:
                /* both header and data in SNR */
                data_size = calculate_eaac_size(sf_head, ea, ea->num_samples, ea->stream_offset, 1);
                if (data_size == 0) goto fail;

                new_sf = open_wrap_streamfile(sf_head);
                if (!new_sf) goto fail;
                temp_sf = new_sf;

                new_sf = open_clamp_streamfile(temp_sf, ea->stream_offset, data_size);
                if (!new_sf) goto fail;
                temp_sf = new_sf;
                break;

            case EAAC_TYPE_STREAM:
                /* header in SNR, data in SNS */
                data_size = calculate_eaac_size(sf_data, ea, ea->num_samples, ea->stream_offset, 0);
                if (data_size == 0) goto fail;

                new_sf = open_wrap_streamfile(sf_data);
                if (!new_sf) goto fail;
                temp_sf = new_sf;

                new_sf = open_clamp_streamfile(temp_sf, ea->stream_offset, data_size);
                if (!new_sf) goto fail;
                temp_sf = new_sf;
                break;

            case EAAC_TYPE_GIGASAMPLE:
                /* header and prefetched data in SNR, rest of data in SNS */
                /* open prefetched data */
                data_size = calculate_eaac_size(sf_head, ea, ea->prefetch_samples, ea->prefetch_offset, 1);
                if (data_size == 0) goto fail;

                new_sf = open_wrap_streamfile(sf_head);
                if (!new_sf) goto fail;
                sf_segments[0] = new_sf;

                new_sf = open_clamp_streamfile(sf_segments[0], ea->prefetch_offset, data_size);
                if (!new_sf) goto fail;
                sf_segments[0] = new_sf;

                /* open main data */
                data_size = calculate_eaac_size(sf_data, ea, ea->num_samples - ea->prefetch_samples, ea->stream_offset, 0);
                if (data_size == 0) goto fail;

                new_sf = open_wrap_streamfile(sf_data);
                if (!new_sf) goto fail;
                sf_segments[1] = new_sf;

                new_sf = open_clamp_streamfile(sf_segments[1], ea->stream_offset, data_size);
                if (!new_sf) goto fail;
                sf_segments[1] = new_sf;

                new_sf = open_multifile_streamfile(sf_segments, 2);
                if (!new_sf) goto fail;
                temp_sf = new_sf;
                sf_segments[0] = NULL;
                sf_segments[1] = NULL;
                break;

            default:
                goto fail;
        }
    }
    else {
        data_size = calculate_eaac_size(sf_head, ea, ea->num_samples, ea->stream_offset, 0);
        if (data_size == 0) goto fail;

        new_sf = open_wrap_streamfile(sf_head);
        if (!new_sf) goto fail;
        temp_sf = new_sf;

        new_sf = open_clamp_streamfile(temp_sf, ea->stream_offset, data_size);
        if (!new_sf) goto fail;
        temp_sf = new_sf;
    }

    return temp_sf;
fail:
    close_streamfile(sf_segments[0]);
    close_streamfile(sf_segments[1]);
    close_streamfile(temp_sf);

    return NULL;
}

/* Actual looping uses 2 block sections, separated by a block end flag *and* padded.
 *
 * We use the segmented layout, since the eaac_streamfile doesn't handle padding,
 * and the segments seem fully separate (so even skipping would probably decode wrong). */
// todo reorganize code for more standard init
static segmented_layout_data* build_segmented_eaaudiocore_looping(STREAMFILE* sf_head, STREAMFILE* sf_data, eaac_header_t* eaac) {
    segmented_layout_data *data = NULL;
    STREAMFILE* temp_sf = NULL;
    uint32_t data_size = get_streamfile_size(sf_data);
    off_t offsets[2] = { 0x00, eaac->loop_offset };
    uint32_t sizes[2] = { eaac->loop_offset, data_size - eaac->loop_offset };
    off_t start_offset;
    int num_samples[2] = { eaac->loop_start, eaac->num_samples - eaac->loop_start};
    int segment_count = 2; /* intro/loop */
    int i;


    /* init layout */
    data = init_layout_segmented(segment_count);
    if (!data) goto fail;

    for (i = 0; i < segment_count; i++) {
        VGMSTREAM* vgmstream = allocate_vgmstream(eaac->channels, 0);

        if (!vgmstream) goto fail;
        data->segments[i] = vgmstream;

        data->segments[i]->sample_rate = eaac->sample_rate;
        data->segments[i]->num_samples = num_samples[i];
        //data->segments[i]->meta_type = eaac->meta_type; /* bleh */

        switch(eaac->codec) {
#ifdef VGM_USE_FFMPEG
            case EAAC_CODEC_EAXMA: {
                eaac_header_t temp_eaac = *eaac; /* equivalent to memcpy... I think */
                temp_eaac.loop_flag = 0;
                temp_eaac.num_samples = num_samples[i];

                start_offset = 0x00; /* must point to the custom streamfile's beginning */

                /* layers inside segments, how trippy */
                data->segments[i]->layout_data = build_layered_eaaudiocore(sf_data, &temp_eaac, offsets[i]);
                if (!data->segments[i]->layout_data) goto fail;
                data->segments[i]->coding_type = coding_FFmpeg;
                data->segments[i]->layout_type = layout_layered;
                break;
            }
#endif

            case EAAC_CODEC_XAS1: {
                start_offset = offsets[i];

                data->segments[i]->coding_type = coding_EA_XAS_V1;
                data->segments[i]->layout_type = layout_blocked_ea_sns;
                break;
            }

#ifdef VGM_USE_MPEG
            case EAAC_CODEC_EALAYER3_V1:
            case EAAC_CODEC_EALAYER3_V2_PCM:
            case EAAC_CODEC_EALAYER3_V2_SPIKE: {
                mpeg_custom_config cfg = {0};
                mpeg_custom_t type = (eaac->codec == 0x05 ? MPEG_EAL31b : (eaac->codec == 0x06) ? MPEG_EAL32P : MPEG_EAL32S);

                start_offset = 0x00; /* must point to the custom streamfile's beginning */

                temp_sf = setup_eaac_audio_streamfile(sf_data, eaac->version,eaac->codec,eaac->streamed,0,0, offsets[i], sizes[i]);
                if (!temp_sf) goto fail;

                data->segments[i]->codec_data = init_mpeg_custom(temp_sf, 0x00, &data->segments[i]->coding_type, eaac->channels, type, &cfg);
                if (!data->segments[i]->codec_data) goto fail;
                data->segments[i]->layout_type = layout_none;
                break;
            }
#endif
#ifdef VGM_USE_ATRAC9
            case EAAC_CODEC_EATRAX: { /* EATrax (unknown FourCC) [Need for Speed: Most Wanted (Vita)] */
                atrac9_config cfg = {0};

                /* EATrax is "buffered" ATRAC9, uses custom IO since it's kind of complex to add to the decoder */

                cfg.channels = eaac->channels;
                /* sub-header after normal header */
                cfg.config_data = read_u32be(0x14 + 0x00, sf_head); //todo pass header offset
                /* 0x04: data size without blocks, LE b/c why make sense (but don't use it in case of truncated files) */
                /* 0x08: 16b frame size (same as config data) */

                vgmstream->codec_data = init_atrac9(&cfg);
                if (!vgmstream->codec_data) goto fail;
                vgmstream->coding_type = coding_ATRAC9;
                vgmstream->layout_type = layout_none;

                start_offset = 0x00; /* must point to the custom streamfile's beginning */

                //todo should make sizes
                temp_sf = setup_eaac_audio_streamfile(sf_data, eaac->version, eaac->codec, eaac->streamed,0,0, offsets[i], sizes[i]);
                if (!temp_sf) goto fail;

                break;
            }
#endif
            default:
                goto fail;
        }

        if (!vgmstream_open_stream(data->segments[i], temp_sf == NULL ? sf_data : temp_sf, start_offset))
            goto fail;

        close_streamfile(temp_sf);
        temp_sf = NULL;
    }

    if (!setup_layout_segmented(data))
        goto fail;
    return data;

fail:
    close_streamfile(temp_sf);
    free_layout_segmented(data);
    return NULL;
}

static layered_layout_data* build_layered_eaaudiocore(STREAMFILE* sf_data, eaac_header_t* eaac, off_t start_offset) {
    layered_layout_data* data = NULL;
    STREAMFILE* temp_sf = NULL;
    int i, layers = (eaac->channels+1) / 2;


    /* init layout */
    data = init_layout_layered(layers);
    if (!data) goto fail;

    /* open each layer subfile (1/2ch streams: 2ch+2ch..+1ch or 2ch+2ch..+2ch). */
    for (i = 0; i < layers; i++) {
        int layer_channels = (i+1 == layers && eaac->channels % 2 == 1) ? 1 : 2; /* last layer can be 1/2ch */

        /* build the layer VGMSTREAM */
        data->layers[i] = allocate_vgmstream(layer_channels, eaac->loop_flag);
        if (!data->layers[i]) goto fail;

        data->layers[i]->sample_rate = eaac->sample_rate;
        data->layers[i]->num_samples = eaac->num_samples;
        data->layers[i]->loop_start_sample = eaac->loop_start;
        data->layers[i]->loop_end_sample = eaac->loop_end;

        switch(eaac->codec) {
#ifdef VGM_USE_FFMPEG
            /* EA-XMA uses completely separate 1/2ch streams, unlike standard XMA that interleaves 1/2ch
             * streams with a skip counter to reinterleave (so EA-XMA streams don't have skips set) */
            case EAAC_CODEC_EAXMA: {
                int block_size;
                size_t stream_size;
                int is_xma1;

                temp_sf = setup_eaac_audio_streamfile(sf_data, eaac->version, eaac->codec, eaac->streamed,i,layers, start_offset, 0);
                if (!temp_sf) goto fail;

                stream_size = get_streamfile_size(temp_sf);
                block_size = 0x10000;

                /* EA adopted XMA2 when it appeared around 2006, but detection isn't so easy
                 * (SNS with XMA2 do exist). Decoder should work when playing XMA1 as XMA2, but
                 * the other way around can cause issues, so it's safer to just use XMA2. */
                is_xma1 = 0; //eaac->version == EAAC_VERSION_V0; /* approximate */
                if (is_xma1) {
                    data->layers[i]->codec_data = init_ffmpeg_xma1_raw(temp_sf, 0x00, stream_size, data->layers[i]->channels, data->layers[i]->sample_rate, 0);
                }
                else {
                    data->layers[i]->codec_data = init_ffmpeg_xma2_raw(temp_sf, 0x00, stream_size, data->layers[i]->num_samples, data->layers[i]->channels, data->layers[i]->sample_rate, block_size, 0);
                }
                if (!data->layers[i]->codec_data) goto fail;

                data->layers[i]->coding_type = coding_FFmpeg;
                data->layers[i]->layout_type = layout_none;
                data->layers[i]->stream_size = get_streamfile_size(temp_sf);

                xma_fix_raw_samples(data->layers[i], temp_sf, 0x00,stream_size, 0, 0,0); /* samples are ok? */
                break;
            }

            /* Opus can do multichannel just fine, but that wasn't weird enough for EA */
            case EAAC_CODEC_EAOPUS: {
                int skip;
                size_t data_size;

                /* We'll remove EA blocks and pass raw data to FFmpeg Opus decoder */
                temp_sf = setup_eaac_audio_streamfile(sf_data, eaac->version, eaac->codec, eaac->streamed,i,layers, start_offset, 0);
                if (!temp_sf) goto fail;

                skip = ea_opus_get_encoder_delay(0x00, temp_sf);
                data_size = get_streamfile_size(temp_sf);

                data->layers[i]->codec_data = init_ffmpeg_ea_opus(temp_sf, 0x00,data_size, layer_channels, skip, eaac->sample_rate);
                if (!data->layers[i]->codec_data) goto fail;
                data->layers[i]->coding_type = coding_FFmpeg;
                data->layers[i]->layout_type = layout_none;
                break;
            }
#endif
            default:
                goto fail;
        }

        if (!vgmstream_open_stream(data->layers[i], temp_sf, 0x00))
            goto fail;

        close_streamfile(temp_sf);
        temp_sf = NULL;
    }

    if (!setup_layout_layered(data))
        goto fail;
    return data;

fail:
    close_streamfile(temp_sf);
    free_layout_layered(data);
    return NULL;
}
