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


#endif
