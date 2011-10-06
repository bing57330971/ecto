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
#pragma once
#include <boost/python.hpp>
#include <boost/python/type_id.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <ecto/util.hpp> //name_of
#include <ecto/tendril.hpp>
#include <stdexcept>
#include <string>
#include <set>
#include <sstream>
#include <cstring>

namespace ecto
{
  /**
   * \class spore
   * \brief The spore is a typed handle for tendrils, making holding onto tendrils a bit easier.
   */
  template<typename T>
  struct spore
  {
    typedef spore<T> this_type;
    typedef T value_type;
    typedef T& reference_type;
    typedef T* pointer_type;
    typedef const T* const_pointer_type;
    /**
     * Allocates a spore that doesn't point to anything.
     */
    spore() { }

    /**
     * implicit constructor from a tendril ptr. Needs to be a shared_ptr
     * and uses this to ensure that the spore always points to valid tendril.
     */
    spore(tendril::ptr t) :
        tendril_(t)
    {
      if(!t)
        BOOST_THROW_EXCEPTION(except::NullTendril()
                              << except::diag_msg("creating sport with type")
                              << except::spore_typename(name_of<T>()));

      t->enforce_type<T>();
    }

    /**
     * Set a typed callback, that will be called when ever the tendril value is changed by the
     * user.
     * @param cb The callback
     * @return ref to this spore, for chaining.
     */
    spore<T>& set_callback(typename boost::function<void(T)> cb)
    {
      get()->set_callback(cb);
      return *this;
    }

    spore<T>& notify()
    {
      get()->notify();
      return *this;
    }

    spore<T>& set_doc(const std::string& doc)
    {
      get()->set_doc(doc);
      return *this;
    }

    spore<T>& set_default_val(const T& val)
    {
      get()->set_default_val(val);
      return *this;
    }

    bool dirty() const
    {
      return get()->dirty();
    }

    void dirty(bool d)
    {
      return get()->dirty(d);
    }

    bool user_supplied() const
    {
      return get()->user_supplied();
    }

    bool has_default() const
    {
      return get()->has_default();
    }

    spore<T>& required(bool b)
    {
      get()->required(b);
      return *this;
    }

    bool required() const
    {
      return get()->required();
    }

    pointer_type operator->()
    {
      tendril::ptr _p = get();
      return &(_p->get<T>());
    }

    const_pointer_type operator->() const
    {
      tendril::const_ptr _p = get();
      return &(_p->get<const T>());
    }

    reference_type operator*()
    {
      tendril::ptr _p = get();
      return _p->get<T>();
    }

    const_pointer_type operator*() const
    {
      tendril::const_ptr _p = get();
      return _p->get<const T>();
    }

    typedef tendril::ptr this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
      return tendril_ ? &this_type::tendril_ : 0;
    }

  private:

    /**
     * Grab a pointer to the tendril that this spore points to.
     * @return non const pointer to tendril
     */
    inline tendril::ptr get()
    {
      if (!tendril_)
        BOOST_THROW_EXCEPTION(except::NullTendril());
      return tendril_;
    }

    /**
     * Grab a pointer to the tendril that this spore points to. const overload.
     * @return const pointer to tendril
     */
    inline tendril::const_ptr get() const
    {
      if (!tendril_)
        BOOST_THROW_EXCEPTION(except::NullTendril() 
                              << except::diag_msg("access via spore")
                              << except::spore_typename(name_of<T>()));

      return tendril_;
    }
    boost::shared_ptr<tendril> tendril_;
  };
}
