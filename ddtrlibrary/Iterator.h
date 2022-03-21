#ifndef ITERATOR_H
#define ITERATOR_H

#include "DDTIncludes.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {
	template <class iteratorExtension> class ReverseIteratorAdapter;
	template <class iteratorExtension> class ConstIteratorAdapter;

	template <class DataStructureType> class Iterator_Base_Unlogged;
        
	template <class DataStructureType> class Iterator_Base;
	///////////////////////////////////////////////////////////////
	// Iterators extentions. These "extensions" should provide
	// implementations for specific functions of the base class.
	// Furthermore, these extensions can be used to define to 
	// members to the iterator class.
	
	///////////////////////////////////////////////////////////////
	// Iterators Bases
	///////////////////////////////////////////////////////////////
	template <class DataStructureType>
	class Iterator_Base {
	public:
		////////////////////////////////////////////////////////////////////////
        enum { type_num = DataStructureType::type_num };
        typedef typename DataStructureType::Logger Logger;
        ////////////////////////////////////////////////////////////////////////
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef DataStructureType IteratorDataStructureType;
		typedef typename DataStructureType::value_type value_type;
		typedef typename DataStructureType::pointer pointer;
		typedef typename DataStructureType::reference reference;
		typedef typename DataStructureType::element_pointer element_pointer;
		typedef typename DataStructureType::size_type size_type;
		typedef typename DataStructureType::difference_type difference_type;
		typedef typename DataStructureType::input_value_type input_value_type;

		typedef typename DataStructureType::element_pointer_unlogged element_pointer_unlogged;
	    
		// Constructors
		Iterator_Base () {}

        Iterator_Base (const DataStructureType* datastructure, element_pointer data, unsigned int instance_id):
        _datastructure(datastructure), _data(data), _instance_id(instance_id) {}
        
		Iterator_Base (const Iterator_Base& itr)
			: _datastructure(itr._datastructure), _data(itr._data), _instance_id(itr._instance_id) {
		}
        
        Iterator_Base (const Iterator_Base_Unlogged<DataStructureType>& itr)
            : _datastructure(itr._datastructure), _data(itr._data), _instance_id(itr._instance_id) {
        }

		// Destructor.
		~Iterator_Base() {}

		virtual void increment() {
			_data = _datastructure->increment(_data);
		}
		virtual void decrement() {
			_data = _datastructure->decrement(_data);
		}

		Iterator_Base& operator=(const Iterator_Base& other) {
			/*#ifdef LOGGING
				typename Logger::iterator_reset S(TYPEID(input_value_type), type_num, other._instance_id, other._index);
			#endif*/
			_instance_id = other._instance_id;
			_data = other._data;
			_datastructure = other._datastructure;
			return *this;
		}
		//Iterator_Base& operator=(const Iterator_Base<DataStructureType>& other) {
		//	#ifdef LOGGING
		//		typename Logger::iterator_reset S(TYPEID(input_value_type), type_num, other._instance_id, other._index);
		//	#endif
		//	_instance_id = other._instance_id;
		//	_data = other._data;
		//	return *this;
		//}

		Iterator_Base& operator++() {
			#ifdef LOGGING
				typename Logger::iterator_next S(TYPEID(input_value_type), type_num, _instance_id,
											 sizeof(input_value_type), (void *) _data);
			#endif
			increment();
			return *this;
		}
		Iterator_Base operator++(int) {
            #ifdef LOGGING
            	typename Logger::iterator_next S(TYPEID(input_value_type), type_num, _instance_id,
											 sizeof(input_value_type), (void *) _data);
            #endif
			Iterator_Base tmp = *this;
			increment();
			return tmp;
		}
		Iterator_Base& operator--() {
			#ifdef LOGGING
				typename Logger::iterator_previous S(TYPEID(input_value_type), type_num, _instance_id,
												 sizeof(input_value_type), (void *) _data);
			#endif
			decrement();
			return *this;
		}
		Iterator_Base operator--(int) {
            #ifdef LOGGING
            	typename Logger::iterator_previous S(TYPEID(input_value_type), type_num, _instance_id,
												 sizeof(input_value_type), (void *) _data);
            #endif
			Iterator_Base tmp = *this;
			decrement();
			return tmp;
		}


		Iterator_Base& operator+=(size_type n) {
            #ifdef LOGGING
            	typename Logger::iterator_add S(TYPEID(input_value_type), type_num, _instance_id,
									        sizeof(input_value_type), (void *) _data, n);
            #endif
			for (size_type i = 0; i < n; i++)
				increment();
			return *this;
		}

		Iterator_Base operator+(size_type n) const {
			Iterator_Base tmp(*this);
			tmp += n;
			return tmp;
		}

		Iterator_Base& operator-=(size_type n) {
            #ifdef LOGGING
            	typename Logger::iterator_sub S(TYPEID(input_value_type), type_num, _instance_id,
											sizeof(input_value_type), (void *) _data, n);
            #endif
			for (size_type i = 0; i < n; i++)
				decrement();
			return *this;
		}

		Iterator_Base operator-(size_type n) const {
			Iterator_Base tmp(*this);
			tmp -= n;
			return tmp;
		}

			
		difference_type operator-(const Iterator_Base& other) const {
			//from MSDN help:
			//Note that although difference_type is available for all iterators that satisfy the requirements of an input iterator, which includes the class of bidirectional iterators supported by reversible containers like set, subtraction between iterators is only supported by random-access iterators provided by a random-access container, such as Vector Class.
			//Therefore the following code work only if it2 is past it1.
			difference_type diff = 0;
            Iterator_Base it1(*this);
			Iterator_Base it2(other);
			if (_instance_id != it2.getId())
				return NULL;
			else {
				for (;it1 != it2; it1++)
					diff++;
				return diff;
			}	
		}

		bool operator==(const Iterator_Base & other) const {
			return (_data == other._data);
		}
		bool operator!=(const Iterator_Base & other) {
			return (_data != other._data);
		}
        bool operator<=(const Iterator_Base &other) {
            return (_data <= other._data);
        }
        bool operator>=(const Iterator_Base &other) {
            return (_data >= other._data);
        }
        bool operator<(const Iterator_Base &other) {
            return (_data < other._data);
        }
        bool operator>(const Iterator_Base &other) {
            return (_data > other._data);
        }

		reference operator*() {
            #ifdef LOGGING
            	typename Logger::iterator_get S(TYPEID(input_value_type), type_num, _instance_id,
											sizeof(input_value_type), (void *) _data);
            #endif
			return *(_datastructure->getData(_data)); 
		}
		
        pointer operator->() const {
            #ifdef LOGGING
                typename Logger::iterator_get S(TYPEID(input_value_type), type_num, _instance_id,
                                            sizeof(input_value_type), (void *) _data);
            #endif
            return _datastructure->getData(_data);
        }

		
		element_pointer_unlogged getData() const {
			return _data;
		}
		unsigned int getId() {
			return _instance_id;
		}
	    
	  // --------------------- End public protocol -----------------------

	 
	protected:
		/*
         Iterator_Base (DataStructureType* datastructure, element_pointer_unlogged data,
			const unsigned int instance_id)
			: _datastructure(datastructure), _data(data), _instance_id(instance_id) {}
         */
		unsigned int _instance_id;
		element_pointer _data;			// The item being inspected
		const DataStructureType* _datastructure;		// The datastructure being inspected

		template <class, class, class> friend class Vector;
		template <class> friend class List;
		template <class> friend class Tree;
		template <class, class> friend class GlueLayer;
        template <class> friend class Iterator_Base_Unlogged;
	};

	template <class DataStructureType>
	class Iterator_Base_Unlogged {
	public:
		////////////////////////////////////////////////////////////////////////
        enum { type_num = DataStructureType::type_num };
        typedef typename DataStructureType::Logger Logger;
        ////////////////////////////////////////////////////////////////////////
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef DataStructureType IteratorDataStructureType;
		typedef typename DataStructureType::value_type value_type;
		typedef typename DataStructureType::pointer pointer;
		typedef typename DataStructureType::reference reference;
		typedef typename DataStructureType::element_pointer element_pointer;
		typedef typename DataStructureType::size_type size_type;
		typedef typename DataStructureType::difference_type difference_type;
		typedef typename DataStructureType::input_value_type input_value_type;

		typedef typename DataStructureType::element_pointer_unlogged element_pointer_unlogged;
	    
		// Constructors
		Iterator_Base_Unlogged () {}
        
        Iterator_Base_Unlogged (const DataStructureType* datastructure, element_pointer_unlogged data):
        _datastructure(datastructure), _data(data) {}
        
        Iterator_Base_Unlogged (const DataStructureType* datastructure, element_pointer_unlogged data, unsigned int instance_id):
        _datastructure(datastructure), _data(data), _instance_id(instance_id) {}

		Iterator_Base_Unlogged (const Iterator_Base_Unlogged& itr)
			: _datastructure(itr._datastructure), _data(itr._data) {
		}
        Iterator_Base_Unlogged (const Iterator_Base<DataStructureType>& itr)
            : _datastructure(itr._datastructure), _data(itr._data) {
        }

		// Destructor.
		~Iterator_Base_Unlogged() {}

		virtual void increment() {
			_data = _datastructure->increment(_data);
		}
		virtual void decrement() {
			_data = _datastructure->decrement(_data);
		}

		Iterator_Base_Unlogged& operator=(const Iterator_Base_Unlogged& other) {
			_data = other._data;
			_datastructure = other._datastructure;
			return *this;
		}

		Iterator_Base_Unlogged& operator++() {
			increment();
			return *this;
		}
		Iterator_Base_Unlogged operator++(int) {
			Iterator_Base_Unlogged tmp = *this;
			increment();
			return tmp;
		}
		Iterator_Base_Unlogged& operator--() {
			decrement();
			return *this;
		}
		Iterator_Base_Unlogged& operator--(int) {
			Iterator_Base_Unlogged tmp = *this;
			decrement();
			return tmp;
		}


		Iterator_Base_Unlogged& operator+=(size_type n) {
			for (size_type i = 0; i < n; i++)
				increment();
			return *this;
		}

		Iterator_Base_Unlogged operator+(size_type n) const {
			Iterator_Base_Unlogged tmp(*this);
			tmp += n;
			return tmp;
		}

		Iterator_Base_Unlogged& operator-=(size_type n) {
			for (size_type i = 0; i < n; i++)
				decrement();
			return *this;
		}

		Iterator_Base_Unlogged operator-(size_type n) const {
			Iterator_Base_Unlogged tmp(*this);
			tmp -= n;
			return tmp;
		}

			
		difference_type operator-(const Iterator_Base_Unlogged& other) const {
			//from MSDN help:
			//Note that although difference_type is available for all iterators that satisfy the requirements of an input iterator, which includes the class of bidirectional iterators supported by reversible containers like set, subtraction between iterators is only supported by random-access iterators provided by a random-access container, such as Vector Class.
			//Therefore the following code work only if it2 is past it1.
			difference_type diff = 0;
            Iterator_Base_Unlogged<DataStructureType> it2(*this);
            Iterator_Base_Unlogged<DataStructureType> it1(other);
			/*if (_instance_id != it2.getId())
				return NULL;
			else {*/
				for (;it1 != it2; it1++)
					diff++;
				return diff;
				//}	
		}
		
		bool operator==(const Iterator_Base_Unlogged & other) const {
			return (_data == other._data);
		}
		bool operator!=(const Iterator_Base_Unlogged & other) {
			return (_data != other._data);
		}
        bool operator<=(const Iterator_Base_Unlogged &other) {
            return (_data <= other._data);
        }
        bool operator>=(const Iterator_Base_Unlogged &other) {
            return (_data >= other._data);
        }
        bool operator<(const Iterator_Base_Unlogged &other) {
            return (_data < other._data);
        }
        bool operator>(const Iterator_Base_Unlogged &other) {
            return (_data > other._data);
        }
        

		reference operator*() {
			return *(_datastructure->getData(_data));
		}
		pointer operator->() const {
			return _datastructure->getData(_data);
		}
		
		element_pointer_unlogged getData() const {
			return _data;
		}
	    
	  // --------------------- End public protocol -----------------------
	 
	protected:
		/*Iterator_Base_Unlogged (DataStructureType* datastructure, element_pointer_unlogged data,
			const unsigned int dummy = 0)
			: _datastructure(datastructure), _data(data) {}
         */
        unsigned int _instance_id;
		element_pointer _data;					// The item being inspected
		const DataStructureType* _datastructure;		// The datastructure being inspected

		template <class> friend class List;
		template <class> friend class Tree;
		template <class, class, class> friend class Vector;
		template <class, class> friend class GlueLayer;
        template <class> friend class Iterator_Base;
	};


	///////////////////////////////////////////////////////////////
	// Iterator Adapters. These classes add functionality to 
	// the iterator.
	///////////////////////////////////////////////////////////////
	template <class iteratorExtension>
	class ReverseIteratorAdapter : public iteratorExtension {    
	public:
		////////////////////////////////////////////////////////////////////////
        enum { type_num = iteratorExtension::type_num };
        typedef typename iteratorExtension::Logger Logger;
        ////////////////////////////////////////////////////////////////////////
		typedef typename iteratorExtension::IteratorDataStructureType DataStructureType;
		typedef typename iteratorExtension::pointer pointer;
		typedef typename iteratorExtension::reference reference;

		typedef typename iteratorExtension::element_pointer_unlogged element_pointer_unlogged;
		
		typedef typename iteratorExtension::size_type size_type;
		typedef typename iteratorExtension::difference_type difference_type;
	       
		typedef typename iteratorExtension::element_pointer element_pointer;
	    
		// Constructors
		ReverseIteratorAdapter () {}

	    ReverseIteratorAdapter (const iteratorExtension& itr)
			: iteratorExtension(itr) {
		}

		// Destructor.
		~ReverseIteratorAdapter() {}
	   
		virtual void increment() {
			iteratorExtension::decrement();
		}
		virtual void decrement() {
			iteratorExtension::increment();
		}

	protected:

		ReverseIteratorAdapter (DataStructureType* datastructure, element_pointer_unlogged data,
			const unsigned int instance_id)
			: iteratorExtension(datastructure, data, instance_id) {}

		template <class, class, class> friend class Vector;
		template <class> friend class List;
		template <class> friend class Tree;
		template <class, class> friend class GlueLayer;
	};

	template <class iteratorExtension>
	class ConstIteratorAdapter : public iteratorExtension {    
	public:
		////////////////////////////////////////////////////////////////////////
        enum { type_num = iteratorExtension::type_num };
        typedef typename iteratorExtension::Logger Logger;
        ////////////////////////////////////////////////////////////////////////
		typedef typename iteratorExtension::IteratorDataStructureType DataStructureType;
		typedef typename iteratorExtension::pointer pointer;
		typedef typename iteratorExtension::reference reference;

		typedef typename iteratorExtension::element_pointer_unlogged element_pointer_unlogged;
		
		typedef typename iteratorExtension::size_type size_type;
		typedef typename iteratorExtension::difference_type difference_type;
	       
		typedef typename iteratorExtension::element_pointer element_pointer;
	    
		// Constructors
		ConstIteratorAdapter () {}

		ConstIteratorAdapter (const Iterator_Base<DataStructureType>& itr)
			: iteratorExtension (itr) { }
        ConstIteratorAdapter (Iterator_Base_Unlogged<DataStructureType>& itr)
            : iteratorExtension (itr) { }
		// Destructor.
		~ConstIteratorAdapter() {}

	protected:

		ConstIteratorAdapter (DataStructureType* datastructure, element_pointer_unlogged data,
			const unsigned int instance_id)
			: iteratorExtension(datastructure, data, instance_id) {}

		template <class, class, class> friend class Vector;
		template <class> friend class List;
		template <class> friend class Tree;
		template <class, class> friend class GlueLayer;
	};

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif
