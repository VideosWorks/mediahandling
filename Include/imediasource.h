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

#ifndef IMEDIASOURCE_H
#define IMEDIASOURCE_H

#include <stdint.h>
#include <map>
#include <any>

#include "types.h"
#include "imediastream.h"

namespace media_handling
{

  /**
   * @brief The "Container"
   */
  class IMediaSource : public IMediaPropertyObject
  {
    public:
      virtual ~IMediaSource() override {}

      /**
       * @brief     Initialise the Media source
       * @note      Typically, this should not be needed if the implementing class employs RAII. It's more of a
       *            workaround for unit-testing
       * @return    true==success
       */
      virtual bool initialise() = 0;

      /**
       * @brief             Set the file path of this source
       * @param file_path   Local path of the source-file
       */
      virtual void setFilePath(const std::string& file_path) = 0;

      /**
       * @brief         Retrieve an audio stream
       * @param index   Index from the available audio-streams (not index of all streams)
       * @return        Stream on success or nullptr
       */
      virtual IMediaStreamPtr audioStream(const int index) = 0;

      /**
       * @brief         Obtain a visual (video/image) stream
       * @param index   Index from the available visual-streams (not index of all streams)
       * @return        Stream on success or nullptr
       */
      virtual IMediaStreamPtr visualStream(const int index) = 0;

      /* IMediaPropertyObject */
      virtual void setProperties(const std::map<MediaProperty, std::any>& props) override = 0;
      virtual void setProperty(const MediaProperty prop, std::any value) override = 0;
      virtual std::any property(const MediaProperty prop, bool& is_valid) const override = 0;
      template <typename T>
      T property(const MediaProperty prop, bool& is_valid) const
      {
        return IMediaPropertyObject::property<T>(prop, is_valid);
      }
  };
}

#endif // IMEDIASOURCE_H
