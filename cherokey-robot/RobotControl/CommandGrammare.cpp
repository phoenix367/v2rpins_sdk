#include "CommandGrammare.hpp"

void tmp()
{
    const std::string input = "apple tag:beer banana grape";
    typedef std::string::const_iterator It;

    query_grammar<It> parser;
    std::vector<query_term> data;

    It f(input.begin()), l(input.end());
    bool ok = qi::phrase_parse(f, l, parser, qi::space, data);

    if (ok)
        std::cout << karma::format(karma::delimit [ karma::auto_ ] % 
                karma::eol, data) << '\n';
    if (f!=l)
        std::cerr << "Unparsed: '" << std::string(f,l) << "'\n";
}
