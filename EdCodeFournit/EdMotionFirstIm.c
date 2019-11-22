// ------------------------------------------------------------------
/**
 * @file EdMotionFirsIm.c
 *
 * @brief Smooth Filtering using Mean Filter
 * This file is the part "interface with the user"
 *
 * @author Patrick Bonnin
 * @email  patrick.bonnin@gmail.com
 * @date 2012.11.01 : creation.
 * @date 2012.11.01 : last modification.
 */
// ------------------------------------------------------------------
/* COPYRIGHT (C)	2012, P. Bonnin <patrick.bonnin@gmail.com>
 *
 * This  library  is  a free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as  published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This  Library  is  distributed in the hope that it will be useful,
 * but  WITHOUT  ANY  WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULIAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You  should  have received a copy of the GNU Lesser General Public
 * License  along  with  this  library;  if  not,  write  to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
// ------------------------------------------------------------------
/* Modifications :
 * 2012.11.01 : creation
 */
// ------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EdStructures.h"
#include "EdUtilities.h"
#include "EdLibMotionFirstIm.h"

int main(int argc, char **argv)
{
  EdIMAGE	    *image = NULL,*imres = NULL, *imref = NULL;
  int	        nlig = 0, ncol = 0;
  unsigned char	prof = 0;
  FILE	        *fichier = NULL, *fichres = NULL;
  int           ret;
  char          NomGenSrc[255],NomGenRes[255], NomImSrc[255], NomImRes[255];
  int           firstNum, NumIm, iSeuil;
  unsigned char Seuil;

  /* --- USAGE --- */
  if(argc != 5)
  {
    fprintf(stderr,"USAGE :  PMotionFirstIm NomGenSrc NomGenRes FirstIm Seuil \n");
    fprintf(stderr,"image : name of the image to filter \n");
    fprintf(stderr,"immoy : name of the result image \n");
    fprintf(stderr,"Smooth Filtering by Mean Filter of B&W image \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  strcpy (NomGenSrc, *++argv);
  strcpy (NomGenRes, *++argv);
  firstNum = atoi(*++argv);
  iSeuil = atoi(*++argv);
  Seuil = (unsigned char)iSeuil;

  sprintf(NomImSrc,"%s%04d.ppm",NomGenSrc,firstNum);
  fprintf(stderr,"%s %s %d %s %d\n",NomGenSrc,NomGenRes, firstNum, NomImSrc, (int)Seuil);


  /* --- Premiere Image --- */
  if(!(fichier = fopen(NomImSrc,"rb")))
  {
    fprintf(stderr,"Source Image %s Pb of Opening\n",NomImSrc);
    system ("PAUSE"); // Windows Only
    exit(0);
  }

  /* --- Reading of Image Header --- */
  if ((ret = Reading_ImageHeader(fichier, &ncol, &nlig, &prof)))
  {
    fprintf (stderr, "Problem of Reading Image Header \n");
    system("PAUSE");
    exit(0);
  }
  fprintf(stderr, "Size of the Image : %d lines x %d colums : Type : %d\n", nlig,ncol, (int)prof);
  if (prof != 3)
  {
    fprintf(stderr," the source image is not a B&W image \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  /* --- Creation of Images Header and Data --- */
  if (crea_IMAGE(imref) == NULL)	/* creation of Image Header  */
  {
    fprintf(stderr,"Error of Memory Allocation  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  ret = Creation_Image (imref, nlig, ncol, prof); // Image Data
  if (!ret)
  {
    fprintf(stderr,"Error of Memory Allocation of Pixels  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  if (crea_IMAGE(image) == NULL)	/* creation of Image Header  */
  {
    fprintf(stderr,"Error of Memory Allocation  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  ret = Creation_Image (image, nlig, ncol, prof); // Image Data
  if (!ret)
  {
    fprintf(stderr,"Error of Memory Allocation of Pixels  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }

  if (crea_IMAGE(imres) == NULL)
  {
    fprintf(stderr,"Error of Memory Allocation  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  ret = Creation_Image (imres, nlig, ncol, prof);
  if (!ret)
  {
    fprintf(stderr,"Error of Memory Allocation of Pixels  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }

  /* --- Reading of Image Data from file to Memory --- */
  ret = Reading_ImageData(fichier,imref); // Image Pixel Data
  if (!ret)
  {
    fprintf(stderr,"Problem of Reading \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  fclose (fichier);


  NumIm = firstNum;

  /* --- Boucle de Traitements --- */
  while(1)
  {
    NumIm++;
    sprintf(NomImSrc,"%s%04d.ppm",NomGenSrc,NumIm);
    sprintf(NomImRes,"%s%04d.ppm",NomGenRes,NumIm);

    fprintf(stderr,"Traitement : %s \n Resultat %s \n\n",NomImSrc, NomImRes);
    if(!(fichier = fopen(NomImSrc,"rb")))
    {
      fprintf(stderr,"Fin de Sequence %d\n",NumIm);
      break;
    }
    /* --- Result Image  --- */
    if(!(fichres = fopen(NomImRes,"wb")))
    {
      fprintf(stderr,"Result Image %s Pb of Opening\n",NomImRes);
      system ("PAUSE"); // Windows Only
      exit(0);
    }
    /* --- Reading of Image Header --- */
    if ((ret = Reading_ImageHeader(fichier, &ncol, &nlig, &prof)))
    {
      fprintf (stderr, "Problem of Reading Image Header \n");
      system("PAUSE");
      exit(0);
    }
    /* --- Reading of Image Data from file to Memory --- */
    ret = Reading_ImageData(fichier,image); // Image Pixel Data
    if (!ret)
    {
      fprintf(stderr,"Problem of Reading \n");
      system ("PAUSE"); // Windows Only
      exit(0);
    }

    ret = MotionDetectionRefFirstIm(image, imres, imref, Seuil);

    if (ret)
    {
      fprintf(stderr,"Problem of Memory \n");
      system ("PAUSE"); // Windows Only
      exit(0);
    }
    /* --- Writing of the Image Result in File --- */
    fprintf(fichres,"P6\n#creating by EdEnviTI\n%d %d\n255\n",(int)ncol, (int)nlig); // Header
    ret = Writing_ImageData(fichres, imres); // Image Pixel Data
    if (!ret)
    {
      fprintf(stderr,"Problem of Writing \n");
      system ("PAUSE"); // Windows Only
      exit(0);
    }
    fclose (fichier);
    fclose (fichres);
  } /* fin de la boucle de traitements */

/* --- Free of Images --- */
  ret = Free_Image(image);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }
  ret = Free_Image(imres);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }
  ret = Free_Image(imref);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }


  fprintf(stderr,"End of Motion Detection using First Image Reference\n");
  system("PAUSE"); // Windows Only
  return 0;
}
