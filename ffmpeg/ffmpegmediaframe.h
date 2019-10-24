/*
  Copyright (c) 2019, Jonathan Noble
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FFMPEGMEDIAFRAME_H
#define FFMPEGMEDIAFRAME_H

#include "imediaframe.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

namespace media_handling
{

  // TODO: move the AV types somewhere else
  void avframeDeleter(AVFrame* frame);
  void swsContextDeleter(SwsContext* context);
  void swrContextDeleter(SwrContext* context);

  template <auto fn>
  using deleter_from_fn = std::integral_constant<decltype(fn), fn>;
  using AVFrameUPtr = std::unique_ptr<AVFrame, deleter_from_fn<avframeDeleter>>;
  using SWSContextUPtr = std::unique_ptr<SwsContext, deleter_from_fn<swsContextDeleter>>;
  using SWSContextPtr = std::shared_ptr<SwsContext>;
  using SWRContextUPtr = std::unique_ptr<SwrContext, deleter_from_fn<swrContextDeleter>>;
  using SWRContextPtr = std::shared_ptr<SwrContext>;

  class FFMpegMediaFrame : public IMediaFrame
  {
    public:
      FFMpegMediaFrame(AVFrameUPtr frame, const bool visual);

      FFMpegMediaFrame(AVFrameUPtr frame, const bool visual, SWSContextPtr converter);

      FFMpegMediaFrame(AVFrameUPtr frame, const bool visual, SWRContextPtr converter);

      ~FFMpegMediaFrame() override;

      // FIXME: rule of five

      std::optional<bool> isAudio() const override;

      std::optional<bool> isVisual() const override;

      std::optional<int64_t> lineSize(const int index) const override;

      uint8_t** data() noexcept override;

      uint8_t** convertedData() noexcept override;

      void extractProperties() override;

      int64_t timestamp() const override;

    private:
      AVFrameUPtr ff_frame_ {nullptr};
      std::optional<bool> is_audio_;
      std::optional<bool> is_visual_;
      uint8_t** data_ {nullptr};
      int64_t timestamp_ {-1};
      SWSContextPtr sws_context_{nullptr};
      SWRContextPtr swr_context_{nullptr};

      void extractVisualProperties();

      void extractAudioProperties();

      /**
       * @brief Convert from FFMpeg type to media_handling type
       * @param format FFMpeg sample format
       * @return SampleFormat
       */
      constexpr SampleFormat convert(enum AVSampleFormat format) const noexcept;

  };
}

#endif // FFMPEGMEDIAFRAME_H
