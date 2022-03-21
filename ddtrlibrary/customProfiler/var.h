/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 * This is a smart wrapper that allows to easily check
 * all the accesses made to the datatype that has been wrapped.
 * Because this is a memory-level access-checker and not
 * DDT-accesschecker, it should be placed only around basic types
 * as it has not been designed for complicated types.  This is not
 * too bad as all complicated types are constructed from basic types.
 * (In the end memory is only occupied by chars, ints, floats, pointers,
 * etc...)
 *
 */

#ifndef TRACING_VAR_HPP
#define TRACING_VAR_HPP
#include "traits.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {

  //////////////////////////////////////////////////////////////////////////////
  // var<> template:
  //   Early definition for traits
  //////////////////////////////////////////////////////////////////////////////
template <typename T, typename Traits = no_traits>
    class var;
template <typename T, typename Traits = no_traits>
    class var_semilogged;
  //////////////////////////////////////////////////////////////////////////////
  // var_traits:
  //   Modifies a traits by redefining the value_type to be wrapped by 
  //   the var template
  //////////////////////////////////////////////////////////////////////////////
  template <class traits>
  struct var_traits {
    enum {type_num = traits::type_num };
    template <typename T>
      struct transform {
        typedef var<T, traits> value_type;
          typedef var_semilogged<T, traits> semilogged_value_type;
      };
    typedef typename traits::Logger Logger;
    typedef typename traits::Allocator Allocator;
  };


  //////////////////////////////////////////////////////////////////////////////
  // var<> template:
  //   Traces all accesses to a basic type
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename Traits>
    class var : public allocated<typename Traits::Allocator> {
      public:
        enum { type_num = Traits::type_num };
        typedef typename Traits::Logger Logger;
        typedef typename Traits::Allocator Allocator;

        // Special cases
        var() {};
        var(const var & other);
        var & operator= (const var & other);

        // Constructors
        template <typename T2> var(const T2 & data);
        template <typename T2, typename Traits2> var(const var<T2, Traits2> & other);

        // Accessors
        inline const T & getData() const;
        inline void setData(const T & data);
        inline operator const T &() const;

        // Direct Accessors (Unvar)
        inline const T & getUnvarData() const;
        inline void setUnvarData(const T & data);

        // Assignment operators from basic type
        template <typename T2> var & operator=   (const T2 & data);
        template <typename T2> var & operator+=  (const T2 & data);
        template <typename T2> var & operator-=  (const T2 & data);
        template <typename T2> var & operator*=  (const T2 & data);
        template <typename T2> var & operator/=  (const T2 & data);
        template <typename T2> var & operator%=  (const T2 & data);
        template <typename T2> var & operator^=  (const T2 & data);
        template <typename T2> var & operator&=  (const T2 & data);
        template <typename T2> var & operator|=  (const T2 & data);
        template <typename T2> var & operator<<= (const T2 & data);
        template <typename T2> var & operator>>= (const T2 & data);

        // Assignment operators from wrapped type
        template <typename T2, typename Traits2> var & operator=   (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator+=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator-=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator*=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator/=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator%=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator^=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator&=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator|=  (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator<<= (const var<T2, Traits2> & other);
        template <typename T2, typename Traits2> var & operator>>= (const var<T2, Traits2> & other);

        // Unary operators
        T operator+() const;
        T operator-() const;
        bool operator!() const;
        T operator~() const;
        var & operator++();
        var & operator--();
        T operator++(int);
        T operator--(int);
        const T & operator->() const;
        T & operator->();

        T data_;
    };

  // Special cases
  template <typename T, typename Traits>
    var<T, Traits>::var(const var & other) 
    : data_(other.getData()) {
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    }

  template <typename T, typename Traits> 
    var<T, Traits> & var<T, Traits>::operator= (const var & other) {
      Traits::Logger::log_read(&(other.data_), Traits::type_num, sizeof(T));
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      data_ = other.data_;
      return *this;
    }

  // Constructors
  template <typename T, typename Traits>
    template <typename T2>
    var<T, Traits>::var(const T2 & data) 
    : data_(data) {
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    }

  template <typename T, typename Traits>
    template <typename T2, typename Traits2>
    var<T, Traits>::var(const var<T2, Traits2> & other) 
    : data_(other.getData()) {
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    }

  // Accessors
  template <typename T, typename Traits>
    const T& var<T, Traits>::getData() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return data_;
    }

  template <typename T, typename Traits>
    void var<T, Traits>::setData(const T & data) {
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      data_ = data;
    }

  template <typename T, typename Traits>
    var<T, Traits>::operator const T &() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return data_;
    }

  // Direct Accessors (Unvar)
  template <typename T, typename Traits>
    const T& var<T, Traits>::getUnvarData() const {
      return data_;
    }

  template <typename T, typename Traits>
    void var<T, Traits>::setUnvarData(const T & data) {
      data_ = data;
    }

  // Assignment operators from basic type
  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator= (const T2 & data) {
      data_ = data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator+= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ += data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator-= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      data_ -= data;
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator*= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ *= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator/= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ /= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator%= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ %= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator^= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ ^= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator&= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ &= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator|= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ |= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator<<= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ <<= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2>
    var<T, Traits> & var<T, Traits>::operator>>= (const T2 & data) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      data_ >>= data;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  // Assignment operators from wrapped type
  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator= (const var<T2, Traits2> & other) {
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ = other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator+= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ += other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator-= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ -= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator*= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ *= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator/= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ /= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator%= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ %= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator^= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ ^= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator&= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ &= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator|= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ |= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator<<= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ <<= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits> 
    template <typename T2, typename Traits2>
    var<T, Traits> & var<T, Traits>::operator>>= (const var<T2, Traits2> & other) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
      data_ >>= other.data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  // Unary operators
  template <typename T, typename Traits> 
    T var<T, Traits>::operator+() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return +data_;
    }

  template <typename T, typename Traits> 
    T var<T, Traits>::operator-() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return -data_;
    }

  template <typename T, typename Traits> 
    bool var<T, Traits>::operator!() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return !data_;
    }

  template <typename T, typename Traits> 
    T var<T, Traits>::operator~() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return ~data_;
    }

  template <typename T, typename Traits>
    var<T, Traits> & var<T, Traits>::operator++() {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      ++data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits>
    var<T, Traits> & var<T, Traits>::operator--() {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      --data_;
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return *this;
    }

  template <typename T, typename Traits>
    T var<T, Traits>::operator++(int) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return data_++;
    }

  template <typename T, typename Traits>
    T var<T, Traits>::operator--(int) {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
      return data_--;
    }

  template <typename T, typename Traits>
    T & var<T, Traits>::operator->() {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return data_;
    }

  template <typename T, typename Traits>
    const T & var<T, Traits>::operator->() const {
      Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
      return data_;
    }


//////////////////////////////////////////////////////////////////////////////
// var<> template:
//   Traces all accesses to a basic type
//////////////////////////////////////////////////////////////////////////////
template <typename T, typename Traits>
  class var_semilogged : public allocated<typename Traits::Allocator> {
    public:
      enum { type_num = Traits::type_num };
      typedef typename Traits::Logger Logger;
      typedef typename Traits::Allocator Allocator;

      // Special cases
      var_semilogged() {};
      var_semilogged(const var_semilogged & other);
      var_semilogged & operator= (const var_semilogged & other);

      // Constructors
      template <typename T2> var_semilogged(const T2 & data);
      template <typename T2, typename Traits2> var_semilogged(const var_semilogged<T2, Traits2> & other);

      // Accessors
      inline const T & getData() const;
      inline void setData(const T & data);
      inline operator const T &() const;

      // Direct Accessors (Unvar)
      inline const T & getUnvarData() const;
      inline void setUnvarData(const T & data);

      // Assignment operators from basic type
      template <typename T2> var_semilogged & operator=   (const T2 & data);
      template <typename T2> var_semilogged & operator+=  (const T2 & data);
      template <typename T2> var_semilogged & operator-=  (const T2 & data);
      template <typename T2> var_semilogged & operator*=  (const T2 & data);
      template <typename T2> var_semilogged & operator/=  (const T2 & data);
      template <typename T2> var_semilogged & operator%=  (const T2 & data);
      template <typename T2> var_semilogged & operator^=  (const T2 & data);
      template <typename T2> var_semilogged & operator&=  (const T2 & data);
      template <typename T2> var_semilogged & operator|=  (const T2 & data);
      template <typename T2> var_semilogged & operator<<= (const T2 & data);
      template <typename T2> var_semilogged & operator>>= (const T2 & data);
      

      // Assignment operators from wrapped type
      template <typename T2, typename Traits2> var_semilogged & operator=   (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator+=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator-=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator*=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator/=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator%=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator^=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator&=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator|=  (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator<<= (const var_semilogged<T2, Traits2> & other);
      template <typename T2, typename Traits2> var_semilogged & operator>>= (const var_semilogged<T2, Traits2> & other);

      // Unary operators
      T operator+() const;
      T operator-() const;
      bool operator!() const;
      T operator~() const;
      var_semilogged & operator++();
      var_semilogged & operator--();
      T operator++(int);
      T operator--(int);
      const T & operator->() const;
      T & operator->();
      T& operator* ();

      T data_;
  };

// Special cases
template <typename T, typename Traits>
  var_semilogged<T, Traits>::var_semilogged(const var_semilogged & other)
  : data_(other.getData()) {
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
  }

template <typename T, typename Traits>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator= (const var_semilogged & other) {
    //Traits::Logger::log_read(&(other.data_), Traits::type_num, sizeof(T));
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    data_ = other.data_;
    return *this;
  }

// Constructors
template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits>::var_semilogged(const T2 & data)
  : data_(data) {
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits>::var_semilogged(const var_semilogged<T2, Traits2> & other)
  : data_(other.getData()) {
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
  }

// Accessors
template <typename T, typename Traits>
  const T& var_semilogged<T, Traits>::getData() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return data_;
  }

template <typename T, typename Traits>
  void var_semilogged<T, Traits>::setData(const T & data) {
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    data_ = data;
  }

template <typename T, typename Traits>
  var_semilogged<T, Traits>::operator const T &() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return data_;
  }

// Direct Accessors (Unvar)
template <typename T, typename Traits>
  const T& var_semilogged<T, Traits>::getUnvarData() const {
    return data_;
  }

template <typename T, typename Traits>
  void var_semilogged<T, Traits>::setUnvarData(const T & data) {
    data_ = data;
  }

// Assignment operators from basic type
template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator= (const T2 & data) {
    data_ = data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator+= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ += data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator-= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    data_ -= data;
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator*= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ *= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator/= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ /= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator%= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ %= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator^= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ ^= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator&= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ &= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator|= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ |= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator<<= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ <<= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator>>= (const T2 & data) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    data_ >>= data;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

// Assignment operators from wrapped type
template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator= (const var_semilogged<T2, Traits2> & other) {
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ = other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator+= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ += other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator-= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ -= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator*= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ *= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator/= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ /= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator%= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ %= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator^= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ ^= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator&= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ &= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator|= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ |= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator<<= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ <<= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  template <typename T2, typename Traits2>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator>>= (const var_semilogged<T2, Traits2> & other) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits2::Logger::log_read(&(other.data_), Traits2::type_num, sizeof(T2));
    data_ >>= other.data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

// Unary operators
template <typename T, typename Traits>
  T var_semilogged<T, Traits>::operator+() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return +data_;
  }

template <typename T, typename Traits>
  T var_semilogged<T, Traits>::operator-() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return -data_;
  }

template <typename T, typename Traits>
  bool var_semilogged<T, Traits>::operator!() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return !data_;
  }

template <typename T, typename Traits>
  T var_semilogged<T, Traits>::operator~() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return ~data_;
  }

template <typename T, typename Traits>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator++() {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    ++data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  var_semilogged<T, Traits> & var_semilogged<T, Traits>::operator--() {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    --data_;
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return *this;
  }

template <typename T, typename Traits>
  T var_semilogged<T, Traits>::operator++(int) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return data_++;
  }

template <typename T, typename Traits>
  T var_semilogged<T, Traits>::operator--(int) {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    //Traits::Logger::log_write(&(data_), Traits::type_num, sizeof(T));
    return data_--;
  }

template <typename T, typename Traits>
  T & var_semilogged<T, Traits>::operator->() {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return data_;
  }

template <typename T, typename Traits>
  const T & var_semilogged<T, Traits>::operator->() const {
    //Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
    return data_;
  }


    template <typename T, typename Traits>
    T& var_semilogged<T, Traits>::operator* () {
        Traits::Logger::log_read(&(data_), Traits::type_num, sizeof(T));
        return data_;
    }

}

////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
