// ------------------------------------------------------------------
/**
 * @file EdGradientKirsh4.c
 *
 * @brief Compte the Gradient Vector (Norm, Argument)
 * using Kirsh 4 algorithm
 *
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
#include "EdStructures.h"
#include "EdUtilities.h"
#include "EdLibGradientKirsh4GathMasks.h"

int main(int argc, char **argv)
{
  EdIMAGE	    *image = NULL,*imnorm = NULL, *imarg = NULL;
  int	        nlig = 0, ncol = 0;
  unsigned char	prof = 0;
  FILE	        *fichier = NULL, *fichnorm = NULL, *ficharg = NULL;
  int           ret;
  /* --- USAGE --- */
  if(argc != 4)
  {
    fprintf(stderr,"USAGE :  GradientKirsh4GathMask image imnorm imarg \n");
    fprintf(stderr,"image : name of the image to filter \n");
    fprintf(stderr,"imnorm : name of the image of the norm of the gradient \n");
    fprintf(stderr,"imarg : name of the image of the argument of the gradient \n");
    fprintf(stderr,"Compute the gradient vector : norm and argument on B&W image according to Kirsh's algorithm \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  /* --- Source Image --- */
  if(!(fichier = fopen(*++argv,"rb")))
  {
    fprintf(stderr,"Source Image %s Pb of Opening\n",*argv);
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  /* --- Result Images : Norm & Argument  --- */
  if(!(fichnorm = fopen(*++argv,"wb")))
  {
    fprintf(stderr,"Norm Result Image %s Pb of Opening\n",*argv);
    system ("PAUSE"); // Windows Only
    exit(0);
  }

  if(!(ficharg = fopen(*++argv,"wb")))
  {
    fprintf(stderr,"Argument Result Image %s Pb of Opening\n",*argv);
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
    fprintf(stderr," the source image is not a RGB image \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  /* --- Creation of Images Header and Data --- */
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

  if (crea_IMAGE(imnorm) == NULL)
  {
    fprintf(stderr,"Error of Memory Allocation  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  ret = Creation_Image (imnorm, nlig, ncol, prof);
  if (!ret)
  {
    fprintf(stderr,"Error of Memory Allocation of Pixels  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }

  if (crea_IMAGE(imarg) == NULL)
  {
    fprintf(stderr,"Error of Memory Allocation  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }
  ret = Creation_Image (imarg, nlig, ncol, prof);
  if (!ret)
  {
    fprintf(stderr,"Error of Memory Allocation of Pixels  \n");
    system ("PAUSE"); // Windows Only
    exit (0);
  }

  /* --- Reading of Umage Data from file to Memory --- */
  ret = Reading_ImageData(fichier,image); // Image Pixel Data
  if (!ret)
  {
    fprintf(stderr,"Problem of Reading \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }

  /* --- Gradient Vecteur Comutation : Kirsh4 Algorithm --- */
  // ret = GradientKirsh4SepMask (image, imnorm, imarg);
  ret = GradientKirsh4GathMask (image, imnorm, imarg);
  if (ret)
  {
    fprintf(stderr,"Problem of Memory Allocation in Kirsh 4 Operator \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }

  /* --- Writing of the Images Result in File : Norm and Argument --- */
  fprintf(fichnorm,"P6\n#creating by EdEnviTI\n%d %d\n255\n",(int)ncol, (int)nlig); // Header
  ret = Writing_ImageData(fichnorm, imnorm); // Image Pixel Data
  if (!ret)
  {
    fprintf(stderr,"Problem of Writing \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }

  fprintf(ficharg,"P6\n#creating by EdEnviTI\n%d %d\n255\n",(int)ncol, (int)nlig); // Header
  ret = Writing_ImageData(ficharg, imarg); // Image Pixel Data
  if (!ret)
  {
    fprintf(stderr,"Problem of Writing \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
/* --- Free of Images --- */
  ret = Free_Image(image);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }
  ret = Free_Image(imnorm);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }
  ret = Free_Image(imarg);
  if(ret == FALSE)
  {
    fprintf(stderr,"Problem of Free the Memory \n");
  }

  fclose (fichier);
  fclose (fichnorm);
  fclose (ficharg);

  fprintf(stderr,"End ofEdGradientKirsh4GathMasks Operator\n");
  return 0;
}
