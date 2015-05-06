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

#ifndef HTMLENTITY_H
#define HTMLENTITY_H

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QTextStream>

#include <docparser.h>

/** @brief Singleton helper class to map html entities to other formats */
class HtmlEntityMapper
{
 public:
   static HtmlEntityMapper *instance();
   static void deleteInstance();
   DocSymbol::SymType name2sym(const QByteArray &symName) const;
   const char *utf8(DocSymbol::SymType symb, bool useInPrintf = false) const;
   const char *html(DocSymbol::SymType symb, bool useInPrintf = false) const;
   const char *xml(DocSymbol::SymType symb) const;
   const char *docbook(DocSymbol::SymType symb) const;
   const char *latex(DocSymbol::SymType symb) const;
   const char *man(DocSymbol::SymType symb) const;
   const char *rtf(DocSymbol::SymType symb) const;
   const DocSymbol::PerlSymb *perl(DocSymbol::SymType symb) const;
   void  writeXMLSchema(QTextStream &t);

 private:
   void  validate();
   HtmlEntityMapper();
   ~HtmlEntityMapper();
   static HtmlEntityMapper *s_instance;
   QHash<QString, int> *m_name2sym;
};

#endif
