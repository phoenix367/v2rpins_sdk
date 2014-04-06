#include "CommandGrammar.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

//#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/include/lex_lexertl.hpp>
//#include <boost/spirit/include/qi_eoi.hpp>
//#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/include/phoenix_statement.hpp>
//#include <boost/spirit/include/phoenix_container.hpp>
//
//using namespace boost::spirit;
//using namespace boost::spirit::ascii;
//
//enum LexerIDs 
//{ 
//    ROTATE_COMMAND
//};
//
//template <typename Lexer>
//struct word_count_tokens : lex::lexer<Lexer>
//{
//    word_count_tokens()
//    : rotate("rotate")
//    {
//        // this lexer will recognize 3 token types: words, newlines, and 
//        // everything else
//        this->self.add
//            (rotate, ROTATE_COMMAND)
//        ;
//    }
//
//    // the token 'word' exposes the matched string as its parser attribute
//    lex::token_def<> rotate;
//};
//
//template <typename Iterator>
//struct word_count_grammar : qi::grammar<Iterator, ascii::space_type>
//{
//    template <typename TokenDef>
//    word_count_grammar(TokenDef const& tok)
//      : word_count_grammar::base_type(start)
//    {
//        using boost::phoenix::ref;
//        using boost::phoenix::size;
//
//        start = rotate_command >> eoi
//              ;
//        rotate_command = tok.rotate >> double_;
//        
//        debug(start);
//        debug(rotate_command);
//    }
//
//    qi::rule<Iterator, ascii::space_type> start;
//    qi::rule<Iterator, ascii::space_type> rotate_command;
//};

bool parseCommandStr(
        const std::string& str,
        QSharedPointer<SocketCommand>& cmd)
{
    /*
   typedef lex::lexertl::token<
        char const*, boost::mpl::vector<std::string>
    > token_type;

   typedef lex::lexertl::lexer<token_type> lexer_type;

   typedef word_count_tokens<lexer_type>::iterator_type iterator_type;

    // now we use the types defined above to create the lexer and grammar
    // object instances needed to invoke the parsing process
    word_count_tokens<lexer_type> word_count;          // Our lexer
    word_count_grammar<iterator_type> g (word_count);  // Our parser 

    // read in the file int memory
    char const* first = str.c_str();
    char const* last = &first[str.size()];
    boost::spirit::ascii::space_type space;

    bool r = lex::tokenize_and_parse(first, last, word_count, g, space);

    if (r) {
    }
    else {
        std::string rest(first, last);
        std::cerr << "Parsing failed\n" << "stopped at: \"" 
                  << rest << "\"\n";
    }
    
    return r;
    */
    
    typedef std::vector< std::string > split_vector_type;
    split_vector_type SplitVec;
    
    std::string commandStr = str;
    boost::trim(commandStr);
    boost::split( SplitVec, commandStr, boost::is_any_of(" \t"), 
            boost::token_compress_on );
    
    if (SplitVec.size() >= 1 && SplitVec[0] == "move")
    {
        if (SplitVec.size() >= 2)
        {
            bool direction;
            if (SplitVec[1] == "forward")
            {
                direction = true;
            }
            else if (SplitVec[1] == "backward")
            {
                direction = false;
            }
            else
            {
                return false;
            }
            
            if (SplitVec.size() == 3)
            {
                std::string durationStr = SplitVec[2];
                float duration = boost::lexical_cast<float>(durationStr);

                if (duration < 0.0f)
                {
                    return false;
                }

                cmd = QSharedPointer<SocketCommand>(
                        new MoveTimeCommand(duration, direction));

                return true;
            }
        }
    }
    else if (SplitVec.size() == 2)
    {
        if (SplitVec[0] == "rotate")
        {
            std::string angleStr = SplitVec[1];
            float angle = boost::lexical_cast<float>(angleStr);
            
            cmd = QSharedPointer<SocketCommand>(
                    new RotateCommand(angle));
            
            return true;
        }
        else if (SplitVec[0] == "wait")
        {
            std::string durationStr = SplitVec[1];
            float duration = boost::lexical_cast<float>(durationStr);
            
            if (duration < 0.0f)
            {
                return false;
            }
            
            cmd = QSharedPointer<SocketCommand>(
                    new WaitCommand(duration));
            
            return true;
        }
    }
    
    return false;
}
