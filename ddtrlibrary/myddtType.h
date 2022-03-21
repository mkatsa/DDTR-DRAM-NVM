#ifndef MYDDTTYPE_H
#define MYDDTTYPE_H

#include "definitions.h"
class compositeObject;

#define LOGFILENAME "log_DLLIST.out"

typedef ddt_helper<compositeObject, 0>::SLLLIST myddtType0;
typedef ddt_helper<compositeObject, 1>::SLLLIST myddtType1;
typedef ddt_helper<compositeObject, 2>::SLLLIST myddtType2;
typedef ddt_helper<compositeObject, 3>::SLLROVINGLIST myddtType3;
typedef ddt_helper<compositeObject, 4>::SLLLIST myddtType4;

#endif
