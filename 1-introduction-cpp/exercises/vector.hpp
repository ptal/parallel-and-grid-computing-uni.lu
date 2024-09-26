template <class T>
class Vector {
  /* The number of elements currently in the array (as stored by the user). */
  size_t n;
  /* The capacity of the vector, which is the size of `values`. */
  size_t cap;
  T* values;

public:

  /* Default constructor */
  /* Copy constructor */
  /* Destructor */

  /* allocate an array of size `n` filled with 0. */
  Vector(size_t n);

  size_t capacity() const;
  size_t size() const;

  void resize(size_t n);
  void reserve(size_t n);

  /* Add the value `v` at the back of the vector, reallocating if necessary. */
  void push_back(const T& v);

  /* Remove the last element of the array.
     Precondition: `size() != 0`. */
  void pop_back();

  T& back();
  const T& back() const;

  T& front();
  const T& front() const;

  T& operator[](size_t i);
  const T& operator[](size_t i) const;
};
