// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define millis()			(uint32_t)::timeGetTime()

#define A0		0
#define A1		1
#define A2		2
#define A3		3
#define A4		4
#define A5		5
#define A6		6
#define A7		7

#define	analogRead(adc)		(uint32_t)(::timeGetTime() & 1023)

