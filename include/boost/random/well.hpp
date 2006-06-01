#ifndef BOOST_RANDOM_WELL_HPP
#define BOOST_RANDOM_WELL_HPP

#include<iostream>
#include <stdexcept>
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/static_assert.hpp>
#include <boost/integer_traits.hpp>
#include <boost/cstdint.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/detail/workaround.hpp>
#include <limits>

namespace boost { namespace random {

template<int shift>
class mat0pos
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return (v^(v>>shift)); 
		}
	

};

template<int shift>
class mat0neg
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return v^(v <<(-(shift)));
		}

};

template<int shift>
class maF3neg
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return (v<<(-(shift)));
		}

};

template<int shift>
class maF4neg
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return (v ^ ((v<<(-(shift))) & 0xda442d24U));   // Maske immer gleich f�r maF4neg?
		}

};

class identity
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return v;		
		}

};

class zero
{
	public: 
		template<class UIntType>
		UIntType f(UIntType  v)
		{
			return 0;		
		}
};

template<class UIntType, int statesize, UIntType val, class F1, class F2, class F3, class F4, class F5, class F6, class F7, class F8, int p1,  int p2,  int p3, UIntType mask>
class well
{
	public:
		typedef UIntType result_type;

		BOOST_STATIC_CONSTANT(result_type, min_value = 0);
		BOOST_STATIC_CONSTANT(result_type, max_value);
		BOOST_STATIC_CONSTANT(bool, has_fixed_range = true);
				
		//constructor
		well()
		{
			seed();
		}
		
		explicit well(UIntType value)
		{
			seed(value);
		}
		
		template<class It> well(It & first, It last)
		{
			seed(first,last);
		}
		
		//default seed
		
		void seed(result_type value = 5489)
		{
			UIntType seedmask = ~0u;
			state[0] = value & seedmask;
			for (int i = 1; i < statesize; i++) 
			{
				state[i] = (1812433253UL * (state[i-1] ^ (state[i-1] >> (statesize -2))) + i) & seedmask;
			}
			state_i = 0;
			
		}
		
		//seed wenn Iterator �bergeben
		
		template<class It>
		void seed(It& first, It last)
		{
			
			for(int j = 0; j < statesize && first != last; ++j, ++first)
			{
				state[j] = *first;
				if(first == last && j < statesize)
				{
					throw std::invalid_argument("well::seed");
				}
			
			}
			state_i = 0;
				
			
		}
		
		
		
		// ==
		
		friend bool operator==(const well& x, const well& y)
		{
			for(int j = 0; j < statesize; ++j)
			{
				if(x.state[j] != y.state[j])
				{
					return false;
				}
				
			}
			return true;
		}
			
		// !=
		
		friend bool operator!=(const well& x, const well& y)
		{ return !(x == y); }
		
		//ostream
		
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT,Traits>&
		operator<<(std::basic_ostream<CharT,Traits>& os, well& w)
		{
			for(int j = 0; j < statesize; j++)
			{
			os << w.state[j] << " ";
			}
			return os;
		}
		
		//istream
		
		template<class CharT, class Traits>
		friend std::basic_istream<CharT,Traits>&
		operator>>(std::basic_istream<CharT,Traits>& is, well& w)
		{
			for(int j = 0; j < statesize; j++)
			{
			is >> w.state[j] >> std::ws;
			}
			w.state_i = 0;
			return is;
		}
		
		result_type min BOOST_PREVENT_MACRO_SUBSTITUTION ()
		{
			return min_value;
		}
		
		result_type max BOOST_PREVENT_MACRO_SUBSTITUTION ()
		{
			result_type res = 0;
			for(int i = 0; i < 32; ++i)
			res |= (1u << i);
			return res;
		}
		
		static bool validation(result_type value)
		{
			return(val == value);
		}
		
		result_type operator()()
		{
			z0 = state[(state_i + (statesize - 1) )& mask];
			z1 = F1::fF2::f(state[state_i])^F2()(state[(state_i + p1) & mask]);
			z2 = F3::f(state[(state_i + p2)& mask])^F4::f(state[(state_i + p3) & mask]);
			z3 = z1^z2;
			state[state_i] = z3;
			z4 = F5::f(z0)^F6::f(z1)^F7::f(z2)^F8::f(z3);
			state[(state_i + (statesize - 1) )& mask] = z4;
			state_i = (state_i + (statesize - 1) ) & mask;
			return state[state_i];
		}
				
	private:
		result_type state[statesize];
		UIntType state_i;
		result_type z0;
		result_type z1;
		result_type z2;
		result_type z3;
		result_type z4;
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
template<class UIntType, int statesize, UIntType val, class F1, class F2, class F3, class F4, class F5, class F6, class F7, class F8, int p1,  int p2,  int p3, UIntType mask>
const bool well<UIntType, statesize, val, F1, F2, F3, F4, F5, F6, F7, F8, p1, p2, p3, mask>::has_fixed_range;

template<class UIntType, int statesize, UIntType val, class F1, class F2, class F3, class F4, class F5, class F6, class F7, class F8, int p1,  int p2,  int p3, UIntType mask>
const UIntType well<UIntType, statesize, val, F1, F2, F3, F4, F5, F6, F7, F8, p1, p2, p3, mask>::min_value;

template<class UIntType, int statesize, UIntType val, class F1, class F2, class F3, class F4, class F5, class F6, class F7, class F8, int p1,  int p2,  int p3, UIntType mask>
const UIntType well<UIntType, statesize, val, F1, F2, F3, F4, F5, F6, F7, F8, p1, p2, p3, mask>::max_value;
#endif

typedef well<uint32_t,16,178010050,mat0neg<-16>, mat0neg<-15>, mat0pos<11>, 
            zero, mat0neg<-2>, mat0neg<-18>, maF3neg<-28>, maF4neg<-5>,
            13,9,1, 0x0000000fU > well512a; // 10001 element : 417608049

typedef well<uint32_t,32,1573116597,identity, mat0pos<8>, mat0neg<-19>, 
             mat0neg<-14>, mat0neg<-11>, mat0neg<-7>, mat0neg<-13>, zero,
             3,24,10, 0x0000001fU > well1024a;  // 10001 element : 1808838631

} // end namespace random
		
using random::well512a;
using random::well10245a;


} // end namespace boost


#endif /*BOOST_RANDOM_WELL_HPP*/
