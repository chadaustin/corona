#ifndef CORONA_UTILITY_H
#define CORONA_UTILITY_H


template<typename T>
class auto_array {
public:
  explicit auto_array(T* initial = 0) {
    array = initial;
  }

  ~auto_array() {
    delete[] array;
  }

  operator T*() {
    return array;
  }

  T* release() {
    T* old = array;
    array = 0;
    return old;
  }

  auto_array<T>& operator=(T* a) {
    if (array != a) {
      delete array;
      array = a;
    }
    return *this;
  }

private:
  T* array;
};


// replace these with std::min when VC7 is out  :(
template<typename T>
T Min(T a, T b) {
  return a < b ? a : b;
}
template<typename T>
T Max(T a, T b) {
  return a > b ? a : b;
}


template<typename T>
class DefaultDelete : public T {

  // Deriving from DefaultDelete<T> instead of T shadows any interface-
  // defined |operator delete|, thus making implementation code easier to
  // write and read.

  // For example, SimpleImage's implementation of Image::destroy is simply
  // |delete this|, which, if SimpleImage inherited Image directly, would
  // infinitely recurse.  (Image::operator delete simply calls destroy())
  // This class "undefines" the overloaded operator delete by shadowing it.

  void operator delete(void* p) {
    ::operator delete(p);
  }
};


#endif
