#ifndef CONFIG_H
#define CONFIG_H


#include "definitions.h"
class compositeObject;


#define SAMPLING_PERCENTAGE 10
#define LOGFILENAME "log_MO-CMA-ES-DLLROVINGLIST.out"
typedef ddt_helper<compositeObject, 0>::SLLLIST myddtType0;
typedef ddt_helper<compositeObject, 1>::SLLLIST myddtType1;
typedef ddt_helper<compositeObject, 2>::SLLLIST myddtType2;
typedef ddt_helper<compositeObject, 3>::SLLLIST myddtType3;


/*
typedef shark::blas::vector<double> RealVector;

typedef shark::AbstractSingleObjectiveOptimizer<remora::vector<double, remora::cpu_tag> >::SearchPointType SearchPointType;
typedef remora::vector<float, remora::cpu_tag> SearchPointTypeDouble;

typedef ddt_helper<SearchPointType, 0>::SLLLIST vector0;
typedef ddt_helper<RealVector, 1>::SLLLIST vector1;
//typedef ddt_helper<IndividualType, 2>::SLLLIST vector2;
typedef ddt_helper<class ResultType, 3>::SLLLIST vector3;
*/
#endif
