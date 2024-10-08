#pragma once

#include <Replay.h>

#include "webixs/PlayerIXS.h"

namespace rePlayer
{
    class ReplayIXalance : public Replay
    {
    public:
        static Replay* Load(io::Stream* stream, CommandBuffer metadata);

    public:
        ~ReplayIXalance() override;

        uint32_t GetSampleRate() const override { return kSampleRate; }
        bool IsSeekable() const override { return false; }

        uint32_t Render(StereoSample* output, uint32_t numSamples) override;

        void ResetPlayback() override;

        void ApplySettings(const CommandBuffer metadata) override;
        void SetSubsong(uint32_t subsongIndex) override;

        uint32_t GetDurationMs() const override;
        uint32_t GetNumSubsongs() const override;
        std::string GetExtraInfo() const override;
        std::string GetInfo() const override;

    private:
        static constexpr uint32_t kSampleRate = 48000;

    private:
        ReplayIXalance(IXS::PlayerIXS* player);

    private:
//        SmartPtr<io::Stream> m_stream;
        IXS::PlayerIXS* m_player;

        int16_t* m_buffer = nullptr;
        uint m_size = 0;

        uint32_t m_lastLoop = 0;
    };
}
// namespace rePlayer