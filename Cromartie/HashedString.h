#pragma once

#include <string>

#ifndef NULL
#define NULL 0
#endif

class HashedString;
typedef HashedString EventType;

class HashedString
{
public:
	explicit HashedString( char const * const pIdentString )
		: m_ident( hash_name( pIdentString ) )
		, m_identStr( pIdentString )
	{
	}

	unsigned long getHashValue( void ) const
	{
		return reinterpret_cast<unsigned long>( m_ident );
	}

	const std::string & getStr() const
	{
		return m_identStr;
	}

	static void * hash_name( char const *  pIdentStr );

	bool operator< ( HashedString const & o ) const
	{
		bool r = ( getHashValue() < o.getHashValue() );
		return r;
	}

	bool operator== ( HashedString const & o ) const
	{
		bool r = ( getHashValue() == o.getHashValue() );
		return r;
	}

private:
	void * m_ident;
	std::string m_identStr;
};