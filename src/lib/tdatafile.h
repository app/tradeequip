#ifndef _TDATAFILE_H_
#define _TDATAFILE_H_
#include "teinifile.h"
#define TDataFile TEIniFile
#ifdef _MSC_VER
#pragma message("TDataFile object is deprecated. Use #include <teinifile.h> and TEIniFile object instead of #include <tdatafile.h> and TDataFile object.")
#else
#warning TDataFile object is deprecated. Use #include <teinifile.h> and TEIniFile object \
instead of #include <tdatafile.h> and TDataFile object.
#endif
#endif
