/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <string>
#include <vector>

#include <boost/detail/lightweight_test.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_directive.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <string>
#include <iostream>
#include "test.hpp"


int
main()
{
    using spirit_test::test_attr;
    using spirit_test::test;

    using namespace boost::spirit::ascii;
    using boost::spirit::qi::repeat;
    using boost::spirit::qi::inf;
    using boost::spirit::qi::omit;
    using boost::spirit::qi::int_;
    using boost::spirit::qi::_1;

    {
        BOOST_TEST(test("aaaaaaaa", repeat[char_])); // kleene synonym
        BOOST_TEST(test("aaaaaaaa", repeat(8)[char_]));
        BOOST_TEST(!test("aa", repeat(3)[char_]));

        BOOST_TEST(test("aaa", repeat(3, 5)[char_]));
        BOOST_TEST(test("aaaaa", repeat(3, 5)[char_]));
        BOOST_TEST(!test("aaaaaa", repeat(3, 5)[char_]));
        BOOST_TEST(!test("aa", repeat(3, 5)[char_]));

        BOOST_TEST(test("aaa", repeat(3, inf)[char_]));
        BOOST_TEST(test("aaaaa", repeat(3, inf)[char_]));
        BOOST_TEST(test("aaaaaa", repeat(3, inf)[char_]));
        BOOST_TEST(!test("aa", repeat(3, inf)[char_]));
    }

    { // from classic spirit tests
        BOOST_TEST(test("", repeat(0, inf)['x']));

        //  repeat exact 8
        #define rep8 repeat(8)[alpha] >> 'X'
        BOOST_TEST(!test("abcdefgX", rep8, false));
        BOOST_TEST(test("abcdefghX", rep8));
        BOOST_TEST(!test("abcdefghiX", rep8, false));
        BOOST_TEST(!test("abcdefgX", rep8, false));
        BOOST_TEST(!test("aX", rep8, false));

        //  repeat 2 to 8
        #define rep28 repeat(2, 8)[alpha] >> '*'
        BOOST_TEST(test("abcdefg*", rep28));
        BOOST_TEST(test("abcdefgh*", rep28));
        BOOST_TEST(!test("abcdefghi*", rep28, false));
        BOOST_TEST(!test("a*", rep28, false));

        //  repeat 2 or more
        #define rep2_ repeat(2, inf)[alpha] >> '+'
        BOOST_TEST(test("abcdefg+", rep2_));
        BOOST_TEST(test("abcdefgh+", rep2_));
        BOOST_TEST(test("abcdefghi+", rep2_));
        BOOST_TEST(test("abcdefg+", rep2_));
        BOOST_TEST(!test("a+", rep2_, false));

        //  repeat 0
        #define rep0 repeat(0)[alpha] >> '/'
        BOOST_TEST(test("/", rep0));
        BOOST_TEST(!test("a/", rep0, false));

        //  repeat 0 or 1
        #define rep01 repeat(0, 1)[alpha >> digit] >> '?'
        BOOST_TEST(!test("abcdefg?", rep01, false));
        BOOST_TEST(!test("a?", rep01, false));
        BOOST_TEST(!test("1?", rep01, false));
        BOOST_TEST(!test("11?", rep01, false));
        BOOST_TEST(!test("aa?", rep01, false));
        BOOST_TEST(test("?", rep01));
        BOOST_TEST(test("a1?", rep01));
    }

    {
        BOOST_TEST(test(" a a aaa aa", repeat(7)[char_], space));
        BOOST_TEST(test("12345 678 9", repeat(9)[digit], space));
    }

    {
        BOOST_TEST(test("aBcdeFGH", no_case[repeat(8)[lower]]));
        BOOST_TEST(test("a B cde FGH", no_case[repeat(8)[lower]], space));
    }

    {
        std::string s;
        BOOST_TEST(test_attr("bbbb", repeat(4)[char_], s) && s == "bbbb");

        s.clear();
        BOOST_TEST(test_attr("b b b b", repeat(4)[char_], s, space) && s == "bbbb");

        // The following 4 tests show that omit does not inhibit explicit attributes
        s.clear();
        BOOST_TEST(test_attr("bbbb", repeat(4)[omit[char_('b')]], s) && s == "bbbb");

        s.clear();
        BOOST_TEST(test_attr("bbbb", omit[repeat(4)[char_('b')]], s) && s == "bbbb");

        s.clear();
        BOOST_TEST(test_attr("b b b b", repeat(4)[omit[char_('b')]], s, space) && s == "bbbb");

        s.clear();
        BOOST_TEST(test_attr("b b b b", omit[repeat(4)[char_('b')]], s, space) && s == "bbbb");
    }

    { // actions
        namespace phx = boost::phoenix;

        std::vector<char> v;
        BOOST_TEST(test("bbbb", repeat(4)[char_][phx::ref(v) = _1]) && 4 == v.size() &&
            v[0] == 'b' && v[1] == 'b' && v[2] == 'b' &&  v[3] == 'b');
    }

    { // more actions
        namespace phx = boost::phoenix;

        std::vector<int> v;
        BOOST_TEST(test("123 456 789", repeat(3)[int_][phx::ref(v) = _1], space) && 3 == v.size() &&
            v[0] == 123 && v[1] == 456 && v[2] == 789);
    }

    { // lazy repeats
        using boost::phoenix::val;

        BOOST_TEST(test("aaaaaaaa", repeat(val(8))[char_]));
        BOOST_TEST(!test("aa", repeat(val(3))[char_]));

        BOOST_TEST(test("aaa", repeat(val(3), val(5))[char_]));
        BOOST_TEST(test("aaaaa", repeat(val(3), val(5))[char_]));
        BOOST_TEST(!test("aaaaaa", repeat(val(3), val(5))[char_]));
        BOOST_TEST(!test("aa", repeat(val(3), val(5))[char_]));

        BOOST_TEST(test("aaa", repeat(val(3), val(inf))[char_]));
        BOOST_TEST(test("aaaaa", repeat(val(3), val(inf))[char_]));
        BOOST_TEST(test("aaaaaa", repeat(val(3), val(inf))[char_]));
        BOOST_TEST(!test("aa", repeat(val(3), val(inf))[char_]));
    }

    { // more lazy repeats
        using boost::phoenix::val;

        BOOST_TEST(test("aaa", repeat(3, val(5))[char_]));
        BOOST_TEST(test("aaaaa", repeat(val(3), 5)[char_]));
        BOOST_TEST(!test("aaaaaa", repeat(3, val(5))[char_]));
        BOOST_TEST(!test("aa", repeat(val(3), 5)[char_]));

        BOOST_TEST(test("aaa", repeat(val(3), inf)[char_]));
        BOOST_TEST(test("aaaaa", repeat(3, val(inf))[char_]));
        BOOST_TEST(test("aaaaaa", repeat(val(3), inf)[char_]));
        BOOST_TEST(!test("aa", repeat(3, val(inf))[char_]));
    }

    return boost::report_errors();
}
