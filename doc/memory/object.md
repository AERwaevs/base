
The `Object` class represents a base class for reference-counted objects. Designed for use with `ref_ptr`.

## Class Overview

The `Object` class provides functionality for reference counting. It is designed to be used as a base class for objects that require reference counting.

## Constructors and Destructors

These are protected to enforce the use of the interface `ICreate`

### `explicit Object() noexcept`

- Description: Constructs an instance of the `Object` class.
- Usage Example:
  ```cpp
  Object* obj = new Object();
  ```

### `virtual ~Object() = default`

- Description: Destructor for the `Object` class.
- Modifiers: `virtual`, `default`
- Usage Example:
  ```cpp
  Object* obj = new Object();
  delete obj;
  ```

### `Object(Object&&) = delete`

- Description: Move constructor. The move constructor is deleted, disallowing the move construction of `Object` instances.
- Usage Example:
  ```cpp
  Object obj1;
  Object obj2(std::move(obj1));  // Compilation error: Move constructor is deleted.
  ```

### `Object(const Object&) = delete`

- Description: Copy constructor. The copy constructor is deleted, disallowing the copy construction of `Object` instances.
- Usage Example:
  ```cpp
  Object obj1;
  Object obj2(obj1);  // Compilation error: Copy constructor is deleted.
  ```

## Public Member Functions

### `auto ref_count(std::memory_order order = std::memory_order_relaxed) const noexcept`

- Description: Returns the current reference count of the object.
- Parameters:
  - `order` (optional): The memory order used for the atomic load operation. Defaults to `std::memory_order_relaxed`.
- Returns: An `auto` value representing the current reference count.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  Object* obj = new Object();
  auto count = obj->ref_count();
  ```

## Protected Member Functions

The following protected member functions are used internally by the `Object` class:

### `void _ref(std::memory_order order = std::memory_order_relaxed) const noexcept`

- Description: Increments the reference count of the object by one.
- Parameters:
  - `order` (optional): The memory order used for the atomic fetch_add operation. Defaults to `std::memory_order_relaxed`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  Object* obj = new Object();
  obj->_ref();
  ```

### `void _unref(std::memory_order order = std::memory_order_seq_cst) const noexcept`

- Description: Decrements the reference count of the object by one and deletes the object if the reference count reaches zero.
- Parameters:
  - `order` (optional): The memory order used for the atomic fetch_sub operation. Defaults to `std::memory_order_seq_cst`.
- Modifiers: `const`, `noexcept`
- Usage Example:
  ```cpp
  Object* obj = new Object();
  obj->_unref();
  ```

## Friend Class

The `Object` class declares a friend class:

### `template <typename T> class ref_ptr`

- Description: A template class that implements reference counting for `Object` instances.
- Usage Example:
  ```cpp
  ref_ptr<Object> objPtr(new Object());
  ```

## Private Member Variables

The `Object` class has the following private member variable:

- `std::atomic_uint32_t _references`: An `std::atomic` object used for storing the reference count of the object.