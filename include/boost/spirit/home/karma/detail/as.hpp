//  Copyright (c) 2001-2010 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_AS_STRING_DEC_18_0644PM)
#define BOOST_SPIRIT_KARMA_AS_STRING_DEC_18_0644PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/attributes_fwd.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    //  This file contains the attribute to string conversion utility. The 
    //  utility provided also accept spirit's unused_type; all no-ops. Compiler 
    //  optimization will easily strip these away.
    ///////////////////////////////////////////////////////////////////////////

    // This is the default case: the plain attribute values
    template <typename T, typename Attribute, typename Enable/*= void*/>
    struct attribute_as_xxx
    {
        typedef Attribute const& type; 

        static type call(Attribute const& attr)
        {
            return attr;
        }

        static bool is_valid(Attribute const& attr)
        {
            return true;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Attribute>
    inline typename spirit::result_of::attribute_as_xxx<T, Attribute>::type
    as(Attribute const& attr)
    {
        return attribute_as_xxx<T, Attribute>::call(attr);
    }

    template <typename T>
    inline unused_type as(unused_type)
    {
        return unused;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Attribute>
    inline bool valid_as(Attribute const& attr)
    {
        return attribute_as_xxx<T, Attribute>::is_valid(attr);
    }

    template <typename T>
    inline bool valid_as(unused_type)
    {
        return true;
    }
}}}

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace result_of
{
    template <typename T, typename Attribute>
    struct attribute_as_xxx
      : traits::attribute_as_xxx<T, Attribute>
    {};

    template <typename T>
    struct attribute_as_xxx<T, unused_type>
    {
        typedef unused_type type;
    };

    template <typename T>
    struct attribute_as_xxx<T, unused_type const>
    {
        typedef unused_type type;
    };
}}}

#endif