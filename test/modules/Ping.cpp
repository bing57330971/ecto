/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ecto/ecto.hpp>
#include <ecto/registry.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using ecto::tendrils;
namespace ecto_test
{
  namespace pt = boost::posix_time;

  struct Ping
  {
    ecto::spore<pt::ptime> out;

    static void declare_params(tendrils& parameters) { }

    static void declare_io(const ecto::tendrils& parameters, ecto::tendrils& inputs, ecto::tendrils& outputs)
    {
      outputs.declare<pt::ptime> ("out", "output");
    }

    void configure(tendrils& parameters, tendrils& inputs, tendrils& outputs) 
    { 
      out = outputs.at("out");
    }

    int process(const ecto::tendrils& inputs, ecto::tendrils& outputs)
    {
      *out = pt::microsec_clock::universal_time();
      return 0;
    }
  };
}

ECTO_MODULE(ecto_test, ecto_test::Ping, "Ping", "Ping as fast as you can, send the current time downstream");
