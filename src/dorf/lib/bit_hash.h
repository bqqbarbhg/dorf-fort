#ifndef _DORF_LIB_BIT_HASH_H
#define _DORF_LIB_BIT_HASH_H

#include <bitset>

// Inherit this when creating a new hash type
// T: Type of object to hash
// NumBits: Number of bits contained in the hash set (profile to balance memory/collisions)
// REQUIREMENTS:
//   static HashType getHash(T)
template <typename T, unsigned NumBits>
struct HashTraits
{
public:
	// Type of objects to hash
	using Type = T;

	// Type of the result hash
	using HashType = unsigned;

	// Number of bits that the hash SET contains (NOT HashType)
	enum { Bits = NumBits };
private:
};

// Very simple and efficent bloom filter for hashes (will result in a lot of false positives)
// - Currently wraps std::bitset
// - Could be optimized in the future for special bit lengths
// - Bit struct could be used to store a mask to increase performance
template <typename HashTraits>
class HashBits
{
public:
	struct Bit
	{
	public:
	private:
		friend class HashBits<HashTraits>;
		explicit Bit(unsigned hash)
			: pos(hash % HashTraits::Bits)
		{ }
		unsigned pos;
	};
	
	static Bit getHashBit(const typename HashTraits::Type& value)
	{
		return Bit{ HashTraits::getHash(value) };
	}

	// Set a bit
	// See: getHashBit
	void insert(Bit bit)
	{
		m_impl.set(bit.pos);
	}

	// Insert all the bits in an other hash bit set
	void insert(const HashBits<HashTraits>& bits)
	{
		m_impl |= bits.m_impl;
	}

	// Returns whether a bit is set
	// WARNING: Can often return true even if the hash is not present!
	// See: getHashBit
	bool get(Bit bit)
	{
		return m_impl.test(pos);
	}

private:
	std::bitset<HashTraits::Bits> m_impl;
};

#endif
