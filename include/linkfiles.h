#pragma once

#include "utils.h"

int alreadyLinked(const String &WHAT, const String &WHERE);
bool createLink(const String &WHAT, const String &WHERE, bool exist_ok = false);
void batchCreateLink(const String &FROM, const String &TO, int &LINKED, bool exist_ok = false);
int linkFiles(const String &FROM, const String &TO, bool exist_ok = false);
