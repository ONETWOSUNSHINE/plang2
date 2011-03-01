/// \file options.cpp
///

#include <string.h>
#include <iostream>

#include "options.h"
#include "utils.h"

static
bool _handlePrettyPrint(const std::string &_val, void *_p) {
    Options &opts = *(Options *)_p;

    if (_val == "none" || _val == "0")
        opts.prettyPrint = PP_NONE;
    else if (_val == "flat" || _val == "1")
        opts.prettyPrint |= PP_FLAT;
    else if (_val == "ast" || _val == "2")
        opts.prettyPrint |= PP_AST;
    else if (_val == "lex" || _val == "4")
        opts.prettyPrint |= PP_LEX;
    else
        return false;

    return true;
}

static
bool _handleBackEnd(const std::string &_val, void *_p) {
    Options &opts = *(Options *)_p;

    if (_val == "none" || _val == "0")
        opts.backEnd = BE_NONE;
    else if (_val == "pp" || _val == "1")
        opts.backEnd |= BE_PP;
    else if (_val == "c" || _val == "2")
        opts.backEnd |= BE_C;
    else
        return false;

    return true;
}

static
bool _handleTypeCheck(const std::string &_val, void *_p) {
    Options &opts = *(Options *)_p;

    if (_val == "none" || _val == "0")
        opts.typeCheck = TC_NONE;
    else if (_val == "on" || _val == "1")
        opts.typeCheck = TC_ON;
    else
        return false;

    return true;
}

static
bool _handleNotAnOption(const std::string &_val, void *_p) {
    Options &opts = *(Options *)_p;
    opts.args.push_back(_val);
    return true;
}

static
void _printUsage() {
    std::cerr << "Usage:\n\n"
        << "    plang [OPTIONS] FILE\n\n"
        << "Options\n\n"
        << "    -p, --prettyprint=TYPE        Pretty-printer mode, where TYPE is 'none' (0), 'flat' (1), 'ast' (2)\n"
        << "    -b, --backend=TYPE            Use backend, where TYPE is 'none' (0), 'pp' (1), 'c' (2)\n"
        << "    -t, --typecheck=TYPE          Do typecheck, where TYPE is 'none' (0), 'on' (1)\n"
        << "    -o, --output=FILE             Output file name\n"
        << "    -v, --verbose                 Print debug info\n"
        << "        --help                    Show this message\n";
}

bool Options::init(size_t _cArgs, const char **_pArgs) {
    bool bHelp = false;
    Option options[] = {
        { "prettyprint", 'p', _handlePrettyPrint, NULL,                   NULL,                          true  },
        { "backend",     'b', _handleBackEnd,     NULL,                   NULL,                          true  },
        { "typecheck",   't', _handleTypeCheck,   NULL,                   NULL,                          false },
        { "output",      'o', NULL,               NULL,                   &instance().strOutputFilename, false },
        { "verbose",     'v', NULL,               &instance().bVerbose,   NULL,                          false },
        { "help",        'h', NULL,               &bHelp,                 NULL,                          false },
        { NULL,           0,  NULL,               NULL,                   NULL,                          false }
    };

    if (parseOptions(_cArgs, _pArgs, options, &instance(), _handleNotAnOption)) {
        if (!instance().args.empty()) {
            instance().strInputFilename = instance().args.front();
            return true;
        }

        std::cerr << "No input filename given" << std::endl;
    }

    _printUsage();
    return false;
}

Options::Options() :
    prettyPrint(PP_NONE),
    typeCheck(TC_ON),
    backEnd(BE_NONE),
    bVerbose(false)
{
}

Options &Options::instance() {
    static Options g_options;
    return g_options;
}
