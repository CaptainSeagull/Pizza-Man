#ifndef UTILITIES_H
#define UTILITIES_H

	//=========================================================================
	// Macro Defines
	//=========================================================================

	#if ASSERT
		#if WIN32
			#define Assert(expression) if (!(expression)) { DebugBreak(); }
		#else
			#define Assert(expression) if (!(expression)) { *(int*)NULL=1; }
		#endif
	#else
		#define Assert(expression)
	#endif

	#define Breakpoint Assert(1==0)

	struct Vector2f
	{
		float x, y;

		void Set(float ix, float iy)
		{
			x = ix;
			y = iy;
		}
	};
	
	#define Bytes(value) 		((value) * 8LL))
	#define Kilobytes(value) 	((value) * (1024LL))
	#define Megabytes(value) 	(Kilobytes(value) * (1024LL))
	#define Gigabytes(value) 	(Megabytes(value) * (1024LL))
	#define Terabytes(value) 	(Gigabytes(value) * (1024LL))
	#define Pentabytes(value) 	(Terabytes(value) * (1024LL))
	
	#define ArrayCount(array) ((sizeof(array)) / (sizeof((array)[0])))

	#define Square(x) ((x)*(x))
	#define Max(a, b) ((a)>(b)?(a):(b))
	#define Min(a, b) ((a)<(b)?(a):(b))

	#define RoundFloatToInt(number) (int32)((number) + 0.5f)

	#define Pi32 				(3.14159265359f)
	#define Pi64 				(3.141592653589793238462643383279502884L)

	#define Deg2Rad32		(Pi32/180.0f)
	#define Deg2Rad64		(Pi64/180.0)

	#define Rad2Deg32		(180.0f/Pi32)
	#define Rad2Deg64		(180.0/Pi64)

	#define Loop(x) 				for(int32_t index=0; index<(x); index++)
	#define ULoop(x) 				for(uint32_t index=0; index<(x); index++)
	#define RLoop(x)				for(int32_t index=(x)-1; index>0; index--)

	#define Foreach(list, index) 	for(int32_t (index)=0; (index)<(list); (index)++)
	#define UForeach(list, index) 	for(uint32_t (index)=0; (index)<(list); (index)++)
	#define RForeach(list, index) 	for(int32_t (index)=(list)-1; (index)>=0; (index)--)

	#define WINDOW_WIDTH 	(1920)
	#define WINDOW_HEIGHT 	(1080)

	#define SCENE_SCALE 	(0.08f)

	inline uint32_t
	SafeTruncateSize64(uint64_t value)
	{
		Assert(value <= 0xFFFFFFFF);
		uint32_t result = (uint32_t)value;
		return result;
	}

	inline float
	RemoveDecimalFromFloat(float number)
	{
		float result = (float)((int32_t)number);
		return result;
	}

	static uint32_t
	StringLength(char *text)
	{
		uint32_t count = 0;
		while(*text++)
		{
			count++;
		}
		return count;
	}

	// NOTE(Jonny): Isn't very accurate, but works fast!
	static float
	FastSquareRoot(float number)
	{
		uint32_t result = *(uint32_t *)&number;
		result += 127 << 23;
		result >>= 1;
		return *(float*) &result;
	}

	// NOTE(Jonny): Allocates Memory. called free() after use
	static char *
	ConcatonateStrings(char *string1, char *string2)
	{
		uint32_t string1Length = StringLength(string1);
		uint32_t string2Length = StringLength(string2);

		char *result = (char *)malloc(8 * (string1Length + string2Length));
		Assert(result != NULL);

		ULoop(string1Length)
		{
			result[index] = string1[index];
		}
		ULoop(string2Length)
		{
			result[string1Length+index] = string2[index];
		}

		result[string1Length+string2Length] = '\0';

		return result;
	}

	static char *
	ConcatonateStringAndInt(char *string, int32_t integer)
	{
		char *result;
		char integerAsChar[10];
		#if WIN32
			sprintf_s(integerAsChar, sizeof(char) * 10, "%d", integer);
		#else
			sprintf(integerAsChar, "%d", integer);
		#endif
		result = ConcatonateStrings(string, integerAsChar);
		return result;
	}
	
	static char *
	ConcatonateIntAndString(int32_t integer, char *string)
	{
		char *result;
		char integerAsChar[10];
		#if WIN32
			sprintf_s(integerAsChar, sizeof(char) * 10, "%d", integer);
		#else
			sprintf(integerAsChar, "%d", integer);
		#endif
		result = ConcatonateStrings(integerAsChar, string);
		return result;
	}

#endif