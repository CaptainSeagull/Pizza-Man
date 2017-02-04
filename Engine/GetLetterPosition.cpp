// NOTE(Jonny):		This is just down here to keep it out the way
//					because this is one long-ass function...
// 					Will probably move it to it's own .cpp file at some point
#include "Junk.h"


static Vector2f
GetLetterPosition(char letter)
{
	Vector2f result = {};

	switch(letter)
	{
		case ' ':
		{
			result.Set(0.0f, 0.0f);
		} break;

		case '!':
		{
			result.Set(1.0f, 0.0f);
		} break;

		case '"':
		{
			result.Set(2.0f, 0.0f);
		} break;

		case '#':
		{
			result.Set(3.0f, 0.0f);
		} break;

		case '$':
		{
			result.Set(4.0f, 0.0f);
		} break;

		case '%':
		{
			result.Set(5.0f, 0.0f);
		} break;

		case '&':
		{
			result.Set(6.0f, 0.0f);
		} break;

		// case ''':
		// {
		// 	result.Set(7.0f, 0.0f);
		// } break;
		
		case '(':
		{
			result.Set(8.0f, 0.0f);
		} break;	

		case ')':
		{
			result.Set(9.0f, 0.0f);			
		} break;

		case '*':
		{
			result.Set(10.0f, 0.0f);
		} break;

		case '+':
		{
			result.Set(11.0f, 0.0f);
		} break;

		// NOTE(Jonny):	Not sure about this one
		// case '`':
		// {
		// 	result.Set(12.0f, 0.0f);
		// } break;

		// NOTE(Jonny):	Not sure about this one
		case '_':
		{
			result.Set(13.0f, 0.0f);
		} break;

		// Skip 14, 0

		case '/':
		{
			result.Set(15.0f, 0.0f);
		} break;

		case '0':
		{
			result.Set(0.0f, 1.0f);
		} break;

		case '1':
		{
			result.Set(1.0f, 1.0f);
		} break;

		case '2':
		{
			result.Set(2.0f, 1.0f);
		} break;

		case '3':
		{
			result.Set(3.0f, 1.0f);
		} break;

		case '4':
		{
			result.Set(4.0f, 1.0f);
		} break;

		case '5':
		{
			result.Set(5.0f, 1.0f);
		} break;

		case '6':
		{
			result.Set(6.0f, 1.0f);
		} break;

		case '7':
		{
			result.Set(7.0f, 1.0f);
		} break;

		case '8':
		{
			result.Set(8.0f, 1.0f);
		} break;

		case '9':
		{
			result.Set(9.0f, 1.0f);
		} break;

		case ':':
		{
			result.Set(10.0f, 1.0f);
		} break;

		case ';':
		{
			result.Set(11.0f, 1.0f);
		} break;

		case '<':
		{
			result.Set(12.0f, 1.0f);
		} break;

		case '=':
		{
			result.Set(13.0f, 1.0f);
		} break;

		case '>':
		{
			result.Set(14.0f, 1.0f);
		} break;

		case '?':
		{
			result.Set(15.0f, 1.0f);
		} break;

		case '@':
		{
			result.Set(0.0f, 2.0f);
		} break;

		case 'A':
		{
			result.Set(1.0f, 2.0f);
		} break;

		case 'B':
		{
			result.Set(2.0f, 2.0f);
		} break;

		case 'C':
		{
			result.Set(3.0f, 2.0f);
		} break;

		case 'D':
		{
			result.Set(4.0f, 2.0f);
		} break;

		case 'E':
		{
			result.Set(5.0f, 2.0f);
		} break;

		case 'F':
		{
			result.Set(6.0f, 2.0f);
		} break;

		case 'G':
		{
			result.Set(7.0f, 2.0f);
		} break;

		case 'H':
		{
			result.Set(8.0f, 2.0f);
		} break;

		case 'I':
		{
			result.Set(9.0f, 2.0f);
		} break;

		case 'J':
		{
			result.Set(10.0f, 2.0f);
		} break;

		case 'K':
		{
			result.Set(11.0f, 2.0f);
		} break;

		case 'L':
		{
			result.Set(12.0f, 2.0f);
		} break;

		case 'M':
		{
			result.Set(13.0f, 2.0f);
		} break;

		case 'N':
		{
			result.Set(14.0f, 2.0f);
		} break;

		case 'O':
		{
			result.Set(15.0f, 2.0f);
		} break;

		case 'P':
		{
			result.Set(0.0f, 3.0f);
		} break;

		case 'Q':
		{
			result.Set(1.0f, 3.0f);
		} break;

		case 'R':
		{
			result.Set(2.0f, 3.0f);
		} break;

		case 'S':
		{
			result.Set(3.0f, 3.0f);
		} break;

		case 'T':
		{
			result.Set(4.0f, 3.0f);
		} break;

		case 'U':
		{
			result.Set(5.0f, 3.0f);
		} break;

		case 'V':
		{
			result.Set(6.0f, 3.0f);
		} break;

		case 'W':
		{
			result.Set(7.0f, 3.0f);
		} break;

		case 'X':
		{
			result.Set(8.0f, 3.0f);
		} break;

		case 'Y':
		{
			result.Set(9.0f, 3.0f);
		} break;

		case 'Z':
		{
			result.Set(10.0f, 3.0f);
		} break;

		case '[':
		{
			result.Set(11.0f, 3.0f);
		} break;

		// case '\':
		// {
		// 	result.Set(12.0f, 3.0f);
		// } break;

		case ']':
		{
			result.Set(13.0f, 3.0f);
		} break;

		case '^':
		{
			result.Set(14.0f, 3.0f);
		} break;

		// NOTE(Jonny):	Not 100% sure about this one...
		case '-':
		{
			result.Set(15.0f, 3.0f);
		} break;

		// NOTE(Jonny):	Not 100% sure about this one...
		// case '`':
		// {
		// 	result.Set(0.0f, 4.0f);
		// } break;
		
		case 'a':
		{
			result.Set(1.0f, 4.0f);
		} break;

		case 'b':
		{
			result.Set(2.0f, 4.0f);
		} break;

		case 'c':
		{
			result.Set(3.0f, 4.0f);
		} break;

		case 'd':
		{
			result.Set(4.0f, 4.0f);
		} break;

		case 'e':
		{
			result.Set(5.0f, 4.0f);
		} break;

		case 'f':
		{
			result.Set(6.0f, 4.0f);
		} break;

		case 'g':
		{
			result.Set(7.0f, 4.0f);
		} break;

		case 'h':
		{
			result.Set(8.0f, 4.0f);
		} break;

		case 'i':
		{
			result.Set(9.0f, 4.0f);
		} break;

		case 'j':
		{
			result.Set(10.0f, 4.0f);
		} break;

		case 'k':
		{
			result.Set(11.0f, 4.0f);
		} break;

		case 'l':
		{
			result.Set(12.0f, 4.0f);
		} break;

		case 'm':
		{
			result.Set(13.0f, 4.0f);
		} break;

		case 'n':
		{
			result.Set(14.0f, 4.0f);
		} break;

		case 'o':
		{
			result.Set(15.0f, 4.0f);
		} break;

		case 'p':
		{
			result.Set(0.0f, 5.0f);
		} break;

		case 'q':
		{
			result.Set(1.0f, 5.0f);
		} break;

		case 'r':
		{
			result.Set(2.0f, 5.0f);
		} break;

		case 's':
		{
			result.Set(3.0f, 5.0f);
		} break;

		case 't':
		{
			result.Set(4.0f, 5.0f);
		} break;

		case 'u':
		{
			result.Set(5.0f, 5.0f);
		} break;

		case 'v':
		{
			result.Set(6.0f, 5.0f);
		} break;

		case 'w':
		{
			result.Set(7.0f, 5.0f);
		} break;

		case 'x':
		{
			result.Set(8.0f, 5.0f);
		} break;

		case 'y':
		{
			result.Set(9.0f, 5.0f);
		} break;

		case 'z':
		{
			result.Set(10.0f, 5.0f);
		} break;

		case '{':
		{
			result.Set(11.0f, 5.0f);
		} break;

		case '|':
		{
			result.Set(12.0f, 5.0f);
		} break;

		case '}':
		{
			result.Set(13.0f, 5.0f);
		} break;

		case '~':
		{
			result.Set(14.0f, 5.0f);
		} break;
	}

	return result;
}