//--------------------------------------------------------------------------//
/// Copyright (c) 2017 by Milos Tosic. All Rights Reserved.                ///
/// License: http://www.opensource.org/licenses/BSD-2-Clause               ///
//--------------------------------------------------------------------------//

#ifndef __RTM_RBASE_STRING_H__
#define __RTM_RBASE_STRING_H__

#include <rbase/inc/platform.h>
#include <string.h>

namespace rtm {

	typedef char (*CharFn)(char _ch);

	inline static char toNoop(char _ch)
	{
		return _ch;
	}

	inline static bool isUpper(char _ch)
	{
		return _ch >= 'A' && _ch <= 'Z';
	}

	inline static char toLower(char _ch)
	{
		return _ch + (isUpper(_ch) ? 0x20 : 0);
	}

	template<CharFn fn>
	inline static int32_t strCmp(const char* _lhs, const char* _rhs, uint32_t _max)
	{
		for (
			; 0 < _max && fn(*_lhs) == fn(*_rhs)
			; ++_lhs, ++_rhs, --_max
			)
		{
			if (*_lhs == '\0'
			||  *_rhs == '\0')
			{
				break;
			}
		}

		return 0 == _max ? 0 : fn(*_lhs) - fn(*_rhs);
	}

	template<CharFn fn>
	inline static const char* strStr(const char* _str, uint32_t _strMax, const char* _find, uint32_t _findMax)
	{
		const char* ptr = _str;

		int32_t       stringLen = strnlen(_str,  _strMax);
		const int32_t findLen   = strnlen(_find, _findMax);

		for (; stringLen >= findLen; ++ptr, --stringLen)
		{
			// Find start of the string.
			while (fn(*ptr) != fn(*_find) )
			{
				++ptr;
				--stringLen;

				// Search pattern lenght can't be longer than the string.
				if (findLen > stringLen)
				{
					return NULL;
				}
			}

			// Set pointers.
			const char* string = ptr;
			const char* search = _find;

			// Start comparing.
			while (fn(*string++) == fn(*search++) )
			{
				// If end of the 'search' string is reached, all characters match.
				if ('\0' == *search)
				{
					return ptr;
				}
			}
		}

		return NULL;
	}


	inline static int32_t strncmp(const char* _lhs, const char* _rhs, uint32_t _max)
	{
		return strCmp<toNoop>(_lhs, _rhs, _max);
	}

	inline static int32_t strincmp(const char* _lhs, const char* _rhs, uint32_t _max)
	{
		return strCmp<toLower>(_lhs, _rhs, _max);
	}

	inline static int32_t strnlen(const char* _str, uint32_t _max)
	{
		if (NULL == _str)
		{
			return 0;
		}

		const char* ptr = _str;
		for (; 0 < _max && *ptr != '\0'; ++ptr, --_max) {};
		return int32_t(ptr - _str);
	}

	inline static int32_t strlncpy(char* _dst, int32_t _dstSize, const char* _src, uint32_t _num)
	{
		RTM_ASSERT(NULL != _dst, "_dst can't be NULL!");
		RTM_ASSERT(NULL != _src, "_src can't be NULL!");
		RTM_ASSERT(0 < _dstSize, "_dstSize can't be 0!");

		const int32_t len = strnlen(_src, _num);
		const int32_t max = _dstSize-1;
		const int32_t num = (len < max ? len : max);
		memcpy(_dst, _src, num);
		_dst[num] = '\0';

		return num;
	}

	inline static const char* strnstr(const char* _str, const char* _find, uint32_t _max = -1)
	{
		return strStr<toNoop>(_str, _max, _find, INT32_MAX);
	}

	inline static const char* stristr(const char* _str, const char* _find, uint32_t _max = -1)
	{
		return strStr<toLower>(_str, _max, _find, INT32_MAX);
	}

} // namespace rtm

#endif // __RTM_RBASE_STRING_H__

