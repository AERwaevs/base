#include <cstdio>
#include <cstdint>
#include <cstdlib
#include <cstring

namespace aer
{

class MakeID
{
private:
	// Change to uint16_t here for a more compact implementation if 16bit or less IDs work for you.
	typedef uint32_t uint;

	struct Range
	{
		uint first;
		uint last;
	};

	Range*  _ranges;    // Sorted array of ranges of free IDs
	uint    _count;     // Number of ranges in list
	uint    _capacity;  // Total capacity of range list

	MakeID & operator=(const MakeID &);
	MakeID(const MakeID &);

public:
	explicit MakeID(const uint max_id)
	{
		// Start with a single range, from 0 to max allowed ID (specified)
		_ranges = static_cast<Range*>(::malloc(sizeof(Range)));
		_ranges[0].first = 0;
		_ranges[0].last = max_id;
		_count = 1;
		_capacity = 1;
	}

	~MakeID()
	{
		::free(_ranges);
	}


	bool CreateID(uint &id)
	{
		if (_ranges[0].first <= _ranges[0].last)
		{
			id = _ranges[0].first;

			// If current range is full and there is another one, that will become the new current range
			if (_ranges[0].first == _ranges[0].last && _count > 1)
			{
				DestroyRange(0);
			}
			else
			{
				++_ranges[0].first;
			}
			return true;
		}

		// No availble ID left
		return false;
	}

	bool CreateRangeID(uint &id, const uint count)
	{
		uint i = 0;
		do
		{
			const uint range_count = 1 + _ranges[i].last - _ranges[i].first;
			if (count <= range_count)
			{
				id = _ranges[i].first;

				// If current range is full and there is another one, that will become the new current range
				if (count == range_count && i + 1 < _count)
				{
					DestroyRange(i);
				}
				else
				{
					_ranges[i].first += count;
				}
				return true;
			}
			++i;
		} while (i < _count);

		// No range of free IDs was large enough to create the requested continuous ID sequence
		return false;
	}

	bool DestroyID(const uint id)
	{
		return DestroyRangeID(id, 1);
	}

	bool DestroyRangeID(const uint id, const uint count)
	{
		const uint end_id = id + count;

		// Binary search of the range list
		uint i0 = 0;
		uint i1 = _count - 1;

		for (;;)
		{
			const uint i = (i0 + i1) / 2;

			if (id < _ranges[i].first)
			{
				// Before current range, check if neighboring
				if (end_id >= _ranges[i].first)
				{
					if (end_id != _ranges[i].first)
						return false; // Overlaps a range of free IDs, thus (at least partially) invalid IDs

					// Neighbor id, check if neighboring previous range too
					if (i > i0 && id - 1 == _ranges[i - 1].last)
					{
						// Merge with previous range
						_ranges[i - 1].last = _ranges[i].last;
						DestroyRange(i);
					}
					else
					{
						// Just grow range
						_ranges[i].first = id;
					}
					return true;
				}
				else
				{
					// Non-neighbor id
					if (i != i0)
					{
						// Cull upper half of list
						i1 = i - 1;
					}
					else
					{
						// Found our position in the list, insert the deleted range here
						InsertRange(i);
						_ranges[i].first = id;
						_ranges[i].last = end_id - 1;
						return true;
					}
				}
			}
			else if (id > _ranges[i].last)
			{
				// After current range, check if neighboring
				if (id - 1 == _ranges[i].last)
				{
					// Neighbor id, check if neighboring next range too
					if (i < i1 && end_id == _ranges[i + 1].first)
					{
						// Merge with next range
						_ranges[i].last = _ranges[i + 1].last;
						DestroyRange(i + 1);
					}
					else
					{
						// Just grow range
						_ranges[i].last += count;
					}
					return true;
				}
				else
				{
					// Non-neighbor id
					if (i != i1)
					{
						// Cull bottom half of list
						i0 = i + 1;
					}
					else
					{
						// Found our position in the list, insert the deleted range here
						InsertRange(i + 1);
						_ranges[i + 1].first = id;
						_ranges[i + 1].last = end_id - 1;
						return true;
					}
				}
			}
			else
			{
				// Inside a free block, not a valid ID
				return false;
			}

		}
	}

	bool IsID(const uint id) const
	{
		// Binary search of the range list
		uint i0 = 0;
		uint i1 = _count - 1;

		for (;;)
		{
			const uint i = (i0 + i1) / 2;

			if (id < _ranges[i].first)
			{
				if (i == i0)
					return true;

				// Cull upper half of list
				i1 = i - 1;
			}
			else if (id > _ranges[i].last)
			{
				if (i == i1)
					return true;

				// Cull bottom half of list
				i0 = i + 1;
			}
			else
			{
				// Inside a free block, not a valid ID
				return false;
			}

		}
	}

	uint GetAvailableIDs() const
	{
		uint count = _count;
		uint i = 0;

		do
		{
			count += _ranges[i].last - _ranges[i].first;
			++i;
		} while (i < _count);

		return count;
	}

	uint GetLargestContinuousRange() const
	{
		uint max_count = 0;
		uint i = 0;

		do
		{
			uint count = _ranges[i].last - _ranges[i].first + 1;
			if (count > max_count)
				max_count = count;

			++i;
		} while (i < _count);

		return max_count;
	}

	void PrintRanges() const
	{
		uint i = 0;
		for (;;)
		{
			if (_ranges[i].first < _ranges[i].last)
				printf("%u-%u", _ranges[i].first, _ranges[i].last);
			else if (_ranges[i].first == _ranges[i].last)
				printf("%u", _ranges[i].first);
			else
				printf("-");

			++i;
			if (i >= _count)
			{
				printf("\n");
				return;
			}

			printf(", ");
		}
	}


private:

	void InsertRange(const uint index)
	{
		if (_count >= _capacity)
		{
			_capacity += _capacity;
			_ranges = (Range *) realloc(_ranges, _capacity * sizeof(Range));
		}
 
		::memmove(_ranges + index + 1, _ranges + index, (_count - index) * sizeof(Range));
		++_count;
	}

	void DestroyRange(const uint index)
	{
		--_count;
		::memmove(_ranges + index, _ranges + index + 1, (_count - index) * sizeof(Range));
	}
};

}