template <typename T, int I = 1>
struct ddt_helper {
	//List definitions
	typedef DDTLibrary::sllListElement<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> SLLELEMENT;
	typedef DDTLibrary::dllListElement<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> DLLELEMENT;
	typedef DDTLibrary::listBase<SLLELEMENT> SLLLISTBASE;
	typedef DDTLibrary::listBase<DLLELEMENT> DLLLISTBASE;
	typedef DDTLibrary::sll<SLLLISTBASE> SLLLISTTYPE;
	typedef DDTLibrary::dll<DLLLISTBASE> DLLLISTTYPE;

	typedef DDTLibrary::List<SLLLISTTYPE> SLLLIST;
	typedef DDTLibrary::List<DLLLISTTYPE> DLLLIST;
	typedef DDTLibrary::List<DDTLibrary::sllRovingAdapter<SLLLISTTYPE>> SLLROVINGLIST;
	typedef DDTLibrary::List<DDTLibrary::dllRovingAdapter<DLLLISTTYPE>> DLLROVINGLIST;

	//Vector definitions
	typedef DDTLibrary::Vector<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> VECTOR;

	//Tree definitions
	typedef DDTLibrary::BinaryTreeNode<T, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> BINARYTREENODE;
	typedef DDTLibrary::RedBlackTreeNodeAdapter<BINARYTREENODE> REDBLACKTREENODE;
	typedef DDTLibrary::BinaryTree<BINARYTREENODE> BINARYTREEBASE;
	typedef DDTLibrary::BinaryTree<REDBLACKTREENODE> REDBLACKTREE_BASE;
	typedef DDTLibrary::RedBlackTreeAdapter<REDBLACKTREE_BASE> REDBLACKTREEBASE;

	typedef DDTLibrary::Tree<BINARYTREEBASE> BINARYTREE;
	typedef DDTLibrary::Tree<REDBLACKTREEBASE> REDBLACKTREE;

	//Glue Layer definitions
	typedef DLLLIST INTERNALDDT;

	typedef DDTLibrary::sllListElement<INTERNALDDT, Tracing::std_traits, Tracing::var_traits<TRAITS(I)>> EXTERNALDDTELEMENT;
	typedef DDTLibrary::listBase<EXTERNALDDTELEMENT> EXTERNALDDTBASE;
	typedef DDTLibrary::sll<EXTERNALDDTBASE> EXTERNALDDTTYPE;

	typedef DDTLibrary::List<EXTERNALDDTTYPE> EXTERNALDDT;
	typedef DDTLibrary::GlueLayer<EXTERNALDDT, INTERNALDDT> GLUEDTYPE;

	//Types
	typedef GLUEDTYPE type;

};