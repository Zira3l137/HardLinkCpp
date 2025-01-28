#pragma once

#include "utils.h"

int alreadyLinked(const String &WHAT, const String &WHERE);
bool createLink(const String &WHAT, const String &WHERE);
void batchCreateLink(const String &FROM, const String &TO, int &LINKED);
int linkFiles(const String &FROM, const String &TO);
