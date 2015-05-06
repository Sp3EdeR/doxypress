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

#ifndef DECLINFO_H
#define DECLINFO_H

#include <QByteArray>

#include <stdio.h>

extern void parseFuncDecl(const QByteArray &decl, bool objC, QByteArray &clName, QByteArray &type,
                  QByteArray &name, QByteArray &args, QByteArray &funcTempList, QByteArray &exceptions);
#endif
