/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FUNCTION: PARSE_PROPERTIES
          This fuction first tokenize the given line using white space 
          as the delimiter.
          Then the value correspond to the each parameter is saved by its
          appropriate type.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mpm::misc::PARSE_PROPERTIES(std::string& iLine) {

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer; 
    boost::char_separator<char> separator(" ");
    tokenizer token(iLine, separator);
    boost::tokenizer<boost::char_separator<char> >::iterator parameter = token.begin();
    std::string par = *parameter;
    std::advance(parameter,1);

    if (par == "gravityFlag") {
        try {
            gravity = boost::lexical_cast<int>(*parameter);
        }
        catch (const boost::bad_lexical_cast &) {
            std::cerr << "ERROR: failed to parse properties" << "\n";
            abort();
        }
    }
    if (par == "freeSurfaceFlag") {
        try {
            freeSurface = boost::lexical_cast<int>(*parameter);
        }
        catch (const boost::bad_lexical_cast &) {
            std::cerr << "ERROR: failed to parse properties" << "\n";
            abort();
        }
    }

    if (par == "dt") {
        try {
            dt = boost::lexical_cast<double>(*parameter);
        }
        catch (const boost::bad_lexical_cast &) {
            std::cerr << "ERROR: failed to parse properties" << "\n";
            abort();
        }
    }
    if (par == "numOfSteps") {
        try {
            numOfTotalSteps = boost::lexical_cast<unsigned>(*parameter);
        }
        catch (const boost::bad_lexical_cast &) {
            std::cerr << "ERROR: failed to parse properties" << "\n";
            abort();
        }
    }
    if (par == "numOfSubStep") {
        try {
            numOfSubSteps = boost::lexical_cast<unsigned>(*parameter);
        }
        catch (const boost::bad_lexical_cast &) {
            std::cerr << "ERROR: failed to parse properties" << "\n";
            abort();
        }
    }

    // std::cout << gravity << "\t" << projection << "\t" << dt << "\t" << numOfTotalSteps << "\n";
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FUNCTION: READ_PROPERTIES
          This fuction first tokenize the given line using white space 
          as the delimiter.
          Then the value and the property name is stored in a std::map.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mpm::misc::READ_PROPERTIES(std::string& line) {

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer; 
    boost::char_separator<char> separator(" ");
    tokenizer token(line, separator);
    boost::tokenizer<boost::char_separator<char> >::iterator parameter = token.begin();
    std::string propName = *parameter;
    std::advance(parameter,1);

    double propValue = boost::lexical_cast<double>(*parameter);
    propertyList[propName] = propValue;

}
