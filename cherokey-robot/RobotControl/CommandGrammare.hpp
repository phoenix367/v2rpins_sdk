/* 
 * File:   CommandGrammare.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 28, 2014, 11:35 PM
 */

#ifndef COMMANDGRAMMARE_HPP
#define	COMMANDGRAMMARE_HPP

#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

namespace qi    = boost::spirit::qi;
namespace karma = boost::spirit::karma;

struct query_term 
{
    std::string term;
    bool is_tag;
};

BOOST_FUSION_ADAPT_STRUCT(query_term, (std::string,term)(bool,is_tag));

template<typename Iterator> struct query_grammar: qi::grammar<Iterator, 
        std::vector<query_term>(), qi::space_type>
{
    query_grammar() : query_grammar::base_type(query)
    {
        using namespace qi;

        word    = lexeme[ +alnum ];

        tag     = omit[word >> ':'] >> word >> attr(true);

        non_tag = word >> attr(false);

        query   = *(tag | non_tag);

        BOOST_SPIRIT_DEBUG_NODE(word);
        BOOST_SPIRIT_DEBUG_NODE(tag);
        BOOST_SPIRIT_DEBUG_NODE(non_tag);
        BOOST_SPIRIT_DEBUG_NODE(query);
    };

    qi::rule<Iterator, std::string()            , qi::space_type> word;
    qi::rule<Iterator, query_term()             , qi::space_type> tag, non_tag;
    qi::rule<Iterator, std::vector<query_term>(), qi::space_type> query;
};

#endif	/* COMMANDGRAMMARE_HPP */
