#include <typeinfo>
#include <assert.h>
#include <vector>
#include <list>

//Linux Definitions
#include "customProfiler/var.h"
#include "customProfiler/construct.h"
#include "customProfiler/texttraits.h"
#include "customProfiler/bintraits.h"
#include "customProfiler/traits.h"
#include "customProfiler/scope.h"
#include "customProfiler/smartptr.h"

#include "NewInstance.h"
#include "Iterator.h"

#define TRAITS(X) Tracing::ddtr_traits<X>
//#define TRAITS(X) Tracing::text_traits<X>

// gia to Glue Layer
#define COUNTLIMIT 16 //prepei na einai idio me to vector
#define COUNTSTATE 0 

#ifndef NULL
#define NULL  0
#endif

#ifndef RED
#define RED   0
#endif

#ifndef BLACK
#define BLACK 1
#endif


#ifndef TYPEID
#define TYPEID(X) (typeid(X).name())
#endif

#ifndef MAX
#define MAX(x,y)  ((x) < (y) ? (y) : (x))
#endif

