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

#ifndef DIRDEF_H
#define DIRDEF_H

#include <QList>
#include <QStringList>

#include <stringmap.h>
#include <definition.h>

class FileList;
class ClassSDict;
class FileDef;
class OutputList;
class UsedDir;
class FTextStream;
class DirDef;

/** A list of directories. */
class DirList : public QList<DirDef>
{
 public:
   int compareValues(const DirDef *item1, const DirDef *item2) const;
};

/** A model of a directory symbol. */
class DirDef : public Definition
{
 public:
   DirDef(const char *path);
   virtual ~DirDef();

   // accessors
   DefType definitionType() const {
      return TypeDir;
   }

   QByteArray getOutputFileBase() const;
   QByteArray anchor() const {
      return QByteArray();
   }

   bool isLinkableInProject() const;
   bool isLinkable() const;
   QByteArray displayName(bool = true) const {
      return m_dispName;
   }

   const QByteArray &shortName() const {
      return m_shortName;
   }

   void addSubDir(DirDef *subdir);
   FileList    *getFiles() const        {
      return m_fileList;
   }

   void addFile(FileDef *fd);
   const DirList &subDirs() const {
      return m_subdirs;
   }

   bool isCluster() const {
      return m_subdirs.count() > 0;
   }

   int level() const {
      return m_level;
   }

   DirDef *parent() const {
      return m_parent;
   }

   int dirCount() const {
      return m_dirCount;
   }

   const QHash<QString, UsedDir> *usedDirs() const {
      return m_usedDirs;
   }

   bool isParentOf(DirDef *dir) const;
   bool depGraphIsTrivial() const;
   QByteArray shortTitle() const;
   bool hasDetailedDescription() const;

   // generate output
   void writeDocumentation(OutputList &ol);
   void writeDepGraph(FTextStream &t);
   void writeTagFile(FTextStream &t);

   static DirDef *mergeDirectoryInTree(const QByteArray &path);
   bool visited;
   void setDiskName(const QByteArray &name) {
      m_diskName = name;
   }

 private:
   friend void computeDirDependencies();

   void writeDetailedDescription(OutputList &ol, const QByteArray &title);
   void writeBriefDescription(OutputList &ol);
   void writeDirectoryGraph(OutputList &ol);
   void writeSubDirList(OutputList &ol);
   void writeFileList(OutputList &ol);
   void startMemberDeclarations(OutputList &ol);
   void endMemberDeclarations(OutputList &ol);

   void setLevel();
   static DirDef *createNewDir(const char *path);
   static bool matchPath(const QByteArray &path, QStringList &l);
   void addUsesDependency(DirDef *usedDir, FileDef *srcFd,FileDef *dstFd, bool inherited);
   void computeDependencies();

   DirList m_subdirs;
   QByteArray m_dispName;
   QByteArray m_shortName;
   QByteArray m_diskName;
   FileList *m_fileList;                 // list of files in the group
   int m_dirCount;
   int m_level;
   DirDef *m_parent;

   QHash<QString, UsedDir> *m_usedDirs;
};

/** Class representing a pair of FileDef objects */
class FilePair
{
 public:
   FilePair(FileDef *src, FileDef *dst) : m_src(src), m_dst(dst) {}
   const FileDef *source() const {
      return m_src;
   }
   const FileDef *destination() const {
      return m_dst;
   }
 private:
   FileDef *m_src;
   FileDef *m_dst;
};

/** A sorted dictionary of FilePair objects. */
class FilePairDict : public StringMap<QSharedPointer<FilePair>>
{
 public:
   FilePairDict() : StringMap<QSharedPointer<FilePair>>() {}

 private:
   int compareValues(const FilePair *item1, const FilePair *item2) const;
};

/** Usage information of a directory. */
class UsedDir
{
 public:
   UsedDir(DirDef *dir, bool inherited);
   virtual ~UsedDir();

   void addFileDep(FileDef *srcFd, FileDef *dstFd);
   FilePair *findFilePair(const char *name);

   const FilePairDict &filePairs() const {
      return m_filePairs;
   }
   const DirDef *dir() const {
      return m_dir;
   }
   bool inherited() const {
      return m_inherited;
   }

 private:
   DirDef *m_dir;
   FilePairDict m_filePairs;
   bool m_inherited;
};

/** A usage relation between two directories. */
class DirRelation
{
 public:
   DirRelation(const QByteArray &name, DirDef *src, UsedDir *dst)
      : m_name(name), m_src(src), m_dst(dst) {}
   DirDef  *source() const      {
      return m_src;
   }
   UsedDir *destination() const {
      return m_dst;
   }
   void writeDocumentation(OutputList &ol);
   QByteArray getOutputFileBase() const {
      return m_name;
   }

 private:
   QByteArray m_name;
   DirDef  *m_src;
   UsedDir *m_dst;
};

inline int DirList::compareValues(const DirDef *item1, const DirDef *item2) const
{
   return qstricmp(item1->shortName(), item2->shortName());
}

/** A sorted dictionary of DirDef objects. */
class DirSDict : public StringMap<QSharedPointer<DirDef>>
{
 public:
   DirSDict() : StringMap<QSharedPointer<DirDef>>() {}

   int compareValues(const DirDef *item1, const DirDef *item2) const {
      return qstricmp(item1->shortName(), item2->shortName());
   }
};

void buildDirectories();
void generateDirDocs(OutputList &ol);
void computeDirDependencies();
void writeDirDependencyGraph(const char *file);

#endif
