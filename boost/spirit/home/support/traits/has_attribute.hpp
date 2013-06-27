/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2013 Agust�n Berg�
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_HAS_ATTRIBUTE_JUN_6_2012_1714PM)
#define BOOST_SPIRIT_HAS_ATTRIBUTE_JUN_6_2012_1714PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/traits/attribute_of.hpp>
#include <boost/spirit/home/support/utility/sfinae.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace spirit
{
   struct unused_type;
}}

namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Whether a component has an attribute. By default, this compares the 
    // component attribute against unused_type. If the component provides a
    // nested constant expression has_attribute as a hint, that value is used
    // instead. Components may specialize this.
    ///////////////////////////////////////////////////////////////////////////
    template <typename Component, typename Context, typename Enable = void>
    struct has_attribute : mpl::not_<is_same<unused_type,
        typename attribute_of<Component, Context>::type>> {};
    
    template <typename Component, typename Context>
    struct has_attribute<Component, Context,
        typename disable_if_substitution_failure<
            mpl::bool_<Component::has_attribute>>::type>
      : mpl::bool_<Component::has_attribute> {};
}}}

#endif