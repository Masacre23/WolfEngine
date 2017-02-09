#ifndef GLOBALS_H
#define GLOBALS_H

#include "MemLeaks.h"
#include <cassert>

#define DEG_TO_RAD 0.017453292519943295769236907684886127134428718885417254560f
#define RAD_TO_DEG 57.29577951308232087679815481410517033240547246656432155235f

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

struct Point3d
{
	int x = 0, y = 0, z = 0;

	Point3d(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
};

struct Point3df
{
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Point3df(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }

// Configuration from JSON file -----------
#define CONFIGJSON "config.json"

#endif // !GLOBALS_H
