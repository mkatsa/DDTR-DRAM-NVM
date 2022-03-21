#ifndef DEFINITIONS_H
#define DEFINITIONS_H
//#define LOGGING
//#define NOLOGGING
//#define CODE_OPTIMIZATION
#define FACE_STUPID_ERROR
//#define MORE_LOGGING

//#define ORIGINAL_CODE

#include "Vector.h"
#include "List.h"
#include "Tree.h"
//#include "ListAsATree.h"
//#include "MwayTree.h"
#include "Map.h"
#include "Iterator.h"
#include "GlueLayer.h"
#include "NewGlueLayer.h"
#include "Rb-tree.h"
#include "avltree.h"


class compositeObject {
    
    
public:
    
    compositeObject() {
        //std::cout << "compositeObject constructed" << std::endl;
        for (int i =0; i < 1000; i++ ) {
            data[i] = 0;
        }
    }
    
    compositeObject(const compositeObject& other) {
        //std::cout << "compositeObject copy-constructed" << std::endl;
        for (int i = 0; i < 1000; i++) {
            data[i] = other.data[i];
        }
        
    }
    
    ~compositeObject() {
        //std::cout << "compositeObject destructed" << std::endl;
        //std::cout << "destructor compositeObject call*ed" << std::endl;
    }
    
    /*
    void * operator new(size_t size)
       {
           std::cout<< "Overloading new operator with size: " << size << std::endl;
           void * p = ::operator new(size);
           //void * p = malloc(size); will also work fine
        
           return p;
       }
    
       void operator delete(void * p)
       {
           std::cout<< "Overloading delete operator " << std::endl;
           free(p);
       }
     */
    
private:
    int data[1000];
    
};

typedef Tracing::scope<Tracing::DMMLogger> SCOPE;
template <typename T, int I = 1>
struct ddt_helper {
	
	//List helper definitions
    typedef DDTLibrary::sllListElement<T, Tracing::no_traits, Tracing::var_traits<TRAITS(I)> > SLLELEMENT;
    typedef DDTLibrary::dllListElement<T, Tracing::no_traits, Tracing::var_traits<TRAITS(I)> > DLLELEMENT;
    typedef DDTLibrary::sllListElement<T, Tracing::no_traits, Tracing::std_traits<I> > SLLELEMENT_NOLOGGING;
    typedef DDTLibrary::dllListElement<T, Tracing::no_traits, Tracing::std_traits<I> > DLLELEMENT_NOLOGGING;
	typedef DDTLibrary::listBase<SLLELEMENT> SLLLISTBASE;
	typedef DDTLibrary::listBase<DLLELEMENT> DLLLISTBASE;
    typedef DDTLibrary::listBase<SLLELEMENT_NOLOGGING> SLLLISTBASE_NOLOGGING;
    typedef DDTLibrary::listBase<DLLELEMENT_NOLOGGING> DLLLISTBASE_NOLOGGING;
	typedef DDTLibrary::listBaseNoSentinel<SLLELEMENT> SLL_NOSENTINEL_LISTBASE;
	typedef DDTLibrary::listBaseNoSentinel<DLLELEMENT> DLL_NOSENTINEL_LISTBASE;
    typedef DDTLibrary::listBaseNoSentinel<SLLELEMENT_NOLOGGING> SLL_NOSENTINEL_LISTBASE_NOLOGGING;
    typedef DDTLibrary::listBaseNoSentinel<SLLELEMENT_NOLOGGING> DLL_NOSENTINEL_LISTBASE_NOLOGGING;
	typedef DDTLibrary::sll<SLLLISTBASE> SLLLISTTYPE;
	typedef DDTLibrary::dll<DLLLISTBASE> DLLLISTTYPE;
    typedef DDTLibrary::sll<SLLLISTBASE_NOLOGGING> SLLLISTTYPE_NOLOGGING;
    typedef DDTLibrary::dll<DLLLISTBASE_NOLOGGING> DLLLISTTYPE_NOLOGGING;
	typedef DDTLibrary::sll<SLL_NOSENTINEL_LISTBASE> SLLLIST_NOSENTINEL_TYPE;
	typedef DDTLibrary::dll<DLL_NOSENTINEL_LISTBASE> DLLLIST_NOSENTINEL_TYPE;
    typedef DDTLibrary::sll<SLL_NOSENTINEL_LISTBASE_NOLOGGING> SLLLIST_NOSENTINEL_TYPE_NOLOGGING;
    typedef DDTLibrary::dll<DLL_NOSENTINEL_LISTBASE_NOLOGGING> DLLLIST_NOSENTINEL_TYPE_NOLOGGING;
	typedef DDTLibrary::Vector<T, Tracing::no_traits, Tracing::var_traits<TRAITS(I)> > VECTOR;
    typedef DDTLibrary::Vector<T, Tracing::no_traits, Tracing::std_traits<I> > VECTOR_NOLOGGING;

	//Tree helper definitions
	
	typedef DDTLibrary::BinaryTreeNode<T, Tracing::no_traits, Tracing::var_traits<TRAITS(I)> > BINARYTREENODE;
	typedef DDTLibrary::RedBlackTreeNodeExtension<BINARYTREENODE> REDBLACKTREENODE;
	typedef DDTLibrary::BinaryTree<BINARYTREENODE> BINARYTREEBASE;
	typedef DDTLibrary::BinaryTree<REDBLACKTREENODE> REDBLACKTREE_BASE;
	typedef DDTLibrary::RedBlackTreeExtension<REDBLACKTREE_BASE> REDBLACKTREEBASE;
	

	//DDT definitions
	typedef DDTLibrary::List<SLLLISTTYPE> SLLLIST;
	typedef DDTLibrary::List<DLLLISTTYPE> DLLLIST;
    typedef DDTLibrary::List<SLLLISTTYPE_NOLOGGING> SLLLIST_NOLOGGING;
    typedef DDTLibrary::List<DLLLISTTYPE_NOLOGGING> DLLLIST_NOLOGGING;
	typedef DDTLibrary::List<DDTLibrary::sllRovingExtension<SLLLISTTYPE> > SLLROVINGLIST;
	typedef DDTLibrary::List<DDTLibrary::dllRovingExtension<DLLLISTTYPE> > DLLROVINGLIST;
    typedef DDTLibrary::List<DDTLibrary::sllRovingExtension<SLLLISTTYPE_NOLOGGING> > SLLROVINGLIST_NOLOGGING;
    typedef DDTLibrary::List<DDTLibrary::dllRovingExtension<DLLLISTTYPE_NOLOGGING> > DLLROVINGLIST_NOLOGGING;
	typedef DDTLibrary::List<SLLLIST_NOSENTINEL_TYPE> SLLLIST_NOSENTINEL;
	typedef DDTLibrary::List<DLLLIST_NOSENTINEL_TYPE> DLLLIST_NOSENTINEL;
    typedef DDTLibrary::List<SLLLIST_NOSENTINEL_TYPE_NOLOGGING> SLLLIST_NOSENTINEL_NOLOGGING;
    typedef DDTLibrary::List<DLLLIST_NOSENTINEL_TYPE_NOLOGGING> DLLLIST_NOSENTINEL_NOLOGGING;
    typedef DDTLibrary::List<DDTLibrary::sllRovingExtension<SLLLIST_NOSENTINEL_TYPE> > SLLNOSENTINELROVINGLIST;
    typedef DDTLibrary::List<DDTLibrary::dllRovingExtension<DLLLIST_NOSENTINEL_TYPE> > DLLNOSENTINELROVINGLIST;	

			
	typedef DDTLibrary::Tree<BINARYTREEBASE> BINARYTREE;
	typedef DDTLibrary::Tree<REDBLACKTREEBASE> REDBLACKTREE;
	
	typedef VECTOR vectorType;

	/*
	//Glue Layer definitions
	typedef DLLLIST INTERNALDDT;

	typedef DDTLibrary::sllListElement<INTERNALDDT, Tracing::std_traits, Tracing::var_traits<TRAITS(I)> > EXTERNALDDTELEMENT;
	typedef DDTLibrary::listBase<EXTERNALDDTELEMENT> EXTERNALDDTBASE;
	typedef DDTLibrary::sll<EXTERNALDDTBASE> EXTERNALDDTTYPE;

	typedef DDTLibrary::List<EXTERNALDDTTYPE> EXTERNALDDT;
	typedef DDTLibrary::NewGlueLayer<EXTERNALDDT, INTERNALDDT> GLUEDTYPE;
	
	typedef DDTLibrary::listBase<vectorType> newGlueLayer;
	*/
	//Types
	

	//typedef BINARYTREE treeType1;

	//typedef REDBLACKTREE treeType2;

	//typedef GLUEDTYPE gluedType1;

};
/*
template <typename Key, typename Tp, int I = 1>
struct ddtLibraryMap {
  
  typedef DDTLibrary::Map<Key, Tp, Tracing::no_traits, Tracing::var_traits<TRAITS(I), >
}*/

//typedef DDTLibrary::Vector<int, Tracing::std_traits, Tracing::var_traits<TRAITS(2)> > vector1;
//typedef DDTLibrary::Vector<int, Tracing::std_traits, Tracing::var_traits<TRAITS(3)> > vector2;



#endif
