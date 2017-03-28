#include "JsonHandler.h"
#include "Globals.h"
#include "parson\parson.h"
#include "SDL/include/SDL_rect.h"
#include "Math.h"

JSONParser::JSONParser(const char* file)
{
	root_object = json_value_get_object(json_parse_file(file));
}

JSONParser::~JSONParser()
{
}

bool JSONParser::LoadObject(const char* name)
{
	parsing_success = false;

	if (loaded_object == nullptr)
	{
		if (json_object_dothas_value(root_object, name))
		{
			loaded_object = json_object_dotget_object(root_object, name);
			parsing_success = true;
		}
		else
			APPLOG("JSONParser: Unable to load section %s.", name);
	}
	else
		APPLOG("JSONParser: Already one section loaded. Section %s not loaded.", name);
	
	return parsing_success;
}

bool JSONParser::UnloadObject()
{
	bool ret = parsing_success;

	if (parsing_success == false)
		APPLOG("JSONParser: Parsing ended with some errors.");

	parsing_success = true;
	loaded_object = nullptr;
	loaded_array = nullptr;
	return ret;
}

bool JSONParser::LoadArrayInObject(const char * name)
{
	bool ret = false;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
			{
				loaded_array = json_object_get_array(loaded_object, name);
				ret = true;
			}
			else
				APPLOG("JSONParser: Incorrect array value for %s.", name);
		}
		else
			APPLOG("JSONParser: Array %s not found.", name);
	}
	else
		APPLOG("JSONParser: No section loaded. Array %s cannot load.", name);

	return ret;
}

bool JSONParser::GetIntArray(const char* name, int* int_array)
{
	bool ret = true;
	JSON_Array* box_array;
	int count = 0;

	int x0, x1, y0, y1;

	ret = LoadArrayInObject(name);

	for (size_t i = 0; i < json_array_get_count(loaded_array) - 1;)
	{
		box_array = json_array_get_array(loaded_array, i);
		x0 = (int) json_array_get_number(box_array, 0);
		y0 = (int) json_array_get_number(box_array, 1);
		box_array = json_array_get_array(loaded_array, i + 1);
		x1 = (int) json_array_get_number(box_array, 0);
		y1 = (int) json_array_get_number(box_array, 1);
		if (count >= x0 && count <= x1)
		{
			float slope = 1.0f;
			if (x1 != x0)
				slope = ((float)(y1 - y0)) / ((float)(x1 - x0));
			int_array[count] = y0 + (int)(slope * (count - x0));
			count++;
		}
		else
		{
			i++;
		}
	}

	return ret;
}

const char* JSONParser::GetString(const char* name)
{
	const char* ret = nullptr;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONString))
		{
			ret = json_object_dotget_string(loaded_object, name);
		}
		else
		{
			APPLOG("JSONParser: Incorrect string value for %s.", name);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No section loaded.");
		parsing_success = true;
	}

	return ret;
}

int JSONParser::GetInt(const char * name)
{
	int ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONNumber))
		{
			ret = (int)json_object_dotget_number(loaded_object, name);
		}
		else
		{
			APPLOG("JSONParser: Incorrect integer value for %s.", name);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No section loaded.");
		parsing_success = false;
	}

	return ret;
}

float JSONParser::GetFloat(const char * name)
{
	float ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONNumber))
		{
			ret = (float)json_object_dotget_number(loaded_object, name);
		}
		else
		{
			APPLOG("JSONParser: Incorrect float value for %s.", name);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No section loaded.");
		parsing_success = false;
	}

	return ret;
}

bool JSONParser::GetBool(const char * name)
{
	bool ret = false;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONBoolean))
			ret = (json_object_dotget_boolean(loaded_object, name) > 0 ? true : false);
	}

	return ret;
}

bool JSONParser::GetBoolMandatory(const char * name)
{
	bool ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONBoolean))
		{
			ret = (json_object_dotget_boolean(loaded_object, name) > 0 ? true : false);
		}
		else
		{
			APPLOG("JSONParser: Incorrect boolean value for %s.", name);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No section loaded.");
		parsing_success = false;
	}

	return ret;
}

int JSONParser::GetIntFromArray(size_t index_array)
{
	int ret = NULL;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			ret = (int) json_array_get_number(loaded_array, index_array);
		}
		else
		{
			APPLOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No array loaded to extract value.");
		parsing_success = false;
	}

	return ret;
}

const char * JSONParser::GetStringFromArray(size_t index_array)
{
	const char* ret = nullptr;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			ret = json_array_get_string(loaded_array, index_array);
		}
		else
		{
			APPLOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No array loaded to extract value.");
		parsing_success = false;
	}

	return ret;
}

int JSONParser::GetIntFromArrayInArray(size_t array_element, size_t index_array)
{
	int ret = NULL;

	if (loaded_array != nullptr)
	{
		if (array_element < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, array_element);
			if (index_array < json_array_get_count(in_array))
			{
				ret = (int) json_array_get_number(in_array, index_array);
			}
			else
			{
				APPLOG("JSONParser: Error loading element in array. Index %i out of range.", index_array);
				parsing_success = false;
			}
		}
		else
		{
			APPLOG("JSONParser: Error loading array in loaded array. Index %i out of range.", array_element);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No array loaded to extract value.");
		parsing_success = false;
	}

	return ret;
}

const char* JSONParser::GetStringFromArrayInArray(size_t array_element, size_t index_array)
{
	const char* ret = nullptr;

	if (loaded_array != nullptr)
	{
		if (array_element < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, array_element);
			if (index_array < json_array_get_count(in_array))
			{
				ret = json_array_get_string(in_array, index_array);
			}
			else
			{
				APPLOG("JSONParser: Error loading element in array. Index %i out of range.", index_array);
				parsing_success = false;
			}
		}
		else
		{
			APPLOG("JSONParser: Error loading array in loaded array. Index %i out of range.", array_element);
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No array loaded to extract value.");
		parsing_success = false;
	}

	return ret;
}

void JSONParser::GetVector3(const char* name, float3* vector)
{
	if (LoadArrayInObject(name))
	{
		if (json_array_get_count(loaded_array) == 3 )
		{
			vector->x = (float)json_array_get_number(loaded_array, 0);
			vector->y = (float)json_array_get_number(loaded_array, 1);
			vector->z = (float)json_array_get_number(loaded_array, 2);
		}
		else
		{
			APPLOG("JSONParser: Error loading element in loaded array. Incorrect number of elements in array for float3.");
			parsing_success = false;
		}
	}
	else
	{
		APPLOG("JSONParser: No array loaded to extract value.");
		parsing_success = false;
	}
}
