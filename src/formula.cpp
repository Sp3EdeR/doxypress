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

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>

#include <stdlib.h>
#include <unistd.h>

#include <formula.h>
#include <image.h>
#include <util.h>
#include <message.h>
#include <config.h>
#include <portable.h>
#include <index.h>
#include <doxygen.h>
#include <ftextstream.h>

// must appear after the previous include - resolve soon 
#include <doxy_globals.h>

Formula::Formula(const char *text)
{
   static int count = 0;
   number = count++;
   form = text;
}

Formula::~Formula()
{
}

int Formula::getId()
{
   return number;
}

void FormulaList::generateBitmaps(const char *path)
{
   int x1, y1, x2, y2;
   QDir d(path);

   // store the original directory
   if (!d.exists()) {
      err("Output dir %s does not exist!\n", path);
      exit(1);
   }
   QByteArray oldDir = QDir::currentPath().toUtf8();

   // go to the html output directory (i.e. path)
   QDir::setCurrent(d.absolutePath());
   QDir thisDir;

   // generate a latex file containing one formula per page.
   QByteArray texName = "_formulas.tex";
   QList<int> pagesToGenerate;
     
   QFile f(texName);
   bool formulaError = false;

   if (f.open(QIODevice::WriteOnly)) {
      FTextStream t(&f);

      if (Config_getBool("LATEX_BATCHMODE")) {
         t << "\\batchmode" << endl;
      }

      t << "\\documentclass{article}" << endl;
      t << "\\usepackage{epsfig}" << endl; // for those who want to include images

      QStringList s = Config_getList("EXTRA_PACKAGES");

      for (auto item : s) {
         t << "\\usepackage{" << item << "}\n";        
      }

      t << "\\pagestyle{empty}" << endl;
      t << "\\begin{document}" << endl;

      int page = 0;
     
      for (auto formula : *this) {
         QString resultName;
         resultName = QString("form_%1.png").arg(formula.getId());

         // only formulas for which no image exists are generated
         QFileInfo fi(resultName);

         if (! fi.exists()) {
            // we force a pagebreak after each formula
            t << formula.getFormulaText() << endl << "\\pagebreak\n\n";
            pagesToGenerate.append(page);
         }

         Doxygen::indexList->addImageFile(strdup(qPrintable(resultName)));
         page++;
      }

      t << "\\end{document}" << endl;
      f.close();
   }

   if (pagesToGenerate.count() > 0) {
      // there are new formulas
      //printf("Running latex...\n");
      //system("latex _formulas.tex </dev/null >/dev/null");

      QByteArray latexCmd = Config_getString("LATEX_CMD_NAME");

      if (latexCmd.isEmpty()) {
         latexCmd = "latex";
      }
      portable_sysTimerStart();

      if (portable_system(latexCmd, "_formulas.tex") != 0) {
         err("Problems running latex. Check your installation or look "
             "for typos in _formulas.tex and check _formulas.log!\n");
         formulaError = true;
         //return;
      }

      portable_sysTimerStop();
             
      int pageIndex = 1;
    
      for (auto pageNum : pagesToGenerate) {         
         msg("Generating image form_%d.png for formula\n", pageNum);
         
         QString formBase;
         formBase = QString("_form%1").arg(pageNum);

         // run dvips to convert the page with number pageIndex to an
         // encapsulated postscript.

         char dviArgs[4096];   
         sprintf(dviArgs, "-q -D 600 -E -n 1 -p %d -o %s.eps _formulas.dvi",pageIndex, formBase.data());

         portable_sysTimerStart();

         if (portable_system("dvips", dviArgs) != 0) {
            err("Problems running dvips. Check your installation!\n");
            portable_sysTimerStop();
            return;
         }
         portable_sysTimerStop();

         // now we read the generated postscript file to extract the bounding box
         QFileInfo fi(formBase + ".eps");
 
         if (fi.exists()) {
            QByteArray eps = fileToString(qPrintable(formBase + ".eps"));
            int i = eps.indexOf("%%BoundingBox:");

            if (i != -1) {
               sscanf(eps.data() + i, "%%%%BoundingBox:%d %d %d %d", &x1, &y1, &x2, &y2);

            } else {
               err("Could not extract bounding box!\n");
            }
         }

         // next we generate a postscript file which contains the eps
         // and displays it in the right colors and the right bounding box
         f.setFileName(formBase + ".ps");

         if (f.open(QIODevice::WriteOnly)) {
            FTextStream t(&f);
            t << "1 1 1 setrgbcolor" << endl;  // anti-alias to white background
            t << "newpath" << endl;
            t << "-1 -1 moveto" << endl;
            t << (x2 - x1 + 2) << " -1 lineto" << endl;
            t << (x2 - x1 + 2) << " " << (y2 - y1 + 2) << " lineto" << endl;
            t << "-1 " << (y2 - y1 + 2) << " lineto" << endl;
            t << "closepath" << endl;
            t << "fill" << endl;
            t << -x1 << " " << -y1 << " translate" << endl;
            t << "0 0 0 setrgbcolor" << endl;
            t << "(" << formBase << ".eps) run" << endl;
            f.close();
         }
         // scale the image so that it is four times larger than needed.
         // and the sizes are a multiple of four.
         double scaleFactor = 16.0 / 3.0;
         int zoomFactor = Config_getInt("FORMULA_FONTSIZE");

         if (zoomFactor < 8 || zoomFactor > 50) {
            zoomFactor = 10;
         }

         scaleFactor *= zoomFactor / 10.0;
         int gx = (((int)((x2 - x1) * scaleFactor)) + 3) & ~1;
         int gy = (((int)((y2 - y1) * scaleFactor)) + 3) & ~1;

         // Then we run ghostscript to convert the postscript to a pixmap
         // The pixmap is a truecolor image, where only black and white are
         // used.

         char gsArgs[4096];
         sprintf(gsArgs, "-q -g%dx%d -r%dx%dx -sDEVICE=ppmraw "
                 "-sOutputFile=%s.pnm -dNOPAUSE -dBATCH -- %s.ps",
                 gx, gy, (int)(scaleFactor * 72), (int)(scaleFactor * 72),
                 formBase.data(), formBase.data()
                );

         portable_sysTimerStart();

         if (portable_system(portable_ghostScriptCommand(), gsArgs) != 0) {
            err("Problem running ghostscript %s %s. Check your installation!\n", portable_ghostScriptCommand(), gsArgs);
            portable_sysTimerStop();

            return;
         }

         portable_sysTimerStop();
         f.setFileName(formBase + ".pnm");

         uint imageX = 0, imageY = 0;
         
         // we read the generated image again, to obtain the pixel data.
         if (f.open(QIODevice::ReadOnly)) {
            QTextStream t(&f);
            QByteArray s;

            if (! t.atEnd()) {
               s = t.readLine().toUtf8();
            }

            if (s.length() < 2 || s.left(2) != "P6") {
               err("GhostScript produced an illegal image format.");

            } else {
               // assume the size is after the first line that does not start with
               // # excluding the first line of the file.
             
               while (! t.atEnd()) {            
                  s = t.readLine().toUtf8();
                  
                  if (s.isEmpty()) {
                     break;
                  }   

                  if (s.at(0) != '#') {
                     break;
                  }
               }

               sscanf(s, "%d %d", &imageX, &imageY);
            }

            if (imageX > 0 && imageY > 0) {
               //printf("Converting image...\n");
               char *data = new char[imageX * imageY * 3]; // rgb 8:8:8 format
               uint i, x, y, ix, iy;
               f.read(data, imageX * imageY * 3);

               Image srcImage(imageX, imageY), filteredImage(imageX, imageY), dstImage(imageX / 4, imageY / 4);
               uchar *ps = srcImage.getData();

               // convert image to black (1) and white (0) index.
               for (i = 0; i < imageX * imageY; i++) {
                  *ps++ = (data[i * 3] == 0 ? 1 : 0);
               }
               // apply a simple box filter to the image
               static int filterMask[] = {1, 2, 1, 2, 8, 2, 1, 2, 1};
               for (y = 0; y < srcImage.getHeight(); y++) {
                  for (x = 0; x < srcImage.getWidth(); x++) {
                     int s = 0;
                     for (iy = 0; iy < 2; iy++) {
                        for (ix = 0; ix < 2; ix++) {
                           s += srcImage.getPixel(x + ix - 1, y + iy - 1) * filterMask[iy * 3 + ix];
                        }
                     }
                     filteredImage.setPixel(x, y, s);
                  }
               }
               // down-sample the image to 1/16th of the area using 16 gray scale
               // colors.
               // TODO: optimize this code.
               for (y = 0; y < dstImage.getHeight(); y++) {
                  for (x = 0; x < dstImage.getWidth(); x++) {
                     int xp = x << 2;
                     int yp = y << 2;
                     int c = filteredImage.getPixel(xp + 0, yp + 0) +
                             filteredImage.getPixel(xp + 1, yp + 0) +
                             filteredImage.getPixel(xp + 2, yp + 0) +
                             filteredImage.getPixel(xp + 3, yp + 0) +
                             filteredImage.getPixel(xp + 0, yp + 1) +
                             filteredImage.getPixel(xp + 1, yp + 1) +
                             filteredImage.getPixel(xp + 2, yp + 1) +
                             filteredImage.getPixel(xp + 3, yp + 1) +
                             filteredImage.getPixel(xp + 0, yp + 2) +
                             filteredImage.getPixel(xp + 1, yp + 2) +
                             filteredImage.getPixel(xp + 2, yp + 2) +
                             filteredImage.getPixel(xp + 3, yp + 2) +
                             filteredImage.getPixel(xp + 0, yp + 3) +
                             filteredImage.getPixel(xp + 1, yp + 3) +
                             filteredImage.getPixel(xp + 2, yp + 3) +
                             filteredImage.getPixel(xp + 3, yp + 3);
                     // here we scale and clip the color value so the
                     // resulting image has a reasonable contrast
                     dstImage.setPixel(x, y, qMin(15, (c * 15) / (16 * 10)));
                  }
               }

               // save the result as a bitmap
               QString resultName;
               resultName = QString("form_%1.png").arg(pageNum);

               // the option parameter 1 is used here as a temporary hack
               // to select the right color palette!

               dstImage.save(strdup(qPrintable(resultName)), 1);
               delete[] data;
            }

            f.close();
         }

         // remove intermediate image files
         thisDir.remove(formBase + ".eps");
         thisDir.remove(formBase + ".pnm");
         thisDir.remove(formBase + ".ps");

         ++pageIndex;
      }

      // remove intermediate files produced by latex
      thisDir.remove("_formulas.dvi");

      if (! formulaError) {
         thisDir.remove("_formulas.log");   // keep file in case of errors
      }
      thisDir.remove("_formulas.aux");
   }

   // remove the latex file itself
   if (! formulaError) {
      thisDir.remove("_formulas.tex");
   }

   // write/update the formula repository so we know what text the
   // generated images represent (we use this next time to avoid regeneration
   // of the images, and to avoid forcing the user to delete all images in order
   // to let a browser refresh the images).

   f.setFileName("formula.repository");

   if (f.open(QIODevice::WriteOnly)) {
      FTextStream t(&f);

      for (auto formula : *this) {      
         t << "\\form#" << formula.getId() << ":" << formula.getFormulaText() << endl;
      }
      f.close();
   }

   // reset the directory to the original location.
   QDir::setCurrent(oldDir);
}


#ifdef FORMULA_TEST
int main()
{
   FormulaList fl;
   fl.append(new Formula("$x^2$"));
   fl.append(new Formula("$y^2$"));
   fl.append(new Formula("$\\sqrt{x_0^2+x_1^2+x_2^2}$"));
   fl.generateBitmaps("dest");
   return 0;
}
#endif
