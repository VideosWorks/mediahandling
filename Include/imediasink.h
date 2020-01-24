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

#ifndef IMEDIASINK_H
#define IMEDIASINK_H

#include <any>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <set>

#include "types.h"
#include "imediastream.h"

namespace media_handling
{
    class IMediaSink : public MediaPropertyObject
    {
    public:
        ~IMediaSink() override = default;

        virtual bool initialise() = 0;

        virtual void finish() = 0;
        /**
         * @brief     Identify if sink is correctly setup for encoding
         * @return    true==is ready
         */
        virtual bool isReady() = 0;
        /**
         * @brief         Retrieve an audio stream
         * @param index   Index from the available audio-streams (not index of all streams)
         * @return        Stream on success or nullptr
         */
        virtual MediaStreamPtr audioStream(const size_t index) = 0;

        /**
         * @brief   Get all the audio streams
         * @return  list of audio streams
         */
        virtual std::vector<MediaStreamPtr> audioStreams() = 0;

        /**
         * @brief         Obtain a visual (video/image) stream
         * @param index   Index from the available visual-streams (not index of all streams)
         * @return        Stream on success or nullptr
         */
        virtual MediaStreamPtr visualStream(const size_t index) = 0;

        /**
         * @brief   Get all the visual streams
         * @return  list of visual streams
         */
        virtual std::vector<MediaStreamPtr> visualStreams() = 0;

        virtual std::set<Codec> supportedAudioCodecs() const = 0;

        virtual std::set<Codec> supportedVideoCodecs() const = 0;
    };

    using MediaSinkPtr = std::shared_ptr<IMediaSink>;
}

#endif // IMEDIASINK_H
