// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Fixed pitch font, extracted from freeglut (freeglut.sf.net)
 */
// Original copyright notice:
/*
 * freeglut_font.c
 *
 * Bitmap and stroke fonts displaying.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 16 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

  #include "./fixfont.hpp"
  #include "../common/ogl.hpp"

NAMESPACE_VIGO_() //***********************************************************

static const GLubyte Fixed8x13_Character_032[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}; /* blank */
static const GLubyte Fixed8x13_Character_097[] = {  8,  0,  0,116,140,132,124,  4,120,  0,  0,  0,  0,  0}; /* "a" */
static const GLubyte Fixed8x13_Character_098[] = {  8,  0,  0,184,196,132,132,196,184,128,128,128,  0,  0};
static const GLubyte Fixed8x13_Character_099[] = {  8,  0,  0,120,132,128,128,132,120,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_100[] = {  8,  0,  0,116,140,132,132,140,116,  4,  4,  4,  0,  0};
static const GLubyte Fixed8x13_Character_101[] = {  8,  0,  0,120,132,128,252,132,120,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_102[] = {  8,  0,  0, 64, 64, 64, 64,248, 64, 64, 68, 56,  0,  0};
static const GLubyte Fixed8x13_Character_103[] = {  8,120,132,120,128,112,136,136,116,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_104[] = {  8,  0,  0,132,132,132,132,196,184,128,128,128,  0,  0};
static const GLubyte Fixed8x13_Character_105[] = {  8,  0,  0,248, 32, 32, 32, 32, 96,  0, 32,  0,  0,  0};
static const GLubyte Fixed8x13_Character_106[] = {  8,112,136,136,  8,  8,  8,  8, 24,  0,  8,  0,  0,  0};
static const GLubyte Fixed8x13_Character_107[] = {  8,  0,  0,132,136,144,224,144,136,128,128,128,  0,  0};
static const GLubyte Fixed8x13_Character_108[] = {  8,  0,  0,248, 32, 32, 32, 32, 32, 32, 32, 96,  0,  0};
static const GLubyte Fixed8x13_Character_109[] = {  8,  0,  0,130,146,146,146,146,236,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_110[] = {  8,  0,  0,132,132,132,132,196,184,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_111[] = {  8,  0,  0,120,132,132,132,132,120,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_112[] = {  8,128,128,128,184,196,132,196,184,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_113[] = {  8,  4,  4,  4,116,140,132,140,116,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_114[] = {  8,  0,  0, 64, 64, 64, 64, 68,184,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_115[] = {  8,  0,  0,120,132, 24, 96,132,120,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_116[] = {  8,  0,  0, 56, 68, 64, 64, 64,248, 64, 64,  0,  0,  0};
static const GLubyte Fixed8x13_Character_117[] = {  8,  0,  0,116,136,136,136,136,136,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_119[] = {  8,  0,  0, 68,170,146,146,130,130,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_118[] = {  8,  0,  0, 32, 80, 80,136,136,136,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_120[] = {  8,  0,  0,132, 72, 48, 48, 72,132,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_121[] = {  8,120,132,  4,116,140,132,132,132,  0,  0,  0,  0,  0};
static const GLubyte Fixed8x13_Character_122[] = {  8,  0,  0,252, 64, 32, 16,  8,252,  0,  0,  0,  0,  0}; /* "z" */
static const GLubyte Fixed8x13_Character_065[] = {  8,  0,  0,132,132,132,252,132,132,132, 72, 48,  0,  0}; /* "A" */
static const GLubyte Fixed8x13_Character_066[] = {  8,  0,  0,252, 66, 66, 66,124, 66, 66, 66,252,  0,  0};
static const GLubyte Fixed8x13_Character_067[] = {  8,  0,  0,120,132,128,128,128,128,128,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_068[] = {  8,  0,  0,252, 66, 66, 66, 66, 66, 66, 66,252,  0,  0};
static const GLubyte Fixed8x13_Character_069[] = {  8,  0,  0,252,128,128,128,240,128,128,128,252,  0,  0};
static const GLubyte Fixed8x13_Character_070[] = {  8,  0,  0,128,128,128,128,240,128,128,128,252,  0,  0};
static const GLubyte Fixed8x13_Character_071[] = {  8,  0,  0,116,140,132,156,128,128,128,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_072[] = {  8,  0,  0,132,132,132,132,252,132,132,132,132,  0,  0};
static const GLubyte Fixed8x13_Character_073[] = {  8,  0,  0,248, 32, 32, 32, 32, 32, 32, 32,248,  0,  0};
static const GLubyte Fixed8x13_Character_074[] = {  8,  0,  0,112,136,  8,  8,  8,  8,  8,  8, 60,  0,  0};
static const GLubyte Fixed8x13_Character_075[] = {  8,  0,  0,132,136,144,160,192,160,144,136,132,  0,  0};
static const GLubyte Fixed8x13_Character_076[] = {  8,  0,  0,252,128,128,128,128,128,128,128,128,  0,  0};
static const GLubyte Fixed8x13_Character_077[] = {  8,  0,  0,130,130,130,146,146,170,198,130,130,  0,  0};
static const GLubyte Fixed8x13_Character_078[] = {  8,  0,  0,132,132,132,140,148,164,196,132,132,  0,  0};
static const GLubyte Fixed8x13_Character_079[] = {  8,  0,  0,120,132,132,132,132,132,132,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_080[] = {  8,  0,  0,128,128,128,128,248,132,132,132,248,  0,  0};
static const GLubyte Fixed8x13_Character_081[] = {  8,  0,  4,120,148,164,132,132,132,132,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_082[] = {  8,  0,  0,132,136,144,160,248,132,132,132,248,  0,  0};
static const GLubyte Fixed8x13_Character_083[] = {  8,  0,  0,120,132,  4,  4,120,128,128,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_084[] = {  8,  0,  0, 16, 16, 16, 16, 16, 16, 16, 16,254,  0,  0};
static const GLubyte Fixed8x13_Character_085[] = {  8,  0,  0,120,132,132,132,132,132,132,132,132,  0,  0};
static const GLubyte Fixed8x13_Character_087[] = {  8,  0,  0, 68,170,146,146,146,130,130,130,130,  0,  0};
static const GLubyte Fixed8x13_Character_086[] = {  8,  0,  0, 16, 40, 40, 40, 68, 68, 68,130,130,  0,  0};
static const GLubyte Fixed8x13_Character_088[] = {  8,  0,  0,130,130, 68, 40, 16, 40, 68,130,130,  0,  0};
static const GLubyte Fixed8x13_Character_089[] = {  8,  0,  0, 16, 16, 16, 16, 16, 40, 68,130,130,  0,  0};
static const GLubyte Fixed8x13_Character_090[] = {  8,  0,  0,252,128,128, 64, 32, 16,  8,  4,252,  0,  0}; /* "Z" */
static const GLubyte Fixed8x13_Character_048[] = {  8,  0,  0, 48, 72,132,132,132,132,132, 72, 48,  0,  0}; /* "0" */
static const GLubyte Fixed8x13_Character_049[] = {  8,  0,  0,248, 32, 32, 32, 32, 32,160, 96, 32,  0,  0};
static const GLubyte Fixed8x13_Character_050[] = {  8,  0,  0,252,128, 64, 48,  8,  4,132,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_051[] = {  8,  0,  0,120,132,  4,  4, 56, 16,  8,  4,252,  0,  0};
static const GLubyte Fixed8x13_Character_052[] = {  8,  0,  0,  8,  8,252,136,136, 72, 40, 24,  8,  0,  0};
static const GLubyte Fixed8x13_Character_053[] = {  8,  0,  0,120,132,  4,  4,196,184,128,128,252,  0,  0};
static const GLubyte Fixed8x13_Character_054[] = {  8,  0,  0,120,132,132,196,184,128,128, 64, 56,  0,  0};
static const GLubyte Fixed8x13_Character_055[] = {  8,  0,  0, 64, 64, 32, 32, 16, 16,  8,  4,252,  0,  0};
static const GLubyte Fixed8x13_Character_056[] = {  8,  0,  0,120,132,132,132,120,132,132,132,120,  0,  0};
static const GLubyte Fixed8x13_Character_057[] = {  8,  0,  0,112,  8,  4,  4,116,140,132,132,120,  0,  0}; /* "9" */
static const GLubyte Fixed8x13_Character_096[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0, 16, 96,224,  0,  0}; /* "`" */
static const GLubyte Fixed8x13_Character_126[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,144,168, 72,  0,  0}; /* "~" */
static const GLubyte Fixed8x13_Character_033[] = {  8,  0,  0,128,  0,128,128,128,128,128,128,128,  0,  0}; /* "!" */
static const GLubyte Fixed8x13_Character_064[] = {  8,  0,  0,120,128,148,172,164,156,132,132,120,  0,  0}; /* "@" */
static const GLubyte Fixed8x13_Character_035[] = {  8,  0,  0,  0, 72, 72,252, 72,252, 72, 72,  0,  0,  0}; /* "#" */
static const GLubyte Fixed8x13_Character_036[] = {  8,  0,  0,  0, 32,240, 40,112,160,120, 32,  0,  0,  0}; /* "$" */
static const GLubyte Fixed8x13_Character_037[] = {  8,  0,  0,136, 84, 72, 32, 16, 16, 72,164, 68,  0,  0}; /* "%" */
static const GLubyte Fixed8x13_Character_094[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,136, 80, 32,  0,  0}; /* "^" */
static const GLubyte Fixed8x13_Character_038[] = {  8,  0,  0,116,136,148, 96,144,144, 96,  0,  0,  0,  0}; /* "&" */
static const GLubyte Fixed8x13_Character_042[] = {  8,  0,  0,  0,  0, 72, 48,252, 48, 72,  0,  0,  0,  0}; /* "*" */
static const GLubyte Fixed8x13_Character_040[] = {  8,  0,  0, 32, 64, 64,128,128,128, 64, 64, 32,  0,  0}; /* "(" */
static const GLubyte Fixed8x13_Character_041[] = {  8,  0,  0,128, 64, 64, 32, 32, 32, 64, 64,128,  0,  0}; /* ")" */
static const GLubyte Fixed8x13_Character_045[] = {  8,  0,  0,  0,  0,  0,  0,252,  0,  0,  0,  0,  0,  0}; /* "-" */
static const GLubyte Fixed8x13_Character_095[] = {  8,  0,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}; /* "_" */
static const GLubyte Fixed8x13_Character_061[] = {  8,  0,  0,  0,  0,252,  0,  0,252,  0,  0,  0,  0,  0}; /* "=" */
static const GLubyte Fixed8x13_Character_043[] = {  8,  0,  0,  0,  0, 32, 32,248, 32, 32,  0,  0,  0,  0}; /* "+" */
static const GLubyte Fixed8x13_Character_091[] = {  8,  0,  0,240,128,128,128,128,128,128,128,240,  0,  0}; /* "[" */
static const GLubyte Fixed8x13_Character_123[] = {  8,  0,  0, 56, 64, 64, 32,192, 32, 64, 64, 56,  0,  0}; /* "{" */
static const GLubyte Fixed8x13_Character_125[] = {  8,  0,  0,224, 16, 16, 32, 24, 32, 16, 16,224,  0,  0}; /* "}" */
static const GLubyte Fixed8x13_Character_093[] = {  8,  0,  0,240, 16, 16, 16, 16, 16, 16, 16,240,  0,  0}; /* "]" */
static const GLubyte Fixed8x13_Character_059[] = {  8,  0,128, 96,112,  0,  0, 32,112, 32,  0,  0,  0,  0}; /* ";" */
static const GLubyte Fixed8x13_Character_058[] = {  8,  0, 64,224, 64,  0,  0, 64,224, 64,  0,  0,  0,  0}; /* ":" */
static const GLubyte Fixed8x13_Character_044[] = {  8,  0,128, 96,112,  0,  0,  0,  0,  0,  0,  0,  0,  0}; /* "," */
static const GLubyte Fixed8x13_Character_046[] = {  8,  0, 64,224, 64,  0,  0,  0,  0,  0,  0,  0,  0,  0}; /* "." */
static const GLubyte Fixed8x13_Character_060[] = {  8,  0,  0,  8, 16, 32, 64,128, 64, 32, 16,  8,  0,  0}; /* "<" */
static const GLubyte Fixed8x13_Character_062[] = {  8,  0,  0,128, 64, 32, 16,  8, 16, 32, 64,128,  0,  0}; /* ">" */
static const GLubyte Fixed8x13_Character_047[] = {  8,  0,  0,128,128, 64, 32, 16,  8,  4,  2,  2,  0,  0}; /* "/" */
static const GLubyte Fixed8x13_Character_063[] = {  8,  0,  0, 16,  0, 16, 16,  8,  4,132,132,120,  0,  0}; /* "?" */
static const GLubyte Fixed8x13_Character_092[] = {  8,  0,  0,  2,  2,  4,  8, 16, 32, 64,128,128,  0,  0}; /* "\" */
static const GLubyte Fixed8x13_Character_034[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,144,144,144,  0,  0}; /* """ */

/* Missing Characters filled in by John Fay by hand ... */
static const GLubyte Fixed8x13_Character_039[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0, 32, 32, 32,  0,  0}; /* """ */
static const GLubyte Fixed8x13_Character_124[] = {  8, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,  0,  0}; /* """ */
/* The font characters mapping: */
static const GLubyte* Fixed8x13_Character_Map[] = {Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_032,Fixed8x13_Character_033,Fixed8x13_Character_034,Fixed8x13_Character_035,Fixed8x13_Character_036,Fixed8x13_Character_037,Fixed8x13_Character_038,Fixed8x13_Character_039,Fixed8x13_Character_040,
   Fixed8x13_Character_041,Fixed8x13_Character_042,Fixed8x13_Character_043,Fixed8x13_Character_044,Fixed8x13_Character_045,Fixed8x13_Character_046,Fixed8x13_Character_047,Fixed8x13_Character_048,Fixed8x13_Character_049,Fixed8x13_Character_050,Fixed8x13_Character_051,Fixed8x13_Character_052,Fixed8x13_Character_053,Fixed8x13_Character_054,Fixed8x13_Character_055,Fixed8x13_Character_056,Fixed8x13_Character_057,Fixed8x13_Character_058,Fixed8x13_Character_059,Fixed8x13_Character_060,Fixed8x13_Character_061,Fixed8x13_Character_062,Fixed8x13_Character_063,Fixed8x13_Character_064,Fixed8x13_Character_065,Fixed8x13_Character_066,Fixed8x13_Character_067,Fixed8x13_Character_068,Fixed8x13_Character_069,Fixed8x13_Character_070,Fixed8x13_Character_071,Fixed8x13_Character_072,Fixed8x13_Character_073,Fixed8x13_Character_074,Fixed8x13_Character_075,Fixed8x13_Character_076,Fixed8x13_Character_077,Fixed8x13_Character_078,Fixed8x13_Character_079,Fixed8x13_Character_080,Fixed8x13_Character_081,Fixed8x13_Character_082,
   Fixed8x13_Character_083,Fixed8x13_Character_084,Fixed8x13_Character_085,Fixed8x13_Character_086,Fixed8x13_Character_087,Fixed8x13_Character_088,Fixed8x13_Character_089,Fixed8x13_Character_090,Fixed8x13_Character_091,Fixed8x13_Character_092,Fixed8x13_Character_093,Fixed8x13_Character_094,Fixed8x13_Character_095,Fixed8x13_Character_096,Fixed8x13_Character_097,Fixed8x13_Character_098,Fixed8x13_Character_099,Fixed8x13_Character_100,Fixed8x13_Character_101,Fixed8x13_Character_102,Fixed8x13_Character_103,Fixed8x13_Character_104,Fixed8x13_Character_105,Fixed8x13_Character_106,Fixed8x13_Character_107,Fixed8x13_Character_108,Fixed8x13_Character_109,Fixed8x13_Character_110,Fixed8x13_Character_111,Fixed8x13_Character_112,Fixed8x13_Character_113,Fixed8x13_Character_114,Fixed8x13_Character_115,Fixed8x13_Character_116,Fixed8x13_Character_117,Fixed8x13_Character_118,Fixed8x13_Character_119,Fixed8x13_Character_120,Fixed8x13_Character_121,Fixed8x13_Character_122,Fixed8x13_Character_123,Fixed8x13_Character_124,
   Fixed8x13_Character_125,Fixed8x13_Character_126,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,
   Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,
   Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,
   Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,Fixed8x13_Character_042,NULL};
  struct SFG_Font
  {
    char*           Name;         /* The source font name             */
    int             Quantity;     /* Number of chars in font          */
    int             Height;       /* Height of the characters         */
    const GLubyte** Characters;   /* The characters mapping           */

    float           xorig, yorig; /* Relative origin of the character */
  };

  static SFG_Font const fgFontFixed8x13 =
    { "-misc-fixed-medium-r-normal--13-120-75-75-C-80-iso8859-1",
      93, 13, Fixed8x13_Character_Map, -1.0f, 2.0f };

  void BitmapCharacter(char c)
  {
    const GLubyte* face;
    SFG_Font const& font = fgFontFixed8x13;

    if(c<1) return;

    /*
     * Find the character we want to draw (??)
     */
    face = font.Characters[c-1];

    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES,  GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST,   GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH,  0        );
    glPixelStorei( GL_UNPACK_SKIP_ROWS,   0        );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0        );
    glPixelStorei( GL_UNPACK_ALIGNMENT,   1        );
    glBitmap(
        face[ 0 ], font.Height,      /* The bitmap's width and height  */
        font.xorig, font.yorig,     /* The origin in the font glyph   */
        ( float )( face[ 0 ] ), 0.0,  /* The raster advance -- inc. x,y */
        ( face + 1 )                  /* The packed bitmap data...      */
    );
    glPopClientAttrib( );
  }

NAMESPACE_END_() // eof
