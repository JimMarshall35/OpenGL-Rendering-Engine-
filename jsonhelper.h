#pragma once
#include "rapidjson/document.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

bool loadJSONFile(rapidjson::Document &jsonDoc, const char *pFileName);
