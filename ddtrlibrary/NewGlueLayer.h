#ifndef NEW_GLUE_LAYER
#define NEW_GLUE_LAYER

#include "DDTIncludes.h"

namespace DDTLibrary {
	
	template <class ExternalDDT, class InternalDDT>
	class NewGlueLayer {
	public:
		
		typedef typename InternalDDT::const_input_reference const_input_reference;
		typedef typename InternalDDT::reference reference;
		typedef typename InternalDDT::size_type size_type;
		typedef typename InternalDDT::Logger Logger;
		typedef typename InternalDDT::input_value_type input_value_type;

		enum { type_num = InternalDDT::type_num };
		
		
		NewGlueLayer(): externalDDT(), countState(COUNTSTATE), count(0), instance_id(NewInstance::createId()) {
#ifdef LOGGING
			typename Logger::object_construct S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type));
#endif
				InternalDDT * ptr = new InternalDDT;
				externalDDT.push_back(*ptr);
			//recentlyUsedExternalElementPointer = externalDDT.getFirstElement();
			//recentlyUsedIndex = 0;
				countState.push_back(0);
		}
		
		void push_back(const_input_reference data) {
			for (int i = 0; i < countState.size(); i++)
			{
				if (countState[i] < COUNTLIMIT) 
				{
					externalDDT.get(i).push_back(data);
					countState[i] += 1;
					++count;
					return;
				}
			}
			
			InternalDDT* temp_ddt = new InternalDDT;
			temp_ddt->push_back(data);
			externalDDT.push_back(*temp_ddt);
			countState.push_back(1);
			++count;
		}
		
		reference operator[] (size_type offset) {
			size_type externalElementOffset = 0, internalElementOffset = 0;
			for (int i = 0; i < countState.size(); i++)
			{
				externalElementOffset += countState[i];
				if (externalElementOffset-1 >= offset)
					return externalDDT[i].operator[](offset - internalElementOffset);
				internalElementOffset = externalElementOffset;
			}
		}
		
		void printCountState () 
		{
			std::cout << "printing countState" << std::endl;
			for (int i = 0; i < countState.size(); i++)
			{
				std::cout << countState[i] << std::endl;
			}
			std::cout << "end of countState" << std::endl;

		}
		
		inline bool empty() const {
			return count == 0;
		}
		
		inline size_type size() {
			return count;
		}
		
	protected:
		ExternalDDT externalDDT;
		size_type count;
		std::vector<size_type> countState;
		const unsigned int instance_id;
		
	};
}
#endif