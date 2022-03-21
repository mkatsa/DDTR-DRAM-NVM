#ifndef GLUE_LAYER_H
#define GLUE_LAYER_H

#include "DDTIncludes.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {
	
	template <class ExternalDDT, class InternalDDT> class GlueLayer;

	template <class ExternalDDT, class InternalDDT>
	class GlueLayer {
	public:
		enum { type_num = InternalDDT::type_num };
		typedef typename InternalDDT::Logger Logger;

		//Internal Element Types
		typedef typename InternalDDT::pointer pointer;
		typedef typename InternalDDT::reference reference;
		typedef typename InternalDDT::const_reference const_reference;

		typedef typename InternalDDT::element_pointer element_pointer;
		typedef typename InternalDDT::element_reference element_reference;

		typedef typename InternalDDT::element_pointer_unlogged element_pointer_unlogged;

		typedef typename InternalDDT::input_value_type input_value_type;
		typedef typename InternalDDT::const_input_reference const_input_reference;

		typedef typename InternalDDT::size_type size_type;
		typedef typename InternalDDT::value_type value_type;
		typedef typename InternalDDT::difference_type difference_type;

		typedef typename InternalDDT::iterator_unlogged internal_iterator_unlogged;
		typedef typename InternalDDT::reverse_iterator_unlogged internal_reverse_iterator_unlogged;

		//External Element Types
		typedef typename ExternalDDT::pointer external_pointer;
		typedef typename ExternalDDT::reference external_reference;
		typedef typename ExternalDDT::const_reference external_const_reference;

		typedef typename ExternalDDT::element_pointer external_element_pointer;
		typedef typename ExternalDDT::element_reference external_element_reference;

		typedef typename ExternalDDT::element_pointer_unlogged external_element_pointer_unlogged;

		typedef typename ExternalDDT::input_value_type external_input_value_type;
		typedef typename ExternalDDT::const_input_reference external_const_input_reference;

		typedef typename ExternalDDT::size_type external_size_type;

		typedef typename ExternalDDT::iterator_unlogged external_iterator_unlogged;
		typedef typename ExternalDDT::reverse_iterator_unlogged external_reverse_iterator_unlogged;

		////////////////////////////////////////////////////////////////////////////////////////////
		typedef Iterator_Base<GlueLayer> iterator;
		typedef ConstIteratorAdapter<iterator> const_iterator;

		typedef ReverseIteratorAdapter<iterator> reverse_iterator;
		typedef ReverseIteratorAdapter<const_iterator> const_reverse_iterator;
		////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MORE_LOGGING
		typedef Iterator_Base_Unlogged<GlueLayer> iterator_unlogged;
		typedef ReverseIteratorAdapter<iterator_unlogged> reverse_iterator_unlogged;
#else
		typedef iterator iterator_unlogged;
		typedef reverse_iterator reverse_iterator_unlogged;
#endif

		GlueLayer(): externalDDT(), countState(COUNTSTATE), count(0), instance_id(NewInstance::createId()) {
#ifdef LOGGING
			typename Logger::object_construct S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type));
#endif
			InternalDDT * ptr = new InternalDDT;
			externalDDT.push_back(*ptr);
			recentlyUsedExternalElementPointer = externalDDT.getFirstElement();
			recentlyUsedIndex = 0;
			countState.push_back(0);
		}

		void clear() {
			//external_iterator_unlogged begin = externalDDT.unlogged_begin();
			//external_iterator_unlogged end = externalDDT.unlogged_end();
			//for (external_iterator_unlogged it = begin; it != end; it++)
			//{
			//	dynamic_cast<InternalDDT*>(&(*it))->~InternalDDT();
			//}
			externalDDT.clear();
			//recentlyUsedExternalElementPointer = externalDDT.getEnd(); //to check
			//recentlyUsedIndex = -1;
			count = 0;
		}

		~GlueLayer() {
#ifdef LOGGING
			typename Logger::object_destruct S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type));
#endif
			clear(); 
		}



#ifdef FACE_STUPID_ERROR
		GlueLayer& operator=(const GlueLayer& other) {
			externalDDT = other.externalDDT;
			recentlyUsedExternalElementPointer = other.recentlyUsedExternalElementPointer;
			recentlyUsedIndex = other.recentlyUsedIndex;
			count = other.count;
			countState = other.countState;
			return *this;
		}
#endif
		/*const_input_reference Find(element_pointer_unlogged data) const {
			external_iterator_unlogged begin = externalDDT.unlogged_begin();
			external_iterator_unlogged end = externalDDT.unlogged_end();
			for (external_iterator_unlogged it = begin; it != end; it++) {
				const_input_reference result = (*it).Find(object); //to be implemented to all DDTs
				if (result != NULL)
					return result;
			}
			return NULL;
		}*/
		void push_back(const_input_reference data) {
			external_iterator_unlogged begin = externalDDT.unlogged_begin();
			external_iterator_unlogged end = externalDDT.unlogged_end();
			int i = 0;
			for (external_iterator_unlogged it = begin; it != end; it++, i++) {
				if (countState[i] < COUNTLIMIT) {
					(*it).push_back(data);
					countState[i] += 1;
					++count;
					return;
				}
			}
			InternalDDT* temp_list = new InternalDDT;
			temp_list->push_back(data);
			externalDDT.push_back(*temp_list);
			countState.push_back(1);
			++count;
		}

		void pop_back() {
			erase(unlogged_last()); //needs optimization
		}

		reference operator[] (size_type offset) {
			size_type externalElementOffset = 0, internalElementOffset = 0;
			for (int i = 0; i < countState.size(); i++)
			{
				externalElementOffset += countState[i];
				if (externalElementOffset-1 >= offset)
					return dynamic_cast<InternalDDT&>(externalDDT[i]).operator[](offset - internalElementOffset);
				internalElementOffset = externalElementOffset;
			}
		}

		iterator erase(iterator& pos) {
			//First try the LRU inner DDT
			element_pointer_unlogged tmp = externalDDT.getLastElement()->getDataPointer()->getEnd();
			element_pointer_unlogged data = pos.getData();
			external_element_pointer_unlogged tmp2;
			bool found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);//needs optimization here
			if (found) {
				tmp = recentlyUsedExternalElementPointer->getDataPointer()->erase(data);
				countState[recentlyUsedIndex] -= 1;
				if ((countState[recentlyUsedIndex] == 0) && (externalDDT.size() > 1)) {
					externalDDT.erase(recentlyUsedExternalElementPointer);
					for (int i = recentlyUsedIndex; i < countState.size()-1; i++)
						countState[i] = countState[i+1];
				}
				count--;
				if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
					!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
					recentlyUsedExternalElementPointer = tmp2;
					recentlyUsedIndex++;
					tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
				}
			}
			//Else try the others
			else {
				external_iterator_unlogged begin = externalDDT.unlogged_begin();
				external_iterator_unlogged end = externalDDT.unlogged_end();
				recentlyUsedIndex = 0;
				for (external_iterator_unlogged it = begin; it != end; it++, recentlyUsedIndex++) {
					if ( it.getData() != recentlyUsedExternalElementPointer )  {
						recentlyUsedExternalElementPointer = it.getData();
						found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);//needs optimization here
						if (found) {
							tmp = recentlyUsedExternalElementPointer->getDataPointer()->erase(data);
							countState[recentlyUsedIndex] -= 1;
							if ((countState[recentlyUsedIndex] == 0) && (externalDDT.size() > 1)) {
								externalDDT.erase(recentlyUsedExternalElementPointer);
								for (int i = recentlyUsedIndex; i < countState.size()-1; i++)
									countState[i] = countState[i+1];
							}
							count--;
							if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
								!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
									recentlyUsedExternalElementPointer = tmp2;
									recentlyUsedIndex++;
									tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
							}
							break;
						}
					}
				}
			}

			return iterator(this, tmp, instance_id);
		}

		iterator_unlogged erase(const iterator_unlogged& pos) {
			//First try the LRU inner DDT
			element_pointer_unlogged tmp = externalDDT.getLastElement()->getDataPointer()->getEnd();
			element_pointer_unlogged data = pos.getData();
			external_element_pointer_unlogged tmp2;
			bool found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);//needs optimization here
			if (found) {
				tmp = recentlyUsedExternalElementPointer->getDataPointer()->erase(data);
				countState[recentlyUsedIndex] -= 1;
				if ((countState[recentlyUsedIndex] == 0) && (externalDDT.size() > 1)) {
					externalDDT.erase(recentlyUsedExternalElementPointer);
					for (int i = recentlyUsedIndex; i < countState.size()-1; i++)
						countState[i] = countState[i+1];
				}
				count--;
				if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
					!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
					recentlyUsedExternalElementPointer = tmp2;
					recentlyUsedIndex++;
					tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
				}
			}
			//Else try the others
			else {
				external_iterator_unlogged begin = externalDDT.unlogged_begin();
				external_iterator_unlogged end = externalDDT.unlogged_end();
				recentlyUsedIndex = 0;
				for (external_iterator_unlogged it = begin; it != end; it++, recentlyUsedIndex++) {
					if ( it.getData() != recentlyUsedExternalElementPointer )  {
						recentlyUsedExternalElementPointer = it.getData();
						found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);//needs optimization here
						if (found) {
							tmp = recentlyUsedExternalElementPointer->getDataPointer()->erase(data);
							countState[recentlyUsedIndex] -= 1;
							if ((countState[recentlyUsedIndex] == 0) && (externalDDT.size() > 1)) {
								externalDDT.erase(recentlyUsedExternalElementPointer);
								for (int i = recentlyUsedIndex; i < countState.size()-1; i++)
									countState[i] = countState[i+1];
							}
							count--;
							if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
								!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
									recentlyUsedExternalElementPointer = tmp2;
									recentlyUsedIndex++;
									tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
							}
							break;
						}
					}
				}
			}

			return iterator_unlogged(this, tmp, instance_id);
		}
		iterator insert(iterator& pos) {
			//Not Implemented Yet
			return end();
		}
		inline element_pointer_unlogged getFirstElement() {
			return externalDDT.getFirstElement()->getDataPointer()->getFirstElement();
		}
		inline element_pointer_unlogged getLastElement() {
			return externalDDT.getLastElement()->getDataPointer()->getLastElement();
		}
		inline element_pointer_unlogged getEnd() {
			return externalDDT.getLastElement()->getDataPointer()->getEnd();
		}
		inline element_pointer_unlogged getREnd() {
			return externalDDT.getFirstElement()->getDataPointer()->getREnd();
		}
		//Iterator interface
		element_pointer_unlogged decrement(element_pointer_unlogged data) {
			//external_reference recentInnerList = recentlyUsedExternalElementPointer->getData();
			//First try the LRU inner DDT
			element_pointer_unlogged tmp = getREnd();
			external_element_pointer_unlogged tmp2;
			bool found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
			if (found) {
				tmp = recentlyUsedExternalElementPointer->getDataPointer()->decrement(data);
				if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
					!externalDDT.IsNil(tmp2 = externalDDT.decrement(recentlyUsedExternalElementPointer))) {
					recentlyUsedExternalElementPointer = tmp2;
					recentlyUsedIndex--;
					tmp = recentlyUsedExternalElementPointer->getDataPointer()->getLastElement();//to check getLastElement()
				}
				//return tmp;
			}
			//Else try the others
			else {
				external_reverse_iterator_unlogged rbegin = externalDDT.unlogged_rbegin();
				external_reverse_iterator_unlogged rend = externalDDT.unlogged_rend();
				recentlyUsedIndex = countState.size()-1;
				for (external_reverse_iterator_unlogged it = rbegin; it != rend; it++, recentlyUsedIndex--) {
					if ( it.getData() != recentlyUsedExternalElementPointer )  {
						recentlyUsedExternalElementPointer = it.getData();
						found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
						if (found) {
							tmp = recentlyUsedExternalElementPointer->getDataPointer()->decrement(data);
							if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
								!externalDDT.IsNil(tmp2 = externalDDT.decrement(recentlyUsedExternalElementPointer))) {
									recentlyUsedExternalElementPointer = tmp2;
									recentlyUsedIndex--;
									tmp = recentlyUsedExternalElementPointer->getDataPointer()->getLastElement();//to check getLastElement()
							}
							return tmp;
						}
					}
				}
			}
			return tmp;
		}

		element_pointer_unlogged increment(element_pointer_unlogged data) {
			//external_reference recentInnerList = recentlyUsedExternalElementPointer->getData();
			//First try the LRU inner DDT
			element_pointer_unlogged tmp = getEnd();
			external_element_pointer_unlogged tmp2;
			bool found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
			if (found) {
				tmp = recentlyUsedExternalElementPointer->getDataPointer()->increment(data);
				if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
					!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
					recentlyUsedExternalElementPointer = tmp2;
					recentlyUsedIndex++;
					tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
				}
				//return tmp;
			}
			//Else try the others
			else {
				external_iterator_unlogged begin = externalDDT.unlogged_begin();
				external_iterator_unlogged end = externalDDT.unlogged_end();
				recentlyUsedIndex = 0;
				for (external_iterator_unlogged it = begin; it != end; it++, recentlyUsedIndex++) {
					if ( it.getData() != recentlyUsedExternalElementPointer )  {
						recentlyUsedExternalElementPointer = it.getData();
						found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
						if (found) {
							tmp = recentlyUsedExternalElementPointer->getDataPointer()->increment(data);
							if (recentlyUsedExternalElementPointer->getDataPointer()->IsNil(tmp) &&
								!externalDDT.IsNil(tmp2 = externalDDT.increment(recentlyUsedExternalElementPointer))) {
									recentlyUsedExternalElementPointer = tmp2;
									recentlyUsedIndex++;
									tmp = recentlyUsedExternalElementPointer->getDataPointer()->getFirstElement();
							}
							return tmp;
						}
					}
				}
			}
			return tmp;
		}
		inline pointer getData(element_pointer_unlogged data) {
			//return data->getDataPointer();
			pointer tmp = NULL;
			external_element_pointer_unlogged tmp2;
			bool found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
			if (found) {
				tmp = recentlyUsedExternalElementPointer->getDataPointer()->getData(data);
			}
			//Else try the others
			else {
				external_iterator_unlogged begin = externalDDT.unlogged_begin();
				external_iterator_unlogged end = externalDDT.unlogged_end();
				recentlyUsedIndex = 0;
				for (external_iterator_unlogged it = begin; it != end; it++, recentlyUsedIndex++) {
					if ( it.getData() != recentlyUsedExternalElementPointer )  {
						recentlyUsedExternalElementPointer = it.getData();
						found = recentlyUsedExternalElementPointer->getDataPointer()->exists(data);
						if (found)
							return recentlyUsedExternalElementPointer->getDataPointer()->getData(data);
					}
				}
			}
			return tmp;
		}

		////////////////////////////////////////////////////////////////////////
		// Basic iterators of DDT
		////////////////////////////////////////////////////////////////////////

		iterator begin() {
			return iterator(this, getFirstElement(), instance_id);
		}
		iterator end() {
			return iterator(this, getEnd(), instance_id);
		}

		/*const_iterator begin() const {
			return const_iterator(this, getFirstElement(), instance_id);
		}
		const_iterator end() const {
			return const_iterator(this, getEnd(), instance_id);
		}*/
		////////////////////////////////////////////////////////////////////////
		// Reverse iterators of DDT
		////////////////////////////////////////////////////////////////////////

		reverse_iterator rbegin() {
			return reverse_iterator(this, getLastElement(), instance_id);
		}
		reverse_iterator rend() {
			return reverse_iterator(this, getREnd(), instance_id);
		}
		/*const_reverse_iterator rend() const {
			return const_reverse_iterator(this, getLastElement(), instance_id);
		}
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this, getREnd(), instance_id);
		}	*/

		////////////////////////////////////////////////////////////////////////
		// Size information
		////////////////////////////////////////////////////////////////////////
		inline bool empty() const {
			return count == 0;
		}
		inline size_type size() {
			return count;
		}


		void resize(size_type new_size, const_input_reference value = input_value_type()) {
#ifdef LOGGING
			typename Logger::object_resize S(TYPEID(input_value_type), type_num, instance_id, new_size);
#endif
			const size_type old_size = count;
			if (new_size > old_size) {
				//reserve(new_size);
				for (size_type i = old_size; i < new_size; i++) {
					push_back(value);
				}
			} else if (new_size < old_size) {
				for (size_type i = new_size; i < old_size; i++) {
					pop_back();
				}
			}
		}

		inline size_type capacity() {
			return externalDDT.capacity() * COUNTLIMIT;//not really
		}

		void reserve(size_type new_capacity) {}

	protected:

		iterator_unlogged unlogged_begin() {
			iterator_unlogged it(this, getFirstElement(), instance_id);
			return it;
		}
		iterator_unlogged unlogged_last() {
			iterator_unlogged it(this, getREnd(), instance_id);
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


		
		ExternalDDT externalDDT;
		external_element_pointer_unlogged recentlyUsedExternalElementPointer;
		size_type recentlyUsedIndex;
		size_type count;
		std::vector<size_type> countState;
		const unsigned int instance_id;
		friend class GlueLayer;
	};

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif