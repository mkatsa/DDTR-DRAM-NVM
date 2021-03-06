#ifndef NEWINSTANCE_H
#define NEWINSTANCE_H
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {	
	///////////////////////////////////////////////////////////////
	// NewInstance - a different id to every instance of the 
	// data structure
	///////////////////////////////////////////////////////////////
	class NewInstance {
	public:
		static unsigned int createId() {
			static NewInstance instance;
			return instance.nextId();
		}
	private:
		NewInstance() {
			id_ = 0;
		}
		unsigned int nextId() {
			return id_++;
		}
		unsigned int id_;
	};

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif