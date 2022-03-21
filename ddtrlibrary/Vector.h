#ifndef VECTOR_H
#define VECTOR_H

#include "DDTIncludes.h"

////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {

	//////////////////////////////////////////////////////////////////////////////
	// Predefinition of Vector for friendliness
	//////////////////////////////////////////////////////////////////////////////
template <class T, class objectTraits = Tracing::no_traits, class ddtTraits = Tracing::no_traits> class Vector;

	//////////////////////////////////////////////////////////////////////////////
	// Class template Vector
	// This is a simple Vector DDT
	//////////////////////////////////////////////////////////////////////////////

	template <class T, class objectTraits, class ddtTraits>
	class Vector {
	public:
		enum { type_num = ddtTraits::type_num };
		typedef typename ddtTraits::Logger Logger;
		typedef typename ddtTraits::Allocator Allocator;
		////////////////////////////////////////////////////////////////////////
		// Typing behaviour of DDT
		////////////////////////////////////////////////////////////////////////
		typedef typename ddtTraits::template transform<T*>::value_type element_pointer;
        typedef typename ddtTraits::template transform<T*>::semilogged_value_type element_pointer_semilogged;
		typedef typename ddtTraits::template transform<T&>::value_type element_reference;		
		typedef typename objectTraits::template transform<T>::value_type value_type;
        typedef Tracing::smartptr<T, ddtTraits> smartElementPointer;
        
		typedef T elemenType;
		typedef value_type* pointer;
        
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef ddtTraits Traits;
		typedef T input_value_type;
		typedef const input_value_type& const_input_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef Iterator_Base<Vector> iterator;
		typedef ConstIteratorAdapter<iterator> const_iterator;

		typedef ReverseIteratorAdapter<iterator> reverse_iterator;
		typedef ReverseIteratorAdapter<const_iterator> const_reverse_iterator;

		typedef T* element_pointer_unlogged;

#ifndef MORE_LOGGING			
		typedef Iterator_Base_Unlogged<Vector> iterator_unlogged;
		typedef ReverseIteratorAdapter<iterator_unlogged> reverse_iterator_unlogged;
#else
		typedef iterator iterator_unlogged;
		typedef reverse_iterator reverse_iterator_unlogged;
#endif


		////////////////////////////////////////////////////////////////////////
		// Constructors
		////////////////////////////////////////////////////////////////////////
		Vector()
			: instance_id(NewInstance::createId())
		{

#ifdef LOGGING
			typename Logger::object_construct S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			begin_ = (pointer)Allocator::malloc(16*sizeof(value_type));
			last_ = end_ = begin_;
			finish_ = begin_ + 16;
		}

		Vector(size_type sz)
			: instance_id(NewInstance::createId())
		{

#ifdef LOGGING
			typename Logger::object_construct S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			const size_type alloc_size = MAX(sz, 16); 
			begin_ = (pointer)Allocator::malloc(alloc_size*sizeof(value_type));
			end_ = begin_+sz;
			last_ = begin_+sz-1;
			finish_ = begin_ + alloc_size;
			for (element_pointer_semilogged p = begin_; p < end_; ++p) {
#ifdef LOGGING
				//typename Logger::object_insert S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
#ifdef NOLOGGING
				Constructor(p);
#else
                Constructor(p.getData());
#endif
			}
		}

		Vector(size_type sz, const_input_reference value)
			: instance_id(NewInstance::createId())
		{

#ifdef LOGGING
			//typename Logger::object_construct S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			const size_type alloc_size = MAX(sz, 16); 
			begin_ = (pointer)Allocator::malloc(alloc_size*sizeof(value_type));
			end_ = begin_+sz;
			last_ = begin_+sz-1;
			finish_ = begin_ + alloc_size;
			for (element_pointer_semilogged p = begin_; p < end_; ++p) {
#ifdef LOGGING
				//typename Logger::object_insert S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
#ifdef NOLOGGING
                Constructor(p, value);
#else
                Constructor(p.getData(), value);
#endif
				
			}
		}
        
        Vector(iterator_unlogged first, iterator_unlogged last): instance_id(NewInstance::createId()) {
            begin_ = (pointer)Allocator::malloc(16*sizeof(value_type));
            last_ = end_ = begin_;
            finish_ = begin_ + 16;
            
            for (; first!= last; first++) {
                push_back(*first);
            }
        }

		Vector(const Vector& other)
			: instance_id(NewInstance::createId())
		{

#ifdef LOGGING
			//typename Logger::object_copy S(TYPEID(T), type_num, instance_id, sizeof(T), type_num, other.instance_id);
#endif
			size_type other_size = other.size();
			size_type alloc_size = MAX(other_size, 16); 
			begin_ = (pointer)Allocator::malloc(alloc_size*sizeof(value_type));
			finish_ = begin_ + alloc_size;
            element_pointer_semilogged i;
			for (end_ = begin_, i = other.begin_; i < other.end_; ++end_, ++i) {
#ifdef NOLOGGING
				Constructor(end_, *i);
#else
                Constructor(end_.getData(), *(*i));
#endif
			}
			last_ = end_ - 1;
		}

		template <typename objectTraits2, typename ddtTraits2>
		Vector(const Vector<T, objectTraits2, ddtTraits2>& other)
			: instance_id(NewInstance::createId())
		{
#ifdef LOGGING
			//typename Logger::object_copy S(TYPEID(T), type_num, instance_id, sizeof(T), ddtTraits2::type_num, other.instance_id);
#endif
			size_type other_size = other.size();
			size_type alloc_size = MAX(other_size, 16); 
			begin_ = (pointer)Allocator::malloc(alloc_size*sizeof(value_type));
			finish_ = begin_ + alloc_size;
			typename Vector<T, objectTraits2, ddtTraits2>::element_pointer_semilogged i;
			for (end_ = begin_, i = other.begin_; i < other.end_; ++end_, ++i) {
#ifdef NOLOGGING
                Constructor(end_, *i);
#else
                Constructor(end_.getData(), *(*i));
#endif
				
			}
		}

		Vector& operator=(const Vector& other) {
			if (this != &other) {
				Vector tmp(other);
				swap(tmp); 
			}
			return *this;
		}

		template <typename objectTraits2, typename ddtTraits2>
		Vector& operator=(const Vector<T, objectTraits2, ddtTraits2>& other) {
			Vector tmp(other);
			swap(tmp); 
			return *this;
		}

		~Vector() {
			clear();
#ifdef LOGGING
			typename Logger::object_destruct S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
            Allocator::free(begin_);
		}

		template <typename objectTraits2, typename ddtTraits2>
		void swap(Vector<T, objectTraits2, ddtTraits2>& other) {
#ifdef LOGGING
			//typename Logger::object_swap S(TYPEID(T), type_num, instance_id, sizeof(T), ddtTraits2::type_num, other.instance_id);
#endif
			element_pointer_semilogged tmp = other.begin_;
			other.begin_ = begin_;
			begin_ = tmp;
			tmp = other.end_;
			other.end_ = end_;
			end_ = tmp;
			tmp = other.last_;
			other.last_ = last_;
			last_ = tmp;
			tmp = other.finish_;
			other.finish_ = finish_;
			finish_ = tmp;
		}

		
		////////////////////////////////////////////////////////////////////////
		// Basic iterators of DDT
		////////////////////////////////////////////////////////////////////////
		//const_iterator begin() const {
			//return const_iterator(this, getFirstElement(), instance_id);
		//}
		iterator_unlogged begin() const {
			return iterator_unlogged(this, getFirstElement(), instance_id);
		}
		//const_iterator end() const {
		//	return const_iterator(this, getEnd(), instance_id);
		//}
		iterator_unlogged end() const {
			return iterator_unlogged(this, getEnd(), instance_id);
		}
		////////////////////////////////////////////////////////////////////////
		// Reverse iterators of DDT
		////////////////////////////////////////////////////////////////////////
		reverse_iterator rbegin() {
			return reverse_iterator(this, getLastElement(), instance_id);
		}
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this, getLastElement(), instance_id);
		}
		reverse_iterator rend() {
			return reverse_iterator(this, getREnd(), instance_id);
		}
		const_reverse_iterator rend() const {
			return const_reverse_iterator(this, getREnd(), instance_id);
		}			
		////////////////////////////////////////////////////////////////////////
		// iterator integration
		////////////////////////////////////////////////////////////////////////
        inline element_pointer_semilogged decrement(element_pointer_semilogged data) const {
			return --data;
		}
		inline element_pointer_semilogged increment(element_pointer_semilogged data) const {
			return ++data;
		}
		inline element_pointer_unlogged getData(element_pointer_semilogged data) const {
			return data;
		}
		////////////////////////////////////////////////////////////////////////
		// Glue Layer Integration
		////////////////////////////////////////////////////////////////////////
		inline element_pointer_semilogged getFirstElement() const {
			return begin_;
		}
		inline element_pointer_semilogged getLastElement() const {
			return last_;
		}
		inline element_pointer_semilogged getEnd() const {
			return end_;
		}
		inline element_pointer_semilogged getREnd() const {
			return begin_ - 1;
		}
		bool exists(element_pointer_semilogged x) {
			if (!empty()) {
				iterator_unlogged it1(this, begin_, instance_id);
				iterator_unlogged it2(this, end_, instance_id);
				for (iterator_unlogged it = it1; it != it2; it++) {
					if (it.getData() == x)
						return true;
				}
			}
			return false;
		}
		inline bool IsNil(element_pointer_semilogged x) {
			return (x == end_) || (x == begin_ - 1);
		}
		////////////////////////////////////////////////////////////////////////
		// Size information
		////////////////////////////////////////////////////////////////////////
		bool empty() const {
			return (begin_ == end_);
		}
		size_type size() const {
			return size_type(end_ - begin_);
		}
		size_type capacity() {
			return finish_ - begin_;
		}

		void reserve(size_type new_capacity) {
#ifdef LOGGING
			//typename Logger::object_resize S(TYPEID(T), type_num, instance_id, new_capacity);
#endif
			if (new_capacity > capacity()) {
				const size_type old_size = size();
				element_pointer_semilogged new_storage = (pointer)Allocator::malloc(new_capacity * sizeof(value_type));
				element_pointer_semilogged tmp = new_storage;
				for (element_pointer_semilogged i = begin_; i != end_; ++i, ++tmp) {
#ifdef NOLOGGING
                    Constructor(tmp, *i);
#else
                    Constructor(tmp.getData(), *(*i));
#endif
				}
				Destructor(begin_, end_);
				Allocator::free(begin_);
				begin_ = new_storage;
				end_ = begin_ + old_size;
				last_ = begin_ + old_size - 1;
				finish_ = begin_ + new_capacity;
			}
		}

		////////////////////////////////////////////////////////////////////////
		// Basic access operations
		////////////////////////////////////////////////////////////////////////
		reference operator[](size_type n) const {
            return get(n);
            
        }
		//const_reference operator[](size_type n) const {
		//	return get(n);
		//}

		reference get(size_type n) const {
#ifdef LOGGING
			//typename Logger::object_get S(TYPEID(T), type_num, instance_id, sizeof(T), n);
#endif
			return *(begin_ + n); 
		}

		/*const_reference get(size_type n) const {
		#ifdef LOGGING
		typename Logger::object_get S(TYPEID(T), type_num, instance_id, sizeof(T), n);
		#endif
		return *(begin_ + n); 
		}*/

		/*const_reference front() { return get(0); }*/
		reference front() const { return get(0); }
		/*const_reference back() { return get(size() - 1); }*/
		reference back() const { return get(size() - 1); }

		////////////////////////////////////////////////////////////////////////
		// Basic modification operations
		////////////////////////////////////////////////////////////////////////
		void clear() {
#ifdef LOGGING
			//typename Logger::object_clear S(TYPEID(T), type_num, instance_id, sizeof(input_value_type));
#endif
			//Destructor(begin_, end_);
			//iterator_unlogged it1(this, begin_, instance_id);
			//iterator_unlogged it2(this, end_, instance_id);
			if (!empty()) {
				iterator_unlogged it1(this, begin_, instance_id);
				iterator_unlogged it2(this, end_, instance_id);
				Destructor(it1, it2);
			}
			end_ = begin_;
		}

		iterator_unlogged insert(iterator_unlogged pos, const_input_reference value) {
#ifdef LOGGING
			//typename Logger::object_insert S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			Constructor(end_++, value);
			last_++;
			//iterator it;
			if (pos._data + 1 != end_) {
				end_++;
				last_++;
				element_pointer_semilogged o = last_;
				for (element_pointer_semilogged i = o - 1; i != pos._data; --i, --o) {
					*o = *i;
				}
				*pos._data = value;
			}
			if (end_ == finish_) {
				ptrdiff_t offs = pos._data - begin_;
				reserve(capacity()*2);
				pos._data = begin_ + offs;
			}
			return pos;
		}
        
        

		iterator_unlogged erase(iterator_unlogged pos) {
#ifdef LOGGING
			//typename Logger::object_remove S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			if (pos._data + 1 != end_) {
				element_pointer_semilogged o = pos._data;
				for (element_pointer_semilogged i = o + 1; i != end_; ++i, ++o) {
					*o = *i;
				}
			}
            
			Destructor((--end_));
			last_--;
			return pos;
		}
        
        iterator_unlogged copy_move(iterator_unlogged first, iterator_unlogged last, iterator_unlogged result) {
            while (first != last) {
                *result++ = *first++;
            }
            return result;
        }
        
        /*
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result)
       506            {
       507          while (__first != __last)
       508            *--__result = *--__last;
       509          return __result;
       510        }
         */
        
        iterator_unlogged erase(iterator_unlogged first, iterator_unlogged last) {
#ifdef LOGGING
            //typename Logger::object_remove S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
            if (first != last) {
                element_pointer oldEnd = end_;
                if (last != end()) {
                    end_ = copy_move(last, end(), first).getData();
                }
                Destructor(last._data, oldEnd);
            }
            
            return first;
            /*
                    {
             00491     if (__first != __last)
             169         {
             170           if (__last != end())
             171             _GLIBCXX_MOVE3(__last, end(), __first);
             172           _M_erase_at_end(__first.base() + (end() - __last));
             173         }
             174       return __first;
                    }
            */
            
        }

		void push_back(const_input_reference value) {
			//insert(iterator_unlogged(this, end_, instance_id), value);
#ifdef LOGGING
			//typename Logger::object_insert S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			Constructor(end_++, value);
			last_++;
			if (end_ == finish_) {
				reserve(capacity()*2);
			}
		}

		void pop_back() {
			iterator_unlogged it(this, end_ - 1, instance_id);
			erase(it);
		}
		void pop_front() {
			iterator_unlogged it(this, begin_, instance_id);
			erase(it);
		}

		void resize(size_type new_size, const_input_reference value = input_value_type()) {
			const size_type old_size = size();
			if (new_size > old_size) {
				reserve(new_size);
				for (size_type i = old_size; i < new_size; i++) {
					push_back(value);
				}
			} else if (new_size < old_size) {
				//Destructor(begin_ + new_size, end_); //-- Doesn't log removals :/
				for (size_type i = new_size; i < old_size; i++) {
					pop_back();
				}
			} else {
				return;
			}
		}

		////////////////////////////////////////////////////////////////////////
		// Syntactic sugar
		////////////////////////////////////////////////////////////////////////
		void set(size_type n, const_input_reference value) {
			get(n) = value;
		}

		void add(size_type n, const_input_reference value) {
			insert(iterator_unlogged(begin_ + n, instance_id), value);
		}

		void remove(size_type n) {
			erase(iterator_unlogged(begin_ + n, instance_id));
		}

		void append(const_input_reference value) {
			push_back(value);
		}

		////////////////////////////////////////////////////////////////////////
		// Allocation related items
		////////////////////////////////////////////////////////////////////////
	
	private:

		element_pointer_semilogged erase(element_pointer_semilogged item) {
#ifdef LOGGING
			//typename Logger::object_remove S(TYPEID(T), type_num, instance_id, sizeof(T));
#endif
			if (item + 1 != end_) {
				element_pointer_semilogged o = item;
				for (element_pointer_semilogged i = o + 1; i != end_; ++i, ++o) {
					*o = *i;
				}
			}
			Destructor((--end_));
			last_--;
			return item;
		}

		iterator_unlogged unlogged_begin() {
			iterator_unlogged it(this, getFirstElement(), instance_id);
			return it;
		}
		iterator_unlogged unlogged_end() {
			iterator_unlogged it(this, getEnd(), instance_id);
			return it;
		}

		reverse_iterator_unlogged unlogged_rbegin() {
			return reverse_iterator_unlogged(this, getLastElement(), instance_id);
		}
		reverse_iterator_unlogged unlogged_rend() {
			return reverse_iterator_unlogged(this, getREnd(), instance_id);
		}
		
		element_pointer begin_;
		element_pointer last_;
		element_pointer end_;
		element_pointer finish_;
		const unsigned int instance_id;

		template <class, class, class> friend class Vector;
		template <class, class> friend class GlueLayer;
	};

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif
