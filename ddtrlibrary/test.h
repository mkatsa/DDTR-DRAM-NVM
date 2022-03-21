template <typename T, int I = 1>
struct ddt_helper {
	//List definitions
	typedef DDTLibrary::sllListElement<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> SLLELEMENT;
	typedef DDTLibrary::listBase<SLLELEMENT> SLLLISTBASE;
	typedef DDTLibrary::sll<SLLLISTBASE> SLLLISTTYPE;

	typedef DDTLibrary::List<SLLLISTTYPE> SLLLIST;
	typedef DDTLibrary::List<DDTLibrary::sllRovingAdapter<SLLLISTTYPE>> SLLROVINGLIST;

	//Vector definitions
	typedef DDTLibrary::Vector<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> VECTOR;

	//Glue Layer definitions
	typedef VECTOR INTERNALDDT;

	typedef DDTLibrary::sllListElement<INTERNALDDT, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> EXTERNALDDTELEMENT;
	typedef DDTLibrary::listBase<EXTERNALDDTELEMENT> EXTERNALDDTBASE;
	typedef DDTLibrary::sll<EXTERNALDDTBASE> EXTERNALDDTTYPE;

	typedef DDTLibrary::List<EXTERNALDDTTYPE> EXTERNALDDT;
	typedef DDTLibrary::GlueLayer<EXTERNALDDT, INTERNALDDT> GLUEDTYPE;

	//Types
	typedef GLUEDTYPE type;

};

int main()
{
	Tracing::DMMLogger::getInstance()->setLogFile("log.bin");
	Tracing::DMMLogger::getInstance()->setAlternateFile("log.map");
	
	typedef ddt_helper<int, 10>::type myGluedType;
	
	//Glue Layer Testing
	cout << endl << endl << "Glue Layer Testing" << endl;

	cout << endl << endl << "Testing push_back" << endl;
	myGluedType glue;
	for (int i = 0; i < 83; i++) {
		glue.push_back(i);
	}

	int k = 0;
	for (myGluedType::iterator i = glue.begin(); i != glue.end(); k++) {
		if (k / 10 == 1)
			i = glue.erase(i);
		else
			i++;
	}
	cout << endl<< endl << endl << "Testing operator[] After erase : " << endl;
	for (myGluedType::iterator i = glue.begin(); i != glue.end(); ++i) {
		cout << glue[i] << " ";
	}

}