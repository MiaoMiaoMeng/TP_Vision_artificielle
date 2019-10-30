// ------------------------------------------------------------------
/**
 * @file EdMeanFiltering.c
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
#include "EdStructures.h"
#include "EdUtilities.h"
#include "EdLibMeanFiltering.h"

main(int argc, char **argv)
{
  EdIMAGE	    *image = NULL,*imres = NULL;
  int	        nlig = 0, ncol = 0;
  unsigned char	prof = 0;
  FILE	        *fichier = NULL, *fichres = NULL;
  int           ret;
  /* --- USAGE --- */
  if(argc != 3)
  {
    fprintf(stderr,"USAGE :  PMoyen image immoy \n");
    fprintf(stderr,"image : name of the image to filter \n");
    fprintf(stderr,"immoy : name of the result image \n");
    fprintf(stderr,"Smooth Filtering by Mean Filter of B&W image \n");
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
  /* --- Result Image  --- */
  if(!(fichres = fopen(*++argv,"wb")))
  {
    fprintf(stderr,"Result Image %s Pb of Opening\n",*argv);
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
    fprintf(stderr," the source image is not a RVB image \n");
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
 
/* --- Reading of Umage Data from file to Memory --- */
  ret = Reading_ImageData(fichier,image); // Image Pixel Data
  if (!ret)
  {
    fprintf(stderr,"Problem of Reading \n");
    system ("PAUSE"); // Windows Only
    exit(0);
  }
  
/* --- Smoothing by Mean Filtering --- */
  ret = MeanFiltering (image, imres);
  if (ret)
  {
    fprintf(stderr,"Problem of Memory Allocation in Mean Filtering \n");
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
  
  fclose (fichier);
  fclose (fichres);
  	
  fprintf(stderr,"End of MeanFiltering Operator\n");
  system("PAUSE"); // Windows Only
}
