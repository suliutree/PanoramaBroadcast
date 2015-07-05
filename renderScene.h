#pragma once

#include "common_header.h"

void initScene(void);
void renderScene(mat4& mModelViewProjectionMatrix, int frameSequence, bool isLoaded);
void releaseScene(void);