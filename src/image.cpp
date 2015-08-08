/*************************************************************************
 *
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim 
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * All rights reserved.    
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include <QFile>

#include <math.h>

#include <image.h>
#include <lodepng.h>
#include <config.h>

typedef unsigned char  Byte;

/** Helper struct representing a RGBA color */
struct Color {
   Byte red;
   Byte green;
   Byte blue;
   Byte alpha;
};

const int charSetWidth = 80;
const int charHeight = 12;
const int numChars = 96;

unsigned short charPos[numChars]    = {
   0,  5,  8, 13, 20, 27, 38, 47,
   50, 54, 58, 65, 72, 76, 83, 87,
   91, 98, 105, 112, 119, 126, 133, 140,
   147, 154, 161, 164, 167, 174, 181, 188,
   195, 207, 216, 224, 233, 242, 250, 258,
   267, 276, 279, 286, 294, 301, 312, 321,
   331, 339, 349, 357, 365, 372, 380, 389,
   400, 409, 418, 427, 430, 434, 437, 443,
   450, 453, 460, 467, 474, 481, 488, 492,
   499, 506, 509, 512, 518, 521, 530, 537,
   544, 551, 557, 562, 568, 571, 578, 585,
   594, 600, 607, 613, 617, 620, 624, 631
};

unsigned char charWidth[numChars] = {
   5, 3, 5, 7, 7, 11, 9, 3,
   4, 4, 7, 7, 4, 7, 4, 4,

   7, 7, 7, 7, 7, 7, 7, 7,
   7, 7, 3, 3, 7, 7, 7, 7,
   12, 9, 8, 9, 9, 8, 8, 9,
   9, 3, 7, 8, 7, 11, 9, 10,
   8, 10, 8, 8, 7, 8, 9, 11,
   9, 9, 9, 3, 4, 3, 6, 7,
   3, 7, 7, 7, 7, 7, 4, 7,
   7, 3, 3, 6, 3, 9, 7, 7,
   7, 6, 5, 6, 3, 7, 7, 9,
   6, 7, 6, 4, 3, 4, 7, 5
};

unsigned char fontRaw[charSetWidth *charHeight] = {
   0x02, 0x50, 0x01, 0x06, 0x20, 0x60, 0xc6, 0x04, 0x00, 0x00, 0x00, 0x27,
   0x04, 0x1c, 0x38, 0x11, 0xf1, 0xc7, 0xc7, 0x0e, 0x00, 0x00, 0x00, 0x03,
   0x81, 0xf0, 0x10, 0x7c, 0x1e, 0x3e, 0x1f, 0x9f, 0x87, 0x88, 0x24, 0x09,
   0x09, 0x02, 0x02, 0x41, 0x0f, 0x0f, 0x83, 0xc3, 0xe1, 0xe7, 0xf4, 0x24,
   0x12, 0x22, 0x41, 0x20, 0x9f, 0xce, 0x30, 0x00, 0x10, 0x04, 0x00, 0x01,
   0x00, 0x30, 0x08, 0x12, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x00, 0x00, 0x00, 0x00, 0x01, 0xac, 0x00, 0x00, 0x02, 0x51, 0x43, 0x89,
   0x40, 0x90, 0x49, 0x15, 0x00, 0x00, 0x00, 0x28, 0x9c, 0x22, 0x44, 0x31,
   0x02, 0x20, 0x48, 0x91, 0x00, 0x00, 0x00, 0x04, 0x46, 0x08, 0x28, 0x42,
   0x21, 0x21, 0x10, 0x10, 0x08, 0x48, 0x24, 0x09, 0x11, 0x03, 0x06, 0x61,
   0x10, 0x88, 0x44, 0x22, 0x12, 0x10, 0x84, 0x24, 0x12, 0x22, 0x22, 0x20,
   0x80, 0x4a, 0x11, 0x00, 0x20, 0x04, 0x00, 0x01, 0x00, 0x40, 0x08, 0x00,
   0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
   0x02, 0x22, 0x00, 0x00, 0x02, 0x51, 0x45, 0x49, 0x40, 0x90, 0x89, 0x0a,
   0x00, 0x00, 0x00, 0x48, 0x84, 0x02, 0x04, 0x51, 0x02, 0x00, 0x88, 0x91,
   0x00, 0x00, 0x00, 0x04, 0x44, 0xd4, 0x28, 0x42, 0x40, 0x20, 0x90, 0x10,
   0x10, 0x08, 0x24, 0x09, 0x21, 0x03, 0x06, 0x51, 0x20, 0x48, 0x48, 0x12,
   0x12, 0x00, 0x84, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00, 0x89, 0x12, 0x80,
   0x31, 0xc5, 0x87, 0x0d, 0x1c, 0xe3, 0x4b, 0x12, 0x49, 0x29, 0x16, 0x1c,
   0x58, 0x69, 0x4c, 0xe8, 0x91, 0x44, 0x61, 0x44, 0xf2, 0x22, 0x00, 0x00,
   0x02, 0x07, 0xe5, 0x06, 0x80, 0x60, 0x10, 0x95, 0x08, 0x00, 0x00, 0x48,
   0x84, 0x04, 0x18, 0x51, 0xe2, 0xc0, 0x87, 0x11, 0x24, 0x18, 0x03, 0x00,
   0x89, 0x24, 0x44, 0x42, 0x40, 0x20, 0x90, 0x10, 0x10, 0x08, 0x24, 0x09,
   0x41, 0x02, 0x8a, 0x51, 0x20, 0x48, 0x48, 0x12, 0x11, 0x80, 0x84, 0x22,
   0x21, 0x24, 0x14, 0x11, 0x01, 0x09, 0x14, 0x40, 0x02, 0x26, 0x48, 0x93,
   0x22, 0x44, 0xcc, 0x92, 0x51, 0x36, 0x99, 0x22, 0x64, 0x99, 0x92, 0x48,
   0x91, 0x44, 0x52, 0x44, 0x12, 0x22, 0x00, 0x00, 0x02, 0x01, 0x43, 0x80,
   0x80, 0xa0, 0x10, 0x84, 0x08, 0x00, 0x00, 0x88, 0x84, 0x08, 0x04, 0x90,
   0x13, 0x21, 0x08, 0x8f, 0x00, 0x61, 0xf0, 0xc0, 0x8a, 0x24, 0x44, 0x7c,
   0x40, 0x20, 0x9f, 0x9f, 0x11, 0xcf, 0xe4, 0x09, 0xc1, 0x02, 0x8a, 0x49,
   0x20, 0x4f, 0x88, 0x13, 0xe0, 0x60, 0x84, 0x22, 0x21, 0x54, 0x08, 0x0a,
   0x02, 0x08, 0x90, 0x00, 0x00, 0x24, 0x48, 0x11, 0x22, 0x44, 0x48, 0x92,
   0x61, 0x24, 0x91, 0x22, 0x44, 0x89, 0x10, 0x48, 0x91, 0x24, 0x8c, 0x44,
   0x22, 0x22, 0x64, 0x00, 0x02, 0x07, 0xe1, 0x41, 0x31, 0x14, 0x10, 0x80,
   0x3e, 0x07, 0xc0, 0x88, 0x84, 0x10, 0x05, 0x10, 0x12, 0x21, 0x08, 0x81,
   0x01, 0x80, 0x00, 0x31, 0x0a, 0x24, 0x7c, 0x42, 0x40, 0x20, 0x90, 0x10,
   0x10, 0x48, 0x24, 0x09, 0x21, 0x02, 0x52, 0x45, 0x20, 0x48, 0x08, 0x92,
   0x20, 0x10, 0x84, 0x21, 0x41, 0x54, 0x14, 0x04, 0x04, 0x08, 0x90, 0x00,
   0x01, 0xe4, 0x48, 0x11, 0x3e, 0x44, 0x48, 0x92, 0x61, 0x24, 0x91, 0x22,
   0x44, 0x89, 0x0c, 0x48, 0x8a, 0x24, 0x8c, 0x48, 0x44, 0x21, 0x98, 0x00,
   0x02, 0x02, 0x85, 0x41, 0x49, 0x08, 0x10, 0x80, 0x08, 0x00, 0x00, 0x88,
   0x84, 0x20, 0x45, 0xf9, 0x12, 0x21, 0x08, 0x81, 0x00, 0x61, 0xf0, 0xc1,
   0x0a, 0x68, 0x82, 0x42, 0x40, 0x20, 0x90, 0x10, 0x10, 0x48, 0x24, 0x89,
   0x11, 0x02, 0x52, 0x45, 0x20, 0x48, 0x08, 0x52, 0x12, 0x10, 0x84, 0x21,
   0x40, 0x88, 0x22, 0x04, 0x08, 0x08, 0x90, 0x00, 0x02, 0x24, 0x48, 0x11,
   0x20, 0x44, 0x48, 0x92, 0x51, 0x24, 0x91, 0x22, 0x44, 0x89, 0x02, 0x48,
   0x8a, 0x2a, 0x92, 0x28, 0x42, 0x22, 0x00, 0x00, 0x00, 0x02, 0x85, 0x41,
   0x49, 0x18, 0x10, 0x80, 0x08, 0x00, 0x01, 0x08, 0x84, 0x20, 0x44, 0x11,
   0x12, 0x22, 0x08, 0x91, 0x00, 0x18, 0x03, 0x00, 0x09, 0xb0, 0x82, 0x42,
   0x21, 0x21, 0x10, 0x10, 0x08, 0xc8, 0x24, 0x89, 0x09, 0x02, 0x22, 0x43,
   0x10, 0x88, 0x04, 0x22, 0x12, 0x10, 0x84, 0x20, 0x80, 0x88, 0x22, 0x04,
   0x10, 0x08, 0x50, 0x00, 0x02, 0x26, 0x48, 0x93, 0x22, 0x44, 0xc8, 0x92,
   0x49, 0x24, 0x91, 0x22, 0x64, 0x99, 0x12, 0x49, 0x84, 0x11, 0x21, 0x28,
   0x82, 0x22, 0x00, 0x00, 0x02, 0x02, 0x83, 0x82, 0x30, 0xe4, 0x10, 0x80,
   0x00, 0x20, 0x05, 0x07, 0x04, 0x3e, 0x38, 0x10, 0xe1, 0xc2, 0x07, 0x0e,
   0x24, 0x00, 0x00, 0x01, 0x04, 0x00, 0x82, 0x7c, 0x1e, 0x3e, 0x1f, 0x90,
   0x07, 0x48, 0x24, 0x71, 0x05, 0xf2, 0x22, 0x41, 0x0f, 0x08, 0x03, 0xd2,
   0x11, 0xe0, 0x83, 0xc0, 0x80, 0x88, 0x41, 0x04, 0x1f, 0xc8, 0x50, 0x00,
   0x01, 0xd5, 0x87, 0x0d, 0x1c, 0x43, 0x48, 0x92, 0x45, 0x24, 0x91, 0x1c,
   0x58, 0x69, 0x0c, 0x66, 0x84, 0x11, 0x21, 0x10, 0xf2, 0x22, 0x00, 0x00,
   0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x20, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
   0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x10, 0x02, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x09, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x08, 0x10, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x02,
   0x00, 0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
   0x02, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00,
   0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0xac, 0x00, 0x00
};

static Color palette[] = {
   { 0xff, 0xff, 0xff, 0x00 },
   { 0x00, 0x00, 0x00, 0xff },
   { 0xff, 0xff, 0xc0, 0xff },
   { 0x9f, 0x9f, 0x60, 0xff },
   { 0x90, 0x00, 0x00, 0xff },
   { 0x00, 0x90, 0x00, 0xff },
   { 0x00, 0x00, 0x90, 0xff },
   { 0xc0, 0xc0, 0xc0, 0xff }
};

// for alpha we use x^(1/1.3)
static Color palette2[] = {
   { 0x00, 0x00, 0x00, 0x00 },
   { 0x00, 0x00, 0x00, 0x2e },
   { 0x00, 0x00, 0x00, 0x48 },
   { 0x00, 0x00, 0x00, 0x5d },
   { 0x00, 0x00, 0x00, 0x6f },
   { 0x00, 0x00, 0x00, 0x80 },
   { 0x00, 0x00, 0x00, 0x8f },
   { 0x00, 0x00, 0x00, 0x9e },
   { 0x00, 0x00, 0x00, 0xac },
   { 0x00, 0x00, 0x00, 0xb9 },
   { 0x00, 0x00, 0x00, 0xc5 },
   { 0x00, 0x00, 0x00, 0xd2 },
   { 0x00, 0x00, 0x00, 0xdd },
   { 0x00, 0x00, 0x00, 0xe9 },
   { 0x00, 0x00, 0x00, 0xf4 },
   { 0x00, 0x00, 0x00, 0xff }
};

static Color palette3[] = {
   { 0xff, 0xff, 0xff, 0xff },
   { 0xe0, 0xe0, 0xe0, 0xff },
   { 0xd0, 0xd0, 0xd0, 0xff },
   { 0xc0, 0xc0, 0xc0, 0xff },
   { 0xb0, 0xb0, 0xb0, 0xff },
   { 0xa0, 0xa0, 0xa0, 0xff },
   { 0x90, 0x90, 0x90, 0xff },
   { 0x80, 0x80, 0x80, 0xff },
   { 0x70, 0x70, 0x70, 0xff },
   { 0x60, 0x60, 0x60, 0xff },
   { 0x50, 0x50, 0x50, 0xff },
   { 0x40, 0x40, 0x40, 0xff },
   { 0x30, 0x30, 0x30, 0xff },
   { 0x20, 0x20, 0x20, 0xff },
   { 0x10, 0x10, 0x10, 0xff },
   { 0x00, 0x00, 0x00, 0xff }
};

Image::Image(int w, int h)
{
   static int hue   = Config::getInt("html-colorstyle-hue");
   static int sat   = Config::getInt("html-colorstyle-sat");
   static int gamma = Config::getInt("html-colorstyle-gamma");

   double red1, green1, blue1;
   double red2, green2, blue2;

   ColoredImage::hsl2rgb(hue / 360.0,                // hue
                         sat / 255.0,                // saturation
                         pow(235 / 255.0, gamma / 100.0), // luma (gamma corrected)
                         &red1, &green1, &blue1);

   ColoredImage::hsl2rgb(hue / 360.0,                // hue
                         sat / 255.0,                // saturation
                         pow(138 / 255.0, gamma / 100.0), // luma (gamma corrected)
                         &red2, &green2, &blue2);

   palette[2].red   = (int)(red1   * 255.0);
   palette[2].green = (int)(green1 * 255.0);
   palette[2].blue  = (int)(blue1  * 255.0);

   palette[3].red   = (int)(red2   * 255.0);
   palette[3].green = (int)(green2 * 255.0);
   palette[3].blue  = (int)(blue2  * 255.0);

   data = new uchar[w * h];
   memset(data, 0, w * h);
   width = w;
   height = h;
}

Image::~Image()
{
   delete[] data;
}

void Image::setPixel(int x, int y, uchar val)
{
   if (x >= 0 && x < width && y >= 0 && y < height) {
      data[y * width + x] = val;
   }
}

uchar Image::getPixel(int x, int y) const
{
   if (x >= 0 && x < width && y >= 0 && y < height) {
      return data[y * width + x];
   } else {
      return 0;
   }
}

void Image::writeChar(int x, int y, char c, uchar fg)
{
   if (c >= ' ') {
      int xf, yf, ci = c - ' ';
      int rowOffset = 0;
      int cw = charWidth[ci];
      int cp = charPos[ci];

      for (yf = 0; yf < charHeight; yf++) {
         unsigned short bitPattern = 0;
         int bitsLeft = cw;
         int byteOffset = rowOffset + (cp >> 3);
         int bitOffset  = cp & 7;

         // get the bit pattern for row yf of the character from the font data
         while (bitsLeft > 0) {
            int bits = 8 - bitOffset;
            if (bits > bitsLeft) {
               bits = bitsLeft;
            }
            bitPattern <<= bits;
            bitPattern |= ((fontRaw[byteOffset] << bitOffset) & 0xff) >> (8 - bits);
            bitsLeft -= bits;
            bitOffset = 0;
            byteOffset++;
         }

         int mask = 1 << (cw - 1);

         // draw character row yf
         for (xf = 0; xf < cw; xf++) {
            setPixel(x + xf, y + yf, (bitPattern & mask) ? fg : getPixel(x + xf, y + yf));
            mask >>= 1;
         }
         rowOffset += charSetWidth;
      }
   }
}

void Image::writeString(int x, int y, const QString &text, uchar fg)
{
   if (! text.isEmpty()) {

      QByteArray tmp = text.toUtf8();
      const char *s  = tmp.constData();    

      char c;

      while ((c = *s++)) {
         writeChar(x, y, c, fg);
         x += charWidth[c - ' '];
      }
   }
}

uint Image::stringLength(const QString &text)
{
   int w = 0;

   if (! text.isEmpty()) {
      
      QByteArray tmp = text.toUtf8();
      const char *s  = tmp.constData();  

      char c;

      while ((c = *s++)) {
         w += charWidth[c - ' '];
      }
   }

   return w;
}

void Image::drawHorzLine(int y, int xs, int xe, uchar colIndex, uint mask)
{
   int i = 0;
   int j = 0;

   for (int x = xs; x <= xe; x++, j++) {
      if (j & 1) {
         i++;
      }
      if (mask & (1 << (i & 0x1f))) {
         setPixel(x, y, colIndex);
      }
   }
}

void Image::drawHorzArrow(int y, int xs, int xe, uchar colIndex, uint mask)
{
   drawHorzLine(y, xs, xe, colIndex, mask);
   
   for (int i = 0; i < 6; i++) {
      int h = i >> 1;
      drawVertLine(xe - i, y - h, y + h, colIndex, 0xffffffff);
   }
}

void Image::drawVertLine(int x, int ys, int ye, uchar colIndex, uint mask)
{
   int i = 0;

   for (int y = ys; y <= ye; y++, i++) {
      if (mask & (1 << (i & 0x1f))) {
         setPixel(x, y, colIndex);
      }
   }
}

void Image::drawVertArrow(int x, int ys, int ye, uchar colIndex, uint mask)
{
   drawVertLine(x, ys, ye, colIndex, mask);
   
   for (int i = 0; i < 6; i++) {
      int h = i >> 1;
      drawHorzLine(ys + i, x - h, x + h, colIndex, 0xffffffff);
   }
}

void Image::drawRect(int x, int y, int w, int h, uchar colIndex, uint mask)
{
   drawHorzLine(y, x, x + w - 1, colIndex, mask);
   drawHorzLine(y + h - 1, x, x + w - 1, colIndex, mask);
   drawVertLine(x, y, y + h - 1, colIndex, mask);
   drawVertLine(x + w - 1, y, y + h - 1, colIndex, mask);
}

void Image::fillRect(int x, int y, int lwidth, int lheight, uchar colIndex, uint mask)
{
   int xp, yp, xi, yi;

   for (yp = y, yi = 0; yp < y + lheight; yp++, yi++)  {

      for (xp = x, xi = 0; xp < x + lwidth; xp++, xi++) {
         if (mask & (1 << ((xi + yi) & 0x1f))) {
            setPixel(xp, yp, colIndex);
         }
      }
   }
}

QByteArray Image::convert(int mode)
{
   static bool useTransparency = Config::getBool("formula-transparent");

   uchar *buffer;
   size_t bufferSize;

   LodePNG_Encoder encoder;
   LodePNG_Encoder_init(&encoder);

   int numCols;

   if (mode == 0) {
      numCols = 8; 
   } else {
      numCols = 16;
   }

   Color *pPal = mode == 0  ? palette  : useTransparency ? palette2 : palette3 ;
 
   for (int i = 0; i < numCols; i++, pPal++) {
      LodePNG_InfoColor_addPalette(&encoder.infoPng.color, pPal->red, pPal->green, pPal->blue, pPal->alpha);
   }

   encoder.infoPng.color.colorType = 3;
   encoder.infoRaw.color.colorType = 3;

   LodePNG_encode(&encoder, &buffer, &bufferSize, data, width, height);

   QByteArray retval = QByteArray( (const char *)buffer, bufferSize);
   
   free(buffer);
   LodePNG_Encoder_cleanup(&encoder);

   return retval;
}

void ColoredImage::hsl2rgb(double h, double s, double l, double *pRed, double *pGreen, double *pBlue)
{
   double v;
   double r, g, b;

   r = l;   // default to gray
   g = l;
   b = l;
   v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);

   if (v > 0) {
      double m;
      double sv;
      int sextant;
      double fract, vsf, mid1, mid2;

      m       = l + l - v;
      sv      = (v - m ) / v;
      h      *= 6.0;
      sextant = (int)h;
      fract   = h - sextant;
      vsf     = v * sv * fract;
      mid1    = m + vsf;
      mid2    = v - vsf;

      switch (sextant) {
         case 0:
            r = v;
            g = mid1;
            b = m;
            break;
         case 1:
            r = mid2;
            g = v;
            b = m;
            break;
         case 2:
            r = m;
            g = v;
            b = mid1;
            break;
         case 3:
            r = m;
            g = mid2;
            b = v;
            break;
         case 4:
            r = mid1;
            g = m;
            b = v;
            break;
         case 5:
            r = v;
            g = m;
            b = mid2;
            break;
      }
   }
   *pRed   = r;
   *pGreen = g;
   *pBlue  = b;
}

ColoredImage::ColoredImage(int width, int height, const uchar *greyLevels, const uchar *alphaLevels, int saturation, int hue, int gamma)
{
   m_hasAlpha = alphaLevels != 0;
   m_width    = width;
   m_height   = height;
   m_data     = (uchar *)malloc(width * height * 4);
    
   for (int i = 0; i < width * height; i++) {
      uchar r, g, b, a;
      double red, green, blue;      

      // hue, saturation, luma (gamma corrected)
      double temp = pow(greyLevels[i]/255.0, gamma/100.0);
      hsl2rgb(hue / 360.0, saturation / 255.0, temp, &red, &green, &blue);

      r = (int)(red   * 255.0);
      g = (int)(green * 255.0);
      b = (int)(blue  * 255.0);
      a = alphaLevels ? alphaLevels[i] : 255;

      m_data[i * 4 + 0] = r;
      m_data[i * 4 + 1] = g;
      m_data[i * 4 + 2] = b;
      m_data[i * 4 + 3] = a;
   }
}

ColoredImage::~ColoredImage()
{
   free(m_data);
}

QByteArray ColoredImage::convert()
{ 
   uchar *buffer;
   size_t bufferSize;

   LodePNG_Encoder encoder;
   LodePNG_Encoder_init(&encoder);

   // 2=RGB 24 bit, 6=RGBA 32 bit
   encoder.infoPng.color.colorType = m_hasAlpha ? 6 : 2; 

   // 6=RGBA 32 bit
   encoder.infoRaw.color.colorType = 6; 
   
   LodePNG_encode(&encoder, &buffer, &bufferSize, m_data, m_width, m_height);
 
   QByteArray retval = QByteArray((const char *)buffer, bufferSize);
        
   LodePNG_Encoder_cleanup(&encoder);
   free(buffer);

   return retval;
}
