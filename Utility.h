#ifndef CORONA_UTILITY_H
#define CORONA_UTILITY_H


template<typename T>
class auto_array {
public:
  auto_array(T* initial = 0) {
    array = initial;
  }

  ~auto_array() {
    delete[] array;
  }

  T& operator[](int i) {
    return array[i];
  }

  operator T*() {
    return array;
  }

  T* release() {
    T* old = array;
    array = 0;
    return old;
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


#endif
