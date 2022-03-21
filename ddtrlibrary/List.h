#ifndef LIST_H
#define LIST_H

#include "DDTIncludes.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {

    template <class listType> class List;

    template<class listType> class sllRovingExtension;
    template<class listType> class dllRovingExtension;

    template<class listBase> class dll;
    template<class listBase> class sll;

    template<class elementType> class listBase;

template<class T, class objectTraits = Tracing::no_traits, class ddtTraits = Tracing::no_traits> class dllListElement;
template<class T, class objectTraits = Tracing::no_traits, class ddtTraits = Tracing::no_traits> class sllListElement;

    ///////////////////////////////////////////////////////////////
    // List Elements
    ///////////////////////////////////////////////////////////////
    template<class T, class objectTraits, class ddtTraits>
    class dllListElement {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = ddtTraits::type_num };
        typedef typename ddtTraits::Logger Logger;
        typedef typename ddtTraits::Allocator Allocator;
        typedef typename objectTraits::template transform<T>::value_type value_type;
        typedef typename ddtTraits::template transform<T*>::value_type data_pointer;

        typedef typename ddtTraits::template transform<dllListElement*>::value_type element_pointer;
        typedef typename ddtTraits::template transform<dllListElement&>::value_type element_reference;

        //typedef typename ddtTraits::template transform<T*>::value_type value_pointer;

        typedef ddtTraits Traits;
        ////////////////////////////////////////////////////////////////////////
        typedef T input_value_type;
        typedef const input_value_type& const_input_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

        typedef dllListElement* element_pointer_unlogged;

        dllListElement(): data_(NULL) //data_((pointer) Allocator::malloc(sizeof(value_type))) //, next_(NULL), prev_(NULL)
        {
            next_ = element_pointer_unlogged(NULL);
            prev_ = element_pointer_unlogged(NULL);
            //Constructor(data_)  ;
            //next_.setData(NULL);
            //prev_.setData(NULL);
            //data_ = (pointer) Allocator::malloc(sizeof(value_type));
            //data_ = setData(NULL);
        }
        dllListElement(const_input_reference data) //:next_(NULL), prev_(NULL) {
        {
            //next_.setData(NULL);
            //prev_.setData(NULL);
            next_ = element_pointer_unlogged(NULL);
            prev_ = element_pointer_unlogged(NULL);
            //data_ = new T(data);
            data_ = (pointer) Allocator::malloc(sizeof(data));
            Constructor(data_, data);
            //data_ = &static_cast<value_type>(data);
        }

        ~dllListElement() {
            //cout << "~dllListElement()" << endl;
            if (data_ != NULL) {
                Destructor(data_);
                Allocator::free(data_);
            }
            
        }

        inline void initialize (element_pointer_unlogged node) {
            next_ = node;
            prev_ = node;
        }
        inline reference getData() {
            return *data_;
        }
        inline const_reference getData() const {
            return *data_;
        }

        inline pointer getDataPointer() {
            return data_;
        }
        inline void makeLink(element_pointer_unlogged prev, element_pointer_unlogged next) {
            if (prev)
            {
                prev->next_ = this;
                this->prev_ = prev;
            }
            else
            {
                this->prev_ = element_pointer_unlogged(NULL);
            }
            if (next)
            {
                this->next_ = next;
                next->prev_ = this;
            }
            else
            {
                this->next_ = element_pointer_unlogged(NULL);
            }
            
        }
        inline void removeLink(element_pointer_unlogged prev, element_pointer_unlogged next) {
            if (prev)
            {
                prev->next_ = next;
            }
            if (next)
            {
                next->prev_ = prev;
            }
            
        }

        void * operator new(const size_t sz)   { return Allocator::malloc(sz); }
        void operator delete(void * p)         { return Allocator::free(p); }
        void * operator new[](const size_t sz) { return Allocator::malloc(sz); }
        void operator delete[](void * p)       { return Allocator::free(p); }

        element_pointer next_;
        element_pointer prev_;
        //typename Traits::template transform<element_pointer>::value_type next_;
        //typename Traits::template transform<element_pointer>::value_type prev_;

        pointer data_;

    };

    template<class T, class objectTraits, class ddtTraits>
    class sllListElement {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = ddtTraits::type_num };
        typedef typename ddtTraits::Logger Logger;
        typedef typename ddtTraits::Allocator Allocator;
        typedef typename objectTraits::template transform<T>::value_type value_type;
        typedef typename ddtTraits::template transform<T*>::value_type data_pointer;
        typedef typename ddtTraits::template transform<sllListElement*>::value_type element_pointer;
        typedef typename ddtTraits::template transform<sllListElement&>::value_type element_reference;

        //typedef typename ddtTraits::template transform<T*>::value_type value_pointer;

        typedef ddtTraits Traits;
        ////////////////////////////////////////////////////////////////////////
        
        
        typedef T input_value_type;
        typedef const input_value_type& const_input_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef const data_pointer const_data_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

        //typedef typename objectTraits::template transform<sllListElement*>::value_type element_pointer_unlogged;
        typedef sllListElement* element_pointer_unlogged;

        sllListElement(): data_(NULL)//: data_((pointer) Allocator::malloc(sizeof(value_type)))
        {
            //Constructor(data_);
            next_ = element_pointer_unlogged(NULL);
            //next_.setData(NULL);
            //data_ = (pointer) Allocator::malloc(sizeof(value_type));
            //prev_.setData(NULL);
        }

        sllListElement(const_input_reference data) {//, data_(&data) {
            
            next_ = element_pointer_unlogged(NULL);
            
            //next_.setData(NULL);
            //prev_.setData(NULL);
            //data_ = data;
            data_ = (pointer) Allocator::malloc(sizeof(data));
            Constructor(data_, data);
            //data_ = new T(data);
            //data_ = &data;
        }
        ~sllListElement() {
            //std::cout << "~sllListElement()" << std::endl;
            if (data_ != NULL) {
                Destructor(data_);
                Allocator::free(data_);
            }
            
        }

        inline void initialize (element_pointer_unlogged node) {
            next_ = node;
        }

        inline void makeLink(element_pointer_unlogged prev, element_pointer_unlogged next) {
            prev->next_ = this;
            this->next_ = next;
        }
        inline void removeLink(element_pointer_unlogged prev, element_pointer_unlogged next) {
            prev->next_ = next;
        }
        inline reference getData() {
            return *data_;
        }
        inline const_reference getData() const {
            return *data_;
        }
        inline pointer getDataPointer() {
            return data_;
        }


        void * operator new(const size_t sz)   { return Allocator::malloc(sz); }
        void operator delete(void * p)         { return Allocator::free(p); }
        void * operator new[](const size_t sz) { return Allocator::malloc(sz); }
        void operator delete[](void * p)       { return Allocator::free(p); }

        
        
        element_pointer next_;
        //typename Traits::template transform<element_pointer>::value_type next_;
        pointer data_;

    };
    
    template<class T, class objectTraits, class ddtTraits>
    void swap(sllListElement<T, objectTraits, ddtTraits>& lhs, sllListElement<T, objectTraits, ddtTraits>& rhs)
    {
        std::cout << "swap(A&, A&)" << std::endl;
        using std::swap;
        swap(lhs.ptr_, rhs.ptr_);
    }

    // listBase without sentinel. Last element points to null
    template<class elementType>
    class listBaseNoSentinel {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = elementType::type_num };
        typedef typename elementType::Logger Logger;
        typedef typename elementType::Allocator Allocator;
        typedef typename elementType::value_type value_type;
        typedef typename elementType::Traits Traits;
        ////////////////////////////////////////////////////////////////////////
        typedef typename elementType::pointer pointer;
        typedef typename elementType::reference reference;
        typedef const typename elementType::const_reference const_reference;

        typedef elementType listElementType;

        typedef typename elementType::input_value_type input_value_type;
        typedef typename elementType::const_input_reference const_input_reference;

        typedef typename elementType::element_pointer_unlogged element_pointer_unlogged;

        typedef typename elementType::element_pointer element_pointer;
        typedef typename elementType::element_reference element_reference;

        typedef typename elementType::size_type size_type;
        typedef typename elementType::difference_type difference_type;

        listBaseNoSentinel (): instance_id_(NewInstance::createId()) {
#ifdef LOGGING
            typename Logger::object_construct S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type));
#endif
            _size = 0;
            head_ = element_pointer_unlogged(NULL);
            tail_ = element_pointer_unlogged(NULL);
        }
        ~listBaseNoSentinel() {
        }
        void initialize() {
            _size = 0;
        }
        inline element_pointer_unlogged getFirstElement() const {
            return head_;
        }
        inline element_pointer_unlogged getLastElement() const {
            return tail_;
        }
        inline element_pointer_unlogged getNextElement(element_pointer_unlogged data) const {
            return data->next_;
        }
        inline element_pointer_unlogged increment(element_pointer_unlogged data) const {
            return data->next_;
        }
        inline element_pointer_unlogged getEnd() const {
            return element_pointer_unlogged(NULL);
        }
        element_pointer_unlogged push_back(const_input_reference value) {
            element_pointer_unlogged tmp = new elementType(value);
            if (head_ == element_pointer_unlogged(NULL)) {
                head_ = tmp;
                //tmp->makeLink()
                //tmp->makeLink(sentinel, sentinel->next_);
            }
            else {
                //tmp->makeLink(tail_, element_pointer_unlogged(NULL));
                tmp->makeLink(tail_, element_pointer_unlogged(NULL));
            }
            tail_ = tmp;
            _size++;
            return tmp;
        }
        void insert_after(const_input_reference value, element_pointer_unlogged prev) {
            element_pointer_unlogged tmp = new elementType(value);
            tmp->makeLink(prev, prev->next_);
            if (tail_ == prev)
                tail_ = tmp;
            _size++;
        }
        void erase(element_pointer_unlogged prev, element_pointer_unlogged curr, element_pointer_unlogged next) {
            if (curr == tail_)
                tail_ = prev;
            if (curr == head_)
                head_ = next;
            curr->removeLink(prev, next);
            //delete curr;
            _size--;
        }
        //to be removed
        inline pointer getData(element_pointer_unlogged data) const {
            return data->getDataPointer();
        }
        inline bool empty() const {
            return (_size == 0);
        }
        inline size_type getSize() const {
            return _size;
        }
        inline bool IsNil(element_pointer_unlogged x) const {
            return x == NULL;
        }
        
        void resize(size_type n) {
            if (n > _size) { //increase size => append empty elements
                for (size_type i = 0; i < n - _size; i++) {
                    element_pointer_unlogged tmp = new elementType();
                    if (head_ == NULL) {
                        head_ = tmp;
                        //tmp->makeLink(sentinel, sentinel->next_);
                    }
                    else {
                        tmp->makeLink(tail_, NULL);
                    }
                    tail_ = tmp;
                }
            }
            else { //decrease size => remove elements from the end
                element_pointer_unlogged prev = head_;

                for (size_type i = 0; i < n - 1; i++)
                    prev = getNextElement(prev);

                element_pointer_unlogged curr = getNextElement(prev);
                element_pointer_unlogged next = getNextElement(curr);

                for (size_type i = 0; i < _size - n; i++) {
                    erase(prev, curr, next);
                    curr = getNextElement(prev);
                    next = getNextElement(curr);
                }
            }
            _size = n;
        }
        
    protected:
        size_type _size;
        const unsigned int instance_id_;
        element_pointer tail_, head_;
    };
    
    ///////////////////////////////////////////////////////////////
    // listBase defines a type of list. This type uses a sentinel.
    // Other types can also be defined and used.
    ///////////////////////////////////////////////////////////////
    template<class elementType>
    class listBase {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = elementType::type_num };
        typedef typename elementType::Logger Logger;
        typedef typename elementType::Allocator Allocator;
        typedef typename elementType::value_type value_type;
        typedef typename elementType::Traits Traits;
        ////////////////////////////////////////////////////////////////////////
        typedef typename elementType::pointer pointer;
        typedef typename elementType::reference reference;
        typedef const typename elementType::const_reference const_reference;

        typedef elementType listElementType;

        typedef typename elementType::input_value_type input_value_type;
        typedef typename elementType::const_input_reference const_input_reference;

        typedef typename elementType::element_pointer_unlogged element_pointer_unlogged;

        typedef typename elementType::element_pointer element_pointer;
        typedef typename elementType::element_reference element_reference;

        typedef typename elementType::size_type size_type;
        typedef typename elementType::difference_type difference_type;

        listBase (short dummy = 0 /*dummy parameter*/): instance_id_(NewInstance::createId()) {
#ifdef LOGGING
            typename Logger::object_construct S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type));
#endif
            _size = 0;
            sentinel = new elementType;
            sentinel->initialize(sentinel);
            //sentinel->data_ = 0;
            head_ = sentinel;
            tail_ = sentinel;
        }
        ~listBase() {
            delete sentinel;
            //Destructor(sentinel);
            //Allocator::free(sentinel);
        }
        void initialize() {
            _size = 0;
            sentinel->initialize(sentinel);
            head_ = sentinel;
            tail_ = sentinel;
        }
        inline element_pointer_unlogged getFirstElement() const {
            return head_;
        }
        inline element_pointer_unlogged getLastElement() const {
            return tail_;
        }
        inline element_pointer_unlogged getNextElement(element_pointer_unlogged data) const {
            return data->next_;
        }
        inline element_pointer_unlogged increment(element_pointer_unlogged data) const {
            return data->next_;
        }
        inline element_pointer_unlogged getEnd() const {
            return sentinel;
        }
        
        element_pointer_unlogged push_back(const_input_reference value) {
            element_pointer_unlogged tmp = new elementType(value);
            if (sentinel->next_ == sentinel) {
                head_ = tmp;
                tmp->makeLink(sentinel, sentinel->next_);
            }
            else {
                tmp->makeLink(tail_, sentinel);
            }
            tail_ = tmp;
            _size++;
            return tmp;
        }
        void insert_after(const_input_reference value, element_pointer_unlogged prev) {
            element_pointer_unlogged tmp = new elementType(value);
            tmp->makeLink(prev, prev->next_);
            if (tail_ == prev)
                tail_ = tmp;
            _size++;
        }
        void erase(element_pointer_unlogged prev, element_pointer_unlogged curr, element_pointer_unlogged next) {
            if (curr == tail_)
                tail_ = prev;
            if (curr == head_)
                head_ = next;
            curr->removeLink(prev, next);
            //delete curr;
            _size--;
        }
        //to be removed
        inline pointer getData(element_pointer_unlogged data) const {
            return data->getDataPointer();
        }
        inline bool empty() const {
            return (_size == 0);
        }
        inline size_type getSize() const {
            return _size;
        }
        inline bool IsNil(element_pointer_unlogged x) const {
            return x == sentinel;
        }

        void resize(size_type n) {
            if (n > _size) { //increase size => append empty elements
                for (size_type i = 0; i < n - _size; i++) {
                    element_pointer_unlogged tmp = new elementType();
                    if (sentinel->next_ == sentinel) {
                        head_ = tmp;
                        tmp->makeLink(sentinel, sentinel->next_);
                    }
                    else {
                        tmp->makeLink(tail_, sentinel);
                    }
                    tail_ = tmp;
                }
            }
            else { //decrease size => remove elements from the end
                element_pointer_unlogged prev = head_;

                for (size_type i = 0; i < n - 1; i++)
                    prev = getNextElement(prev);

                element_pointer_unlogged curr = getNextElement(prev);
                element_pointer_unlogged next = getNextElement(curr);

                for (size_type i = 0; i < _size - n; i++) {
                    erase(prev, curr, next);
                    curr = getNextElement(prev);
                    next = getNextElement(curr);
                }
            }
            _size = n;
        }
        ////////////////////////////////////////////////////////////////////////
        // Glue Layer Integration
        ////////////////////////////////////////////////////////////////////////
        inline element_pointer_unlogged getREnd() {
            return sentinel;
        }
#ifdef FACE_STUPID_ERROR
        listBase& operator=(const listBase& other) {
            _size = other._size;
            tail_ = other.tail_;
            head_ = other.head_;
            sentinel = other.sentinel;
            return *this;
        }
#endif
    protected:
        size_type _size;
        const unsigned int instance_id_;
        element_pointer tail_, head_;
        element_pointer sentinel;
    };
    //////////////////////////////////////////////////////////////////
    // sll and dll are list interfaces that define getPreviousElement()
    // and get() functions
    //////////////////////////////////////////////////////////////////
    template<class listBase>
    class sll: public listBase {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = listBase::type_num };
        typedef typename listBase::Logger Logger;
        typedef typename listBase::Allocator Allocator;
        typedef typename listBase::value_type value_type;
        typedef typename listBase::Traits Traits;
        ////////////////////////////////////////////////////////////////////////
        typedef typename listBase::input_value_type input_value_type;
        typedef typename listBase::pointer pointer;
        typedef typename listBase::reference reference;
        typedef typename listBase::const_reference const_reference;
        typedef typename listBase::const_input_reference const_input_reference;

        //typedef typename listBase::listElementType elementType;
        typedef typename listBase::element_pointer element_pointer;
        typedef typename listBase::element_reference element_reference;

        typedef typename listBase::element_pointer_unlogged element_pointer_unlogged;

       

        typedef typename listBase::size_type size_type;
        typedef typename listBase::difference_type difference_type;
        
        using listBase::head_;

        sll (){}
        element_pointer_unlogged getPreviousElement(element_pointer_unlogged curr) {
            element_pointer_unlogged tmp = listBase::head_;
            while (!this->IsNil(tmp) && tmp->next_ != curr)
                tmp = tmp->next_;
            return tmp;
        }
        element_pointer_unlogged decrement(element_pointer_unlogged data) const {
            element_pointer_unlogged tmp = head_;
            while (!this->IsNil(tmp) && tmp->next_ != data)
                tmp = tmp->next_;
            return tmp;
        }
        reference get(size_type n) {
            size_type i = 0;
            element_pointer_unlogged tmp = head_;
            while (i < n) {
                tmp = tmp->next_;
                i++;
            }
            return tmp->getData();
        }
        const_reference get(size_type n) const {
            size_type i = 0;
            element_pointer_unlogged tmp = head_;
            while (i < n) {
                tmp = tmp->next_;
                i++;
            }
            return tmp->getData();
        }
        element_pointer_unlogged getElement(size_type n) {
            size_type i = 0;
            element_pointer_unlogged tmp = head_;
            while (i < n) {
                tmp = tmp->next_;
                i++;
            }
            return tmp;
        }
    };

    template<class listBase>
    class dll: public listBase {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = listBase::type_num };
        typedef typename listBase::Logger Logger;
        typedef typename listBase::Allocator Allocator;
        typedef typename listBase::value_type value_type;
        typedef typename listBase::Traits Traits;
        ////////////////////////////////////////////////////////////////////////
        typedef typename listBase::pointer pointer;
        typedef typename listBase::reference reference;
        typedef typename listBase::const_reference const_reference;

        typedef typename listBase::element_pointer element_pointer;
        typedef typename listBase::element_reference element_reference;

        typedef typename listBase::element_pointer_unlogged element_pointer_unlogged;

        typedef typename listBase::input_value_type input_value_type;

        typedef typename listBase::size_type size_type;
        typedef typename listBase::difference_type difference_type;

        using listBase::_size;
        using listBase::head_;
        using listBase::tail_;

        dll () {}

        element_pointer_unlogged getPreviousElement(element_pointer_unlogged curr) {
            return curr->prev_;
        }
        element_pointer_unlogged decrement(element_pointer_unlogged data) const {
            return data->prev_;
        }
        reference get(size_type n) {
            element_pointer_unlogged tmp;
            size_type i = 0;

            if (n <= _size/2) {
                tmp = head_;
                while (i < n) {
                    tmp = tmp->next_;
                    ++i;
                }
            }
            else {
                tmp = tail_;
                size_type newOffset = _size - n - 1;
                while (i < newOffset) {
                    tmp = tmp->prev_;
                    ++i;
                }
            }
            return tmp->getData();
        }
        const_reference get(size_type n) const {
            element_pointer_unlogged tmp;
            size_type i = 0;

            if (n <= _size/2) {
                tmp = head_;
                while (i < n) {
                    tmp = tmp->next_;
                    ++i;
                }
            }
            else {
                tmp = tail_;
                size_type newOffset = _size - n - 1;
                while (i < newOffset) {
                    tmp = tmp->prev_;
                    ++i;
                }
            }
            return tmp->getData();
        }
        
        element_pointer_unlogged getElement(size_type n) {
            element_pointer_unlogged tmp;
            size_type i = 0;

            if (n <= _size/2) {
                tmp = head_;
                while (i < n) {
                    tmp = tmp->next_;
                    ++i;
                }
            }
            else {
                tmp = tail_;
                size_type newOffset = _size - n - 1;
                while (i < newOffset) {
                    tmp = tmp->prev_;
                    ++i;
                }
            }
            return tmp;
        }
    };

    ///////////////////////////////////////////////////////////////
    // Extensions. These classes add functionality to the list. Some
    // of them, like sllBiIteratorAdapter are used to handle the new
    // functionality introduced in specific iterators like sllBiIterator.
    ///////////////////////////////////////////////////////////////
    template<class listType>
    class sllRovingExtension: public listType {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = listType::type_num };
        typedef typename listType::Logger Logger;
        typedef typename listType::Allocator Allocator;
        typedef typename listType::value_type value_type;
        ////////////////////////////////////////////////////////////////////////
        typedef typename listType::pointer pointer;
        typedef typename listType::reference reference;
        typedef typename listType::const_reference const_reference;

        //typedef typename listType::elementType elementType;
        typedef typename listType::element_pointer element_pointer;
        typedef typename listType::element_reference element_reference;

        typedef typename listType::element_pointer_unlogged element_pointer_unlogged;

        typedef typename listType::input_value_type input_value_type;

        typedef typename listType::size_type size_type;
        typedef typename listType::difference_type difference_type;

        sllRovingExtension()
            : roving(listType::getFirstElement()), rovingOffset(0) {}

        void erase(element_pointer_unlogged prev_node, element_pointer_unlogged curr, element_pointer_unlogged next_node) {
            if (roving == curr) {
                listType::erase(prev_node, curr, next_node);
                roving = listType::getFirstElement();
                rovingOffset = 0;
            }
            else
                listType::erase(prev_node, curr, next_node);
        }

        reference get(size_type n)  {
            size_type i = rovingOffset;
            element_pointer_unlogged tmp = roving; //ORIGINAL .this line will be removed. the roving pointer
                                                    //will be assigned only when we are sure that we don't go back to the header.
                                                    //If we assign the roving to tmp and then assign header to tmp we lose one access in vain
            //element_pointer_unlogged tmp;             // OPTIMIZATION . we don't initiallize unless we know we need it
            //check if roving has not been initialized before. initially it points to the sentinel
            //but we want it to point to the first object
            if (this->IsNil(tmp)) {
                tmp = listType::getFirstElement();
                rovingOffset = 0;
                roving = tmp;
            }
            if (i != n) {
                if (i < n) {
                    //element_pointer_unlogged tmp = roving; //OPTIMIZATION .moved this line here
                    while (i != n) {
                        tmp = tmp->next_;
                        i++;
                    }
                }
                else {
                    tmp = listType::getFirstElement();
                    i = 0;
                    while (i != n) {
                        tmp = tmp->next_;
                        i++;
                    }
                }
                rovingOffset = n;
                roving = tmp;
                //return roving->getData(); //ORIGINAL
                return tmp->getData(); //OPTIMIZATION
            }
            else {
                return tmp->getData();
            }
        }

        const_reference get(size_type n) const {
            size_type i = rovingOffset;
            element_pointer_unlogged tmp = roving; //ORIGINAL .this line will be removed. the roving pointer
                                                    //will be assigned only when we are sure that we don't go back to the header.
                                                    //If we assign the roving to tmp and then assign header to tmp we lose one access in vain
            //element_pointer_unlogged tmp;             // OPTIMIZATION . we don't initiallize unless we know we need it
            //check if roving has not been initialized before. initially it points to the sentinel
            //but we want it to point to the first object
            if (this->IsNil(tmp)) {
                tmp = listType::getFirstElement();
                const_cast<sllRovingExtension*>(this)->rovingOffset = 0;
                const_cast<sllRovingExtension*>(this)->roving = tmp;
            }
            if (i != n) {
                if (i < n) {
                    //element_pointer_unlogged tmp = roving; //OPTIMIZATION .moved this line here
                    while (i != n) {
                        tmp = tmp->next_;
                        i++;
                    }
                }
                else {
                    tmp = listType::getFirstElement();
                    i = 0;
                    while (i != n) {
                        tmp = tmp->next_;
                        i++;
                    }
                }
                const_cast<sllRovingExtension*>(this)->rovingOffset = n;
                const_cast<sllRovingExtension*>(this)->roving = tmp;
                //return roving->getData(); //ORIGINAL
                return tmp->getData(); //OPTIMIZATION
            }
            else {
                return tmp->getData();
            }
        }
        

        element_pointer roving;
        size_type rovingOffset;
    };

    template<class listType>
    class dllRovingExtension: public listType {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = listType::type_num };
        typedef typename listType::Logger Logger;
        typedef typename listType::Allocator Allocator;
        typedef typename listType::value_type value_type;
        ////////////////////////////////////////////////////////////////////////
        typedef typename listType::pointer pointer;
        typedef typename listType::reference reference;
        typedef typename listType::const_reference const_reference;

        //typedef typename listType::elementType elementType;
        typedef typename listType::element_pointer element_pointer;
        typedef typename listType::element_reference element_reference;

        typedef typename listType::element_pointer_unlogged element_pointer_unlogged;

        typedef typename listType::input_value_type input_value_type;

        typedef typename listType::size_type size_type;
        typedef typename listType::difference_type difference_type;

        dllRovingExtension()
            : roving(listType::getFirstElement()), rovingOffset(0) {}

        void erase(element_pointer_unlogged prev_node, element_pointer_unlogged curr, element_pointer_unlogged next_node) {
            if (roving == curr) {
                listType::erase(prev_node, curr, next_node);
                roving = listType::getFirstElement();
                rovingOffset = 0;
            }
            else
                listType::erase(prev_node, curr, next_node);
        }

        reference get(size_type n) {
            size_type i = rovingOffset;
            size_type newOffset = 0;
            size_type numberofObjects = listType::getSize();
            element_pointer_unlogged tmp = roving;


            if (this->IsNil(tmp)) {
                tmp = listType::getFirstElement();
                rovingOffset = 0;
                roving = tmp;
            }
            if (i != n) {
                if (i < n) {
                    if (n - i <= numberofObjects - n) {
                        newOffset = n - i;
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->next_;
                            i++;
                        }
                    }
                    else
                    {
                        newOffset = numberofObjects - n - 1;
                        tmp = listType::getLastElement();
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->prev_;
                            i++;
                        }
                    }
                }
                else {
                    if (n <= i/2)
                    {
                        newOffset = n;
                        tmp = listType::getFirstElement();
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->next_;
                            i++;
                        }
                    }
                    else {
                        newOffset = i - n;
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->prev_;
                            i++;
                        }
                    }
                }
                rovingOffset = n;
                roving = tmp;
                return tmp->getData();
            }
            else {
                return tmp->getData();
            }
        }

        const_reference get(size_type n) const {
            size_type i = rovingOffset;
            size_type newOffset = 0;
            size_type numberofObjects = listType::getSize();
            element_pointer_unlogged tmp = roving;


            if (this->IsNil(tmp)) {
                tmp = listType::getFirstElement();
                const_cast<dllRovingExtension*>(this)->rovingOffset = 0;
                const_cast<dllRovingExtension*>(this)->roving = tmp;
            }
            if (i != n) {
                if (i < n) {
                    if (n - i <= numberofObjects - n) {
                        newOffset = n - i;
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->next_;
                            i++;
                        }
                    }
                    else
                    {
                        newOffset = numberofObjects - n - 1;
                        tmp = listType::getLastElement();
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->prev_;
                            i++;
                        }
                    }
                }
                else {
                    if (n <= i/2)
                    {
                        newOffset = n;
                        tmp = listType::getFirstElement();
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->next_;
                            i++;
                        }
                    }
                    else {
                        newOffset = i - n;
                        i = 0;
                        while (i < newOffset) {
                            tmp = tmp->prev_;
                            i++;
                        }
                    }
                }
                const_cast<dllRovingExtension*>(this)->rovingOffset = n;
                const_cast<dllRovingExtension*>(this)->roving = tmp;
                return tmp->getData();
            }
            else {
                return tmp->getData();
            }
        }

        element_pointer roving;
        size_type rovingOffset;
    };

    ///////////////////////////////////////////////////////////////
    // This is the main handler for the list abstract data type
    ///////////////////////////////////////////////////////////////
    template <class listType>
    class List: public listType {
    public:
        ////////////////////////////////////////////////////////////////////////
        enum { type_num = listType::type_num };
        typedef typename listType::Logger Logger;
        typedef typename listType::Allocator Allocator;
        typedef typename listType::value_type value_type;
        ////////////////////////////////////////////////////////////////////////
        typedef typename listType::pointer pointer;
        typedef typename listType::reference reference;
        typedef typename listType::const_reference const_reference;
        typedef typename listType::const_input_reference const_input_reference;
        
        typedef typename listType::element_pointer element_pointer;
        typedef typename listType::element_reference element_reference;

        typedef typename listType::element_pointer_unlogged element_pointer_unlogged;

        typedef typename listType::input_value_type input_value_type;
        

        typedef typename listType::size_type size_type;
        typedef typename listType::difference_type difference_type;

        typedef Iterator_Base<List> iterator;
        typedef ConstIteratorAdapter<iterator> const_iterator;

        typedef ReverseIteratorAdapter<iterator> reverse_iterator;
        typedef ReverseIteratorAdapter<const_iterator> const_reverse_iterator;

#ifndef MORE_LOGGING
        typedef Iterator_Base_Unlogged<List> iterator_unlogged;
        typedef ReverseIteratorAdapter<iterator_unlogged> reverse_iterator_unlogged;
#else
        typedef iterator iterator_unlogged;
        typedef reverse_iterator reverse_iterator_unlogged;
#endif
        using listType::instance_id_;

        //typedef std::vector<List *> inner_vector;

        List() {
            //refers.push_back(this);
        }
        List(size_type sz) {
            resize(sz);
        }
        List(size_type sz, const_input_reference value) {
            resize(sz, value);
        }
        List(iterator_unlogged first, iterator_unlogged last) {
            for (; first != last; first++) {
                push_back(*first);
            }
        }

        void clear() {

            //Mou phre mia mera na syneidhtopoihsw ti shmainei referenced
            //values, garbage collector. H C++ einai low level glwssa san th C.

            /*if (refers.size() == 0) {
                element_pointer_unlogged prev_node = listType::getEnd();
                element_pointer_unlogged curr = prev_node->next_;
                element_pointer_unlogged next_node = curr->next_;
                while (prev_node != curr) {
                    listType::erase(prev_node, curr, next_node);
                    delete curr;
                    curr = next_node;
                    next_node = curr->next_;
                }
            }*/

            if (!listType::empty()) {
                //iterator_unlogged it1(this, listType::getFirstElement(), instance_id_);
                //iterator_unlogged it2(this, listType::getEnd(), instance_id_);
                //DestructAndDelete(it1, it2);
                element_pointer_unlogged ePointer = listType::getFirstElement();
                element_pointer_unlogged endPointer = listType::getEnd();
                while (ePointer != endPointer) {
                    element_pointer_unlogged toDelete = ePointer;
                    ePointer = ePointer->next_;
                    delete toDelete;
                }
                
                
                //Destructor2(it1, it2);
                
            }
            listType::initialize();
        }
        
        
        
        template <class listType2>
        List(const List<listType2>& other)
            : listType()
        {

            iterator_unlogged it1(other.unlogged_begin());
            //iterator_unlogged it3(this, other.getFirstElement(), instance_id_);
            iterator_unlogged it2(this, other.getFirstElement(), instance_id_);
            while (it1 != it2) {
                listType::push_back(*it1);
                ++it1;
            }
        }

        ~List() {
#ifdef LOGGING
            typename Logger::object_destruct S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type));
#endif
            //inner_vector::iterator del;
            //for(vector<List *>::iterator it = refers.begin(); it != refers.end(); it++) {
            //    if (*it == this) {
            //        refers.erase(it);
            //        break;
            //    }
            //}
            clear();
            //also remove the sentinel

            //delete listType::sentinel;
        }

        ////////////////////////////////////////////////////////////////////////
        // Basic iterators of DDT
        ////////////////////////////////////////////////////////////////////////

        iterator_unlogged begin() const {
            return iterator_unlogged(this, listType::getFirstElement(), instance_id_);
        }
        iterator_unlogged end() const {
            return iterator_unlogged(this, listType::getEnd(), instance_id_);
        }

        /*const_iterator begin() const {
            return const_iterator(this, listType::getFirstElement(), instance_id_);
        }
        const_iterator end() const {
            return const_iterator(this, listType::getEnd(), instance_id_);
        }*/
        ////////////////////////////////////////////////////////////////////////
        // Reverse iterators of DDT
        ////////////////////////////////////////////////////////////////////////

        reverse_iterator_unlogged rbegin() {
            return reverse_iterator_unlogged(this, listType::getLastElement(), instance_id_);
        }
        reverse_iterator_unlogged rend() {
            return reverse_iterator_unlogged(this, listType::getEnd(), instance_id_);
        }
        /*const_reverse_iterator rend() const {
            return const_reverse_iterator(this, listType::getEnd(), instance_id_);
        }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(this, listType::getLastElement(), instance_id_);
        }    */

        ////////////////////////////////////////////////////////////////////////
        // Size information
        ////////////////////////////////////////////////////////////////////////
        inline bool empty() const {
            return listType::empty();
        }
        inline size_type size() const {
            return listType::getSize();
        }

        //Resize Version 1
        /*void resize(size_type new_size) {
#ifdef LOGGING
            typename Logger::object_resize S(TYPEID(input_value_type), type_num, instance_id_, new_size);
#endif
            if (new_size > size())
                listType::resize(new_size);
        }*/


        //Resize Version 2
        void resize(size_type new_size, const_input_reference value = input_value_type()) {
#ifdef LOGGING
            typename Logger::object_resize S(TYPEID(input_value_type), type_num, instance_id_, new_size);
#endif
            const size_type old_size = size();
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
            return listType::getSize();
        }

        void reserve(size_type new_capacity) {}

        reference operator[](size_type n) {
#ifdef LOGGING
            //typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                //sizeof(input_value_type), n);
#endif
            return listType::get(n);
        }
        const_reference operator[](size_type n) const {
           return listType::get(n);
        }
        reference get(size_type n)  {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), n);
#endif
            //bool found = false;

            //for(inner_vector::iterator it = refers.begin(); it != refers.end(); it++) {
            //    if (*it == this) {
            //        found = true;
            //        break;
            //    }
            //}
            //if (!found)
            //    refers.push_back(this);

            return listType::get(n);
        }
        //const_reference get(size_type n) const {
        //    #ifdef LOGGING
        //        typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
        //                                      sizeof(input_value_type), n);
        //    #endif
        //    return listType::get(n);
        //}
        
        element_pointer_unlogged getElement(size_type n) {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), n);
#endif
            return listType::getElement(n);
        }
        
        const_reference front() const {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), 0);
#endif
            return listType::getFirstElement()->getData();
        }
        reference front() {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), 0);
#endif
            return listType::getFirstElement()->getData();
        }
        const_reference back() const {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), listType::getSize());
#endif
            return listType::getLastElement()->getData();
        }
        reference back() {
#ifdef LOGGING
            typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
                sizeof(input_value_type), listType::getSize());
#endif
            return listType::getLastElement()->getData();
        }
        
        void push_back(const_input_reference value) {
#ifdef LOGGING
            //typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            listType::push_back(value);
        }
        void pop_back() {
#ifdef LOGGING
            //typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            element_pointer_unlogged curr = listType::getLastElement();
            if (!(this->IsNil(curr))) {
                element_pointer_unlogged prev_node = listType::getPreviousElement(curr);
                element_pointer_unlogged next_node = listType::getEnd();
                listType::erase(prev_node, curr, next_node);
                //delete curr->getDataPointer();
                //Destructor(curr->getDataPointer());
                //Destructor(curr);
                delete curr;
                
            }
        }
        void pop_front() {
            element_pointer_unlogged curr = listType::getFirstElement();

            if (!(this->IsNil(curr))) {
                element_pointer_unlogged prev_node = listType::getEnd();
                element_pointer_unlogged next_node = listType::getNextElement(curr);
                listType::erase(prev_node, curr, next_node);
                //Destructor(curr->getDataPointer());
                //Destructor(curr);
                delete curr;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Glue Layer Integration
        ////////////////////////////////////////////////////////////////////////
        bool exists(element_pointer_unlogged x) {
            if (!listType::empty()) {
                iterator_unlogged it1(this, listType::getFirstElement(), instance_id_);
                iterator_unlogged it2(this, listType::getEnd(), instance_id_);
                for (iterator_unlogged it = it1; it != it2; it++) {
                    if (it.getData() == x)
                        return true;
                }
            }
            return false;
        }
        
        // Inserts after the current position indexed by the iterator
        void insert_after(const_input_reference value, iterator_unlogged& pos)
        {
#ifdef LOGGING
            typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            element_pointer_unlogged curr = pos.getData();
            //element_pointer_unlogged prev_node = listType::getPreviousElement(curr);
            if (!(this->IsNil(curr))) {
                //element_pointer_unlogged next_node = curr->next_;
                listType::insert_after(value, curr);
            }
            
        }
        void insert_after(const_input_reference value, element_pointer_unlogged item) {
#ifdef LOGGING
            typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            if (!(this->IsNil(item))) {
                listType::insert_after(value, item);
            }
        }
        iterator_unlogged erase(iterator_unlogged pos) {
#ifdef LOGGING
            //typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            element_pointer_unlogged curr = pos.getData();
            element_pointer_unlogged next_node = curr->next_;
            if (!(this->IsNil(curr))) {
                element_pointer_unlogged prev_node = listType::getPreviousElement(curr);
                listType::erase(prev_node, curr, next_node);
                //Destructor(curr->getDataPointer());
                //Destructor(curr);
                delete curr;
            }

            return iterator_unlogged(this, next_node, instance_id_);
        }
        
        reverse_iterator_unlogged erase(reverse_iterator_unlogged pos) {
#ifdef LOGGING
            typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            element_pointer_unlogged curr = pos.getData();
            element_pointer_unlogged prev_node = listType::getPreviousElement(curr);
            if (!(this->IsNil(curr))) {
                element_pointer_unlogged next_node = curr->next_;
                listType::erase(prev_node, curr, next_node);
                //Destructor(curr->getDataPointer());
                //Destructor(curr);
                delete curr;
            }

            return reverse_iterator_unlogged(this, prev_node, instance_id_);
        }
        /*void erase(element_pointer_unlogged item)
        {
            element_pointer_unlogged prev_node = listType::getPreviousElement(item);
            if (!(this->IsNil(curr))) {
                element_pointer_unlogged next_node = item->next_;
                listType::erase(prev_node, item, next_node);
                Destructor(item->getDataPointer());
            }
        }*/
        element_pointer_unlogged erase(element_pointer_unlogged item) {
#ifdef LOGGING
            //typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
            element_pointer_unlogged curr = item;
            element_pointer_unlogged next_node = curr->next_;
            if (!(this->IsNil(curr))) {
                element_pointer_unlogged prev_node = listType::getPreviousElement(curr);
                //Destructor(curr->getDataPointer());
                listType::erase(prev_node, curr, next_node);
                //Destructor(curr);
                delete curr;
            }

            return next_node;
        }
        
        iterator_unlogged erase(iterator_unlogged first, iterator_unlogged last) {
            if (first != last) {
                //if (last != end()) {
                    element_pointer_unlogged firstNode = first.getData();
                    element_pointer_unlogged lastNode = last.getData();
                    element_pointer_unlogged next_node = lastNode;
                    
                    //if (firstNode) {
                        element_pointer_unlogged prev_node = listType::getPreviousElement(firstNode);
                        listType::erase(prev_node, firstNode, next_node);
                        
                        //destruct and delete all the removed elements
                        while (firstNode != lastNode) {
                            element_pointer_unlogged currNode = firstNode;
                            firstNode = firstNode->next_;
                            //Destructor(firstNode->getDataPointer());
                            //Destructor(currNode);
                            delete currNode;
                        }
                        
                    //}

                    
                //}
            }
            return last;
        }
        
        void
        printSentinel() {
            std::cout << "sentinel = " << this->sentinel
            << " " << "sentinel->data_->index = " << this->sentinel->data_->index
            << " " << "sentinel->data_->f1 = " << this->sentinel->data_->f1
            << " " << "sentinel->data_->f2 = " << this->sentinel->data_->f2
                << " " << std::endl;
        }
        
        void swap(List& lhs, List& rhs)
        {
            std::cout << "swap(A&, A&)" << std::endl;
            using std::swap;
            swap(lhs.ptr_, rhs.ptr_);
        }
        
    private:

        
        
        
        iterator_unlogged unlogged_begin() const {
            iterator_unlogged it(this, listType::getFirstElement(), instance_id_);
            return it;
        }
        iterator_unlogged unlogged_end() const {
            iterator_unlogged it(this, listType::getEnd(), instance_id_);
            return it;
        }

        reverse_iterator_unlogged unlogged_rbegin() const {
            return reverse_iterator_unlogged(this, listType::getLastElement(), instance_id_);
        }
        reverse_iterator_unlogged unlogged_rend() const {
            return reverse_iterator_unlogged(this, listType::getEnd(), instance_id_);
        }
        
        

        //inner_vector refers;

        template <class> friend class List;
        template <class, class> friend class GlueLayer;

    };

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif

