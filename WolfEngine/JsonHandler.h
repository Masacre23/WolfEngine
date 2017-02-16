#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "Point.h"
#include "Globals.h"
#include "MathGeoLib/src/Math/float3.h"

typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;

struct SDL_Rect;
class Animation;

class JSONParser
{
public:
	JSONParser(const char* file);
	~JSONParser();

	bool LoadObject(const char* name);
	bool UnloadObject();
	bool LoadArrayInObject(const char* name);

	bool GetIntArray(const char* name, int* int_array);

	const char* GetString(const char* name);
	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);
	bool GetBoolMandatory(const char* name);

	int GetIntFromArray(size_t index_array);
	const char* GetStringFromArray(size_t index_array);
	int GetIntFromArrayInArray(size_t array_element, size_t index_array);
	const char* GetStringFromArrayInArray(size_t array_element, size_t index_array);

	void GetVector3(const char* name, float3* vector);

private:
	JSON_Object* root_object = nullptr;
	JSON_Object* loaded_object = nullptr;
	JSON_Array* loaded_array = nullptr;
	bool parsing_success = true;
};

#endif // !JSONHANDLER_H