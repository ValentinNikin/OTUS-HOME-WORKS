#pragma once

#include "Arguments.h"

#include <boost/program_options.hpp>

class ArgumentsReader {
public:
    ArgumentsReader();
    Arguments parse(int argc, char** argv);
    void printHelp() const;
private:
    boost::program_options::options_description _desc;
};