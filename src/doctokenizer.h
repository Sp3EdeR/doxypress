/*************************************************************************
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim 
 * All rights reserved.    
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#ifndef DOCTOKENIZER_H
#define DOCTOKENIZER_H

#include <QByteArray>
#include <QList>

#include <stdio.h>
#include <htmlattrib.h>

class Definition;
class MemberGroup;

enum Tokens {
   TK_WORD          = 1,
   TK_LNKWORD       = 2,
   TK_WHITESPACE    = 3,
   TK_LISTITEM      = 4,
   TK_ENDLIST       = 5,
   TK_COMMAND       = 6,
   TK_HTMLTAG       = 7,
   TK_SYMBOL        = 8,
   TK_NEWPARA       = 9,
   TK_RCSTAG        = 10,
   TK_URL           = 11,

   RetVal_OK             = 0x10000,
   RetVal_SimpleSec      = 0x10001,
   RetVal_ListItem       = 0x10002,
   RetVal_Section        = 0x10003,
   RetVal_Subsection     = 0x10004,
   RetVal_Subsubsection  = 0x10005,
   RetVal_Paragraph      = 0x10006,
   RetVal_SubParagraph   = 0x10007,
   RetVal_EndList        = 0x10008,
   RetVal_EndPre         = 0x10009,
   RetVal_DescData       = 0x1000A,
   RetVal_DescTitle      = 0x1000B,
   RetVal_EndDesc        = 0x1000C,
   RetVal_TableRow       = 0x1000D,
   RetVal_TableCell      = 0x1000E,
   RetVal_TableHCell     = 0x1000F,
   RetVal_EndTable       = 0x10010,
   RetVal_Internal       = 0x10011,
   RetVal_SwitchLang     = 0x10012,
   RetVal_CloseXml       = 0x10013,
   RetVal_EndBlockQuote  = 0x10014,
   RetVal_CopyDoc        = 0x10015,
   RetVal_EndInternal    = 0x10016,
   RetVal_EndParBlock    = 0x10017,
   RetVal_EndDiv         = 0x10018
};

/** @brief Data associated with a token used by the comment block parser. */
struct TokenInfo {
   // unknown token
   char unknownChar;

   // command token
   QByteArray name;

   // command text (RCS tag)
   QByteArray text;

   // comment blocks

   // list token info
   bool isEnumList;
   int indent;

   // sections
   QByteArray sectionId;

   // simple section
   QByteArray simpleSectName;
   QByteArray simpleSectText;

   // verbatim fragment
   QByteArray verb;

   // xrefitem
   int id;

   // html tag
   HtmlAttribList attribs;
   bool endTag;
   bool emptyTag;

   // whitespace
   QByteArray chars;

   // url
   bool isEMailAddr;

   // param attributes
   enum ParamDir { In = 1, Out = 2, InOut = 3, Unspecified = 0 };
   ParamDir paramDir;
};

// globals
extern TokenInfo *g_token;
extern int doctokenizerYYlineno;
extern FILE *doctokenizerYYin;

// helper functions
const char *tokToString(int token);

// operations on the scanner
void doctokenizerYYFindSections(const char *input, QSharedPointer<Definition> d, MemberGroup *mg, const char *fileName);
void doctokenizerYYinit(const char *input, const char *fileName);
void doctokenizerYYcleanup();
void doctokenizerYYpushContext();
bool doctokenizerYYpopContext();
int  doctokenizerYYlex();
void doctokenizerYYsetStatePara();
void doctokenizerYYsetStateTitle();
void doctokenizerYYsetStateTitleAttrValue();
void doctokenizerYYsetStateCode();
void doctokenizerYYsetStateXmlCode();
void doctokenizerYYsetStateHtmlOnly();
void doctokenizerYYsetStateManOnly();
void doctokenizerYYsetStateLatexOnly();
void doctokenizerYYsetStateXmlOnly();
void doctokenizerYYsetStateDbOnly();
void doctokenizerYYsetStateRtfOnly();
void doctokenizerYYsetStateVerbatim();
void doctokenizerYYsetStateDot();
void doctokenizerYYsetStateMsc();
void doctokenizerYYsetStateParam();
void doctokenizerYYsetStateXRefItem();
void doctokenizerYYsetStateFile();
void doctokenizerYYsetStatePattern();
void doctokenizerYYsetStateLink();
void doctokenizerYYsetStateCite();
void doctokenizerYYsetStateRef();
void doctokenizerYYsetStateInternalRef();
void doctokenizerYYsetStateText();
void doctokenizerYYsetStateSkipTitle();
void doctokenizerYYsetStateAnchor();
void doctokenizerYYsetInsidePre(bool b);
void doctokenizerYYpushBackHtmlTag(const char *tag);
void doctokenizerYYsetStateSnippet();
void doctokenizerYYstartAutoList();
void doctokenizerYYendAutoList();
void doctokenizerYYsetStatePlantUML();
void doctokenizerYYsetStateSetScope();

#endif
