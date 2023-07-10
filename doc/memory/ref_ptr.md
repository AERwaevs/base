
The `ref_ptr` class, defined within the `aer` namespace, provides a smart pointer implementation for managing reference-counted objects.

## Dependencies

[Object.h](object.md)

## Class Overview

The `ref_ptr` class provides a smart pointer implementation for managing reference-counted objects. It allows automatic reference counting and cleanup of the managed objects. The class provides a variety of constructors, assignment operators, and member functions for efficient and safe handling of reference-counted objects.

## Template Parameter

- `T`: The type of the object being managed by the smart pointer.

## Constructors and Destructor

### `ref_ptr() noexcept`

- Description: Constructs an instance of the `ref_ptr` class with a null pointer.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr;
  ```

### `~ref_ptr() noexcept`

- Description: Destructor for the `ref_ptr` class. Decrements the reference count and deletes the object if the reference count reaches zero.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  ptr->doSomething();
  // ...
  // The destructor will automatically decrement the reference count and delete the object if necessary.
  ```

### `ref_ptr(const ref_ptr& rhs) noexcept`

- Description: Copy constructor for the `ref_ptr` class. Increments the reference count of the object being copied.
- Parameters:
  - `rhs`: The `ref_ptr` object from which to copy.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr1(new Object());
  ref_ptr<Object> ptr2(ptr1);  // Copy constructor increments the reference count.
  ```

### `template <class R> ref_ptr(ref_ptr<R>&& rhs) noexcept`

- Description: Move constructor for the `ref_ptr` class. Moves the managed object from the source `ref_ptr` and sets the source `ref_ptr` to null.
- Parameters:
  - `rhs`: The `ref_ptr` object from which to move.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr1(new Object());
  ref_ptr<Base> ptr2(std::move(ptr1));  // Move constructor moves the managed object.
  ```

### `template <class R> ref_ptr(const ref_ptr<R>& rhs) noexcept`

- Description: Copy constructor for the `ref_ptr` class with different template parameter. Increments the reference count of the object being copied.
- Parameters:
  - `rhs`: The `ref_ptr` object from which to copy.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Derived> ptr1(new Derived());
  ref_ptr<Base> ptr2(ptr1);  // Copy constructor with different template parameter increments the reference count.
  ```

### `explicit ref_ptr(T* rhs) noexcept`

- Description: Constructs an instance of the `ref_ptr` class with a raw pointer. Increments the reference count of the object being pointed to.
- Parameters:
  - `rhs`: A raw pointer to the object being managed.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  Object* obj = new Object();
  ref_ptr<Object> ptr(obj);  // Construct with a raw pointer.
  ```

### `template <class R> explicit ref_ptr(R* rhs) noexcept`

- Description: Constructs an instance of the `ref_ptr` class with a raw pointer of a different type. Increments the reference count of the object being pointed to.
- Parameters:
  - `rhs`: A raw pointer to the object being managed.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  Derived* derivedObj = new Derived();
  ref_ptr<Base> ptr(derivedObj);  // Construct with a raw pointer of a different type.
  ```

## Assignment Operators

### `ref_ptr& operator=(T* rhs)`

- Description: Assignment operator. Assigns a raw pointer to the `ref_ptr`, managing the object pointed to by the raw pointer. Decrements the reference count of the previously managed object and increments the reference count of the assigned object.
- Parameters:
  - `rhs`: A raw pointer to the object being assigned.
- Returns: A reference to the modified `ref_ptr`.
- Usage Example:
  ```cpp
  Object* obj1 = new Object();
  Object* obj2 = new Object();
  ref_ptr<Object> ptr(obj1);
  ptr = obj2;  // Assign a raw pointer to the ref_ptr.
  ```

### `ref_ptr& operator=(const ref_ptr& rhs)`

- Description: Copy assignment operator. Assigns a `ref_ptr` object to another `ref_ptr`, managing the object pointed to by the assigned `ref_ptr`. Decrements the reference count of the previously managed object and increments the reference count of the assigned object.
- Parameters:
  - `rhs`: The `ref_ptr` object being assigned.
- Returns: A reference to the modified `ref_ptr`.
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr1(new Object());
  ref_ptr<Object> ptr2;
  ptr2 = ptr1;  // Copy assignment operator.
  ```

### `template <class R> ref_ptr& operator=(const ref_ptr<R>& rhs)`

- Description: Copy assignment operator with different template parameter. Assigns a `ref_ptr` object of a different type to another `ref_ptr`, managing the object pointed to by the assigned `ref_ptr`. Decrements the reference count of the previously managed object and increments the reference count of the assigned object.
- Parameters:
  - `rhs`: The `ref_ptr` object being assigned.
- Returns: A reference to the modified `ref_ptr`.
- Usage Example:
  ```cpp
  ref_ptr<Derived> ptr1(new Derived());
  ref_ptr<Base> ptr2;
  ptr2 = ptr1;  // Copy assignment operator with different template parameter.
  ```

### `template <class R> ref_ptr& operator=(ref_ptr<R>&& rhs)`

- Description: Move assignment operator. Moves the managed object from the source `ref_ptr` to the destination `ref_ptr` and sets the source `ref_ptr` to null.
- Parameters:
  - `rhs`: The `ref_ptr` object from which to move.
- Returns: A reference to the modified `ref_ptr`.
- Usage Example:
  ```cpp
  ref_ptr<Derived> ptr1(new Derived());
  ref_ptr<Base> ptr2;
  ptr2 = std::move(ptr1);  // Move assignment operator.
  ```

## Comparison Operators

### `template <class R> bool operator!=(const R* rhs) const`

- Description: Inequality operator. Checks if the managed object is not equal to a raw pointer.
- Parameters:
  - `rhs`: A raw pointer being compared.
- Returns: `true` if the managed object is not equal to the raw pointer; otherwise, `false`.
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  bool result = (ptr != nullptr);  // Inequality operator.
  ```

### `template <class R> bool operator<=>(const R* rhs) const`

- Description: Comparison operator. Checks if the managed object is not equal to a raw pointer.
- Parameters:
  - `rhs`: A raw pointer being compared.
- Returns: `true` if the managed object is not equal to the raw pointer; otherwise, `false`.
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  bool result = (ptr <=> nullptr);  // Comparison operator.
  ```

## Member Functions

### `bool valid() const noexcept`

- Description: Checks if the `ref_ptr` is managing a valid object.
- Returns: `true` if the `ref_ptr` is managing a valid object; otherwise, `false`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  if (ptr.valid()) {
      // Perform operations on the managed object.
  }
  ```

### `explicit operator bool() const noexcept`

- Description: Conversion operator to `bool`. Checks if the `ref_ptr` is managing a valid object.
- Returns: `true` if the `ref_ptr` is managing a valid object; otherwise, `false`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  if (ptr) {
      // Perform operations on the managed object.
  }
  ```

### `operator T*() const noexcept`

- Description: Conversion operator to the managed object's raw pointer.
- Returns: A raw pointer to the managed object.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  Object* rawPtr = ptr;  // Conversion operator.
  ```

### `T* operator->() const noexcept`

- Description: Dereference operator. Provides access to the managed object's member functions and data.
- Returns: A pointer to the managed object.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  ptr->doSomething();  // Dereference operator.
  ```

### `T* get() const noexcept`

- Description: Retrieves the raw pointer to the managed object.
- Returns: A raw pointer to the managed object.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  Object* rawPtr = ptr.get();
  ```

### `T& operator*() const noexcept`

- Description: Dereference operator. Provides access to the managed object's member functions and data.
- Returns: A reference to the managed object.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  ptr->doSomething();  // Dereference operator.
  ```

### `void operator[](int) const = delete`

- Description: Deleted indexing operator. Prevents indexing of the managed object.
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr(new Object());
  ptr[0];  // Compilation error: Indexing operator is deleted.
  ```

### `void swap(ref_ptr& rhs) noexcept`

- Description: Swaps the managed objects between two `ref_ptr` instances.
- Parameters:
  - `rhs`: The `ref_ptr` object to swap with.
- Modifiers: `noexcept`
- Usage Example:
  ```cpp
  ref_ptr<Object> ptr1(new Object());
  ref_ptr<Object> ptr2(new Object());
  ptr1.swap(ptr2);  // Swap managed objects between two ref_ptr instances.
  ```

### `template <class R> ref_ptr<R> cast() const`

- Description: Casts the managed object to a different type and returns a new `ref_ptr` of that type.
- Returns: A new `ref_ptr` managing the casted object, or a null `ref_ptr` if the managed object is null.
- Modifiers: `const`
- Usage Example:
  ```cpp
  ref_ptr<Derived> derivedPtr(new Derived());
  ref_ptr<Base> basePtr = derivedPtr.cast<Base>();  // Cast the managed object to a different type.
  ```

## Protected Member Functions

The following protected member function is used internally by the `ref_ptr` class:

### `template <class R> friend class ref_ptr`

- Description: Friend class declaration for `ref_ptr` with a different template parameter.
- Usage Example:
  ```cpp
  ref_ptr<Base> basePtr(new Base());
  ref_ptr<Derived> derivedPtr;
  derivedPtr = basePtr;  // Friendship allows access to private members.
  ```