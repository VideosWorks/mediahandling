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

#include <gtest/gtest.h>

#include "ffmpegstream.h"
#include "ffmpegsource.h"

using namespace media_handling;
using namespace media_handling::ffmpeg;


TEST (FFMpegStreamTest, NullInst)
{
  AVFormatContext ctx;
  AVStream strm;
  EXPECT_THROW(FFMpegStream(nullptr, nullptr), std::exception);
  EXPECT_THROW(FFMpegStream(&ctx, nullptr), std::exception);
  EXPECT_THROW(FFMpegStream(nullptr, &strm), std::exception);
}

TEST (FFMpegStreamTest, Openh264FHDVisualStream)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  ASSERT_TRUE(src->visualStream(0));
  ASSERT_TRUE(src->visualStream(0)->type() == media_handling::StreamType::VISUAL);
  ASSERT_FALSE(src->visualStream(1));
}

class VisualStreamPixelFormatParameterTests : public testing::TestWithParam<std::tuple<std::string, media_handling::PixelFormat>>
{
  public:
    std::unique_ptr<FFMpegSource> source_;
};

TEST_P (VisualStreamPixelFormatParameterTests, CheckEqual)
{
  auto [path, format] = this->GetParam();
  source_ = std::make_unique<FFMpegSource>(path);
  auto stream = source_->visualStream(0);
  bool is_valid;
  auto prop_fmt = stream->property<PixelFormat>(MediaProperty::PIXEL_FORMAT, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(prop_fmt, format);
}

INSTANTIATE_TEST_CASE_P(
      FFMpegStreamTest,
      VisualStreamPixelFormatParameterTests,
      testing::Values(std::make_tuple("./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4", PixelFormat::YUV420),
                      std::make_tuple("./ReferenceMedia/Video/mpeg2/interlaced_avc.MTS", PixelFormat::YUV420),
                      std::make_tuple("./ReferenceMedia/Video/dnxhd/fhd_dnxhd.mov", PixelFormat::YUV422)
));


class DimensionsParameterTests : public testing::TestWithParam<std::tuple<std::string, Dimensions>>
{
  public:
    std::unique_ptr<FFMpegSource> source_;
};


TEST_P (DimensionsParameterTests, CheckEqual)
{
  auto [path, dims] = this->GetParam();
  source_ = std::make_unique<FFMpegSource>(path);
  auto stream = source_->visualStream(0);
  bool is_valid;
  auto prop_dims = stream->property<Dimensions>(MediaProperty::DIMENSIONS, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(prop_dims.width, dims.width);
  ASSERT_EQ(prop_dims.height, dims.height);
}

INSTANTIATE_TEST_CASE_P(
      FFMpegStreamTest,
      DimensionsParameterTests,
      testing::Values(std::make_tuple("./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4", Dimensions{1920,1080}),
                      std::make_tuple("./ReferenceMedia/Video/mpeg2/interlaced_avc.MTS", Dimensions{1920,1080}),
                      std::make_tuple("./ReferenceMedia/Video/dnxhd/fhd_dnxhd.mov", Dimensions{1920,1080})
));

class PARParameterTests : public testing::TestWithParam<std::tuple<std::string, Rational>>
{
  public:
    std::unique_ptr<FFMpegSource> source_;
};


TEST_P (PARParameterTests, CheckEqual)
{
  auto [path, par] = this->GetParam();
  source_ = std::make_unique<FFMpegSource>(path);
  auto stream = source_->visualStream(0);
  bool is_valid;
  auto prop_par = stream->property<Rational>(MediaProperty::PIXEL_ASPECT_RATIO, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(prop_par, par);
}

INSTANTIATE_TEST_CASE_P(
      FFMpegStreamTest,
      PARParameterTests,
      testing::Values(std::make_tuple("./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4", Rational{1,1}),
                      std::make_tuple("./ReferenceMedia/Video/mpeg2/interlaced_avc.MTS", Rational{1,1}),
                      std::make_tuple("./ReferenceMedia/Video/dnxhd/fhd_dnxhd.mov", Rational{1,1})
));


TEST (FFMpegStreamTest, Openh264FHDVisualStreamDAR)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  bool is_valid;
  auto dar = stream->property<media_handling::Rational>(media_handling::MediaProperty::DISPLAY_ASPECT_RATIO, is_valid);
  ASSERT_FALSE(is_valid);
}

TEST (FFMpegStreamTest, Openh264FHDVisualStreamFrameCount)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  bool is_valid;
  auto frames = stream->property<int64_t>(media_handling::MediaProperty::FRAME_COUNT, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_TRUE(frames == 748);
}

TEST (FFMpegStreamTest, Openh264FHDVisualStreamTimescale)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  bool is_valid;
  auto tscale = stream->property<Rational>(media_handling::MediaProperty::TIMESCALE, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_TRUE(tscale == Rational(1,12800));
}

class FieldOrderParameterTests : public testing::TestWithParam<std::tuple<std::string, FieldOrder>>
{
  public:
    std::unique_ptr<FFMpegSource> source_;
};

TEST_P (FieldOrderParameterTests, CheckIsEqual)
{
  auto [path, order] = this->GetParam();
  source_ = std::make_unique<FFMpegSource>(path);
  auto stream = source_->visualStream(0);
  bool is_valid;
  auto prop_order = stream->property<FieldOrder>(MediaProperty::FIELD_ORDER, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(prop_order, order);
}

INSTANTIATE_TEST_CASE_P(
      FFMpegStreamTest,
      FieldOrderParameterTests,
      testing::Values(std::make_tuple("./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4", FieldOrder::PROGRESSIVE),
                      std::make_tuple("./ReferenceMedia/Video/mpeg2/interlaced_avc.MTS", FieldOrder::TOP_FIRST),
                      std::make_tuple("./ReferenceMedia/Video/dnxhd/fhd_dnxhd.mov", FieldOrder::PROGRESSIVE)
));



TEST (FFMpegStreamTest, Openh264FHDVisualStreamReadFrame)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  auto frame = stream->frame(0);
  ASSERT_TRUE(frame != nullptr);
  ASSERT_TRUE(frame->size() > 0);
  ASSERT_EQ(frame->timestamp(), 0);
}

TEST (FFMpegStreamTest, Openh264FHDVisualStreamReadTenthFrame)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  bool is_valid;
  auto t_base = stream->property<Rational>(MediaProperty::TIMESCALE, is_valid);
  auto pos = int64_t(std::round(10 / boost::rational_cast<double>(t_base)));
  auto frame = stream->frame(pos);
  ASSERT_TRUE(frame != nullptr);
  ASSERT_EQ(frame->timestamp(), pos);
}

TEST (FFMpegStreamTest, Openh264FHDVisualStreamReadNextFrame)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  MediaFramePtr frame = stream->frame(0);
  frame = stream->frame();
  ASSERT_EQ(frame->timestamp(), 256); // timebase/fps
}

TEST (FFMpegStreamTest, Openh264FHDVisualStreamReadToEOS)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->visualStream(0);
  MediaFramePtr frame = stream->frame(0);
  int64_t timestamp = -1;
  int frames = 0;
  while (frame) {
    frame = stream->frame();
    if (frame) {
      frames++;
      timestamp = frame->timestamp();
    }
  }

  ASSERT_TRUE(frames > 0);
  ASSERT_TRUE(timestamp > 0);
}


TEST (FFMpegStreamTest, Openh264FHDAudioStream)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  ASSERT_TRUE(src->audioStream(0));
  ASSERT_TRUE(src->audioStream(0)->type() == media_handling::StreamType::AUDIO);
  ASSERT_FALSE(src->audioStream(1));
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamChannels)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  bool is_valid;
  auto channels = stream->property<int32_t>(media_handling::MediaProperty::AUDIO_CHANNELS, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(channels, 2);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamSamplingRate)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  bool is_valid;
  auto sample_rate = stream->property<int32_t>(media_handling::MediaProperty::AUDIO_SAMPLING_RATE, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(sample_rate, 48000);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamSamplingFormat)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  bool is_valid;
  auto sample_format = stream->property<media_handling::SampleFormat>(media_handling::MediaProperty::AUDIO_FORMAT, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(sample_format, media_handling::SampleFormat::FLOAT_P);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamReadFrame)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  auto frame = stream->frame(0);
  ASSERT_TRUE(frame != nullptr);
  ASSERT_TRUE(frame->size() > 0);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamReadFrameProperties)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  auto frame = stream->frame(0);
  frame->extractProperties();
  bool is_valid;
  auto format = frame->property<media_handling::SampleFormat>(media_handling::MediaProperty::AUDIO_FORMAT, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(format, media_handling::SampleFormat::FLOAT_P);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamChannelLayout)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  bool is_valid;
  auto format = stream->property<media_handling::ChannelLayout>(media_handling::MediaProperty::AUDIO_LAYOUT, is_valid);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(format, ChannelLayout::STEREO);
}

TEST (FFMpegStreamTest, Openh264FHDAudioStreamReadFrameToEOS)
{
  std::string fname = "./ReferenceMedia/Video/h264/h264_yuv420p_avc1_fhd.mp4";
  media_handling::MediaSourcePtr src = std::make_shared<FFMpegSource>(fname);
  auto stream = src->audioStream(0);
  MediaFramePtr frame = stream->frame(0);
  int64_t timestamp = -1;
  int frames = 0;
  while (frame) {
    frame = stream->frame();
    if (frame) {
      frames++;
      timestamp = frame->timestamp();
    }
  }

  ASSERT_TRUE(frames > 0);
  ASSERT_TRUE(timestamp > 0);
}

