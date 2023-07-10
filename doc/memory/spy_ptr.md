The `spy_ptr` class provides a smart pointer implementation for tracking and monitoring of the managed objects, without affecting the reference count. The class provides constructors, comparison operators, and member functions for efficient and safe handling of objects.

## Template Parameter

- `T`: The type of the object being managed by the smart pointer.

## Constructors and Destructor

### `spy_ptr()`

- Description: Constructs an instance of the `spy_ptr` class with a null pointer.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr;
  ```

### `spy_ptr(const spy_ptr& rhs)`

- Description: Copy constructor for the `spy_ptr` class. Constructs an instance of the `spy_ptr` class with the same pointer as the source `spy_ptr`.
- Parameters:
  - `rhs`: The `spy_ptr` object from which to copy.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr1(new Object());
  spy_ptr<Object> ptr2(ptr1);  // Copy constructor.
  ```

### `~spy_ptr()`

- Description: Destructor for the `spy_ptr` class. It does not perform any cleanup because the `spy_ptr` does not own the managed object.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr(new Object());
  // ...
  // The destructor will not perform any cleanup.
  ```

### `template <class R> explicit spy_ptr(R* rhs)`

- Description: Constructs an instance of the `spy_ptr` class with a raw pointer. The managed object is set to the given raw pointer.
- Parameters:
  - `rhs`: A raw pointer to the object being managed.
- Usage Example:
  ```cpp
  Object* obj = new Object();
  spy_ptr<Object> ptr(obj);  // Construct with a raw pointer.
  ```

### `template <class R> explicit spy_ptr(const spy_ptr<R>& rhs)`

- Description: Constructs an instance of the `spy_ptr` class from a different `spy_ptr` object. The managed object is set to the same object as the source `spy_ptr`.
- Parameters:
  - `rhs`: The source `spy_ptr` object.
- Usage Example:
  ```cpp
  spy_ptr<Derived> derivedPtr(new Derived());
  spy_ptr<Base> basePtr(derivedPtr);  // Construct from a different spy_ptr.
  ```

### `template <class R> explicit spy_ptr(const ref_ptr<R>& rhs)`

- Description: Constructs an instance of the `spy_ptr` class from a `ref_ptr` object. The managed object is set to the object managed by the source `ref_ptr`.
- Parameters:
  - `rhs`: The source `ref_ptr` object.
- Usage Example:
  ```cpp
  ref_ptr<Derived> derivedPtr(new Derived());
  spy_ptr<Base> basePtr(derivedPtr);  // Construct from a ref_ptr.
  ```

## Comparison Operators

### `template <class R> bool operator!=(const spy_ptr<R>& rhs) const`

- Description: Inequality operator. Checks if the managed object is not equal to the object managed by the given `spy_ptr`.
- Parameters:
  - `rhs`: The `spy_ptr` object being compared.
- Returns: `true` if the managed object is not equal to the object managed by the given `spy_ptr`; otherwise, `false`.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr1(new Object());
  spy_ptr<Object> ptr2(new Object());
  bool result = (ptr1 != ptr2);  // Inequality operator.
  ```

### `template <class R> bool operator!=(const R* rhs) const`

- Description: Inequality operator. Checks if the managed object is not equal to the given raw pointer.
- Parameters:
  - `rhs`: The raw pointer being compared.
- Returns: `true` if the managed object is not equal to the given raw pointer; otherwise, `false`.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr(new Object());
  bool result = (ptr != nullptr);  // Inequality operator.
  ```

### `template <class R> bool operator<=>(const spy_ptr<R>& rhs) const`

- Description: Comparison operator. Checks if the managed object is not equal to the object managed by the given `spy_ptr`.
- Parameters:
  - `rhs`: The `spy_ptr` object being compared.
- Returns: `true` if the managed object is not equal to the object managed by the given `spy_ptr`; otherwise, `false`.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr1(new Object());
  spy_ptr<Object> ptr2(new Object());
  bool result = (ptr1 <=> ptr2);  // Comparison operator.
  ```

### `template <class R> bool operator<=>(const R* rhs) const`

- Description: Comparison operator. Checks if the managed object is not equal to the given raw pointer.
- Parameters:
  - `rhs`: The raw pointer being compared.
- Returns: `true` if the managed object is not equal to the given raw pointer; otherwise, `false`.
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr(new Object());
  bool result = (ptr <=> nullptr);  // Comparison operator.
  ```

## Member Functions

### `bool valid() const noexcept`

- Description: Checks if the `spy_ptr` is managing a valid object.
- Returns: `true` if the `spy_ptr` is managing a valid object; otherwise, `false`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr(new Object());
  if (ptr.valid()) {
      // Perform operations on the managed object.
  }
  ```

### `explicit operator bool() const noexcept`

- Description: Conversion operator

to `bool`. Checks if the `spy_ptr` is managing a valid object.
- Returns: `true` if the `spy_ptr` is managing a valid object; otherwise, `false`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  spy_ptr<Object> ptr(new Object());
  if (ptr) {
      // Perform operations on the managed object.
  }
  ```

### `template <class R = T> requires std::derived_from<R, Object> explicit operator ref_ptr<R>() const noexcept`

- Description: Conversion operator to `ref_ptr`. Creates a new `ref_ptr` object managing the same object as the `spy_ptr`.
- Returns: A new `ref_ptr` object managing the same object as the `spy_ptr`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  spy_ptr<Derived> derivedPtr(new Derived());
  ref_ptr<Base> basePtr = static_cast<ref_ptr<Base>>(derivedPtr);  // Conversion operator.
  ```

### `template <class R = T> requires std::derived_from<R, Object> ref_ptr<R> load() const`

- Description: Loads the managed object into a new `ref_ptr` object of the same type.
- Returns: A new `ref_ptr` object managing the same object as the `spy_ptr`.
- Modifiers: `const`
- Usage Example:
  ```cpp
  spy_ptr<Object> spyPtr(new Object());
  ref_ptr<Object> refPtr = spyPtr.load();  // Load the managed object into a ref_ptr.
  ```

## Protected Member Functions

The following protected member function is used internally by the `spy_ptr` class:

### `template <class R> friend class spy_ptr`

- Description: Friend class declaration for `spy_ptr` with a different template parameter.
- Usage Example:
  ```cpp
  spy_ptr<Base> basePtr(new Base());
  spy_ptr<Derived> derivedPtr;
  derivedPtr = basePtr;  // Friendship allows access to private members.
  ```