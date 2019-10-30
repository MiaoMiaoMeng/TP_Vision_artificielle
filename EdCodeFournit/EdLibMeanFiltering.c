// ------------------------------------------------------------------
/**
 * @file EdLibMeanFiltering.c
 *
 * @brief Smooth Filtering using Mean Filter
 * This file is the part "Operator" itself
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
#include "EdStructures.h"
#include "EdUtilities.h"

int MeanFiltering(EdIMAGE *image, EdIMAGE *imres)
{
  EdPOINT	*point = NULL, *pointv=NULL; /* current and neighbour (voisin in French) points */
  int	i,j;                             /* index of lines and columns of the 3X3 neighbourhood */
  int	Rmean; 
  int	Vmean;
  int	Bmean;
                                         /* mean variable */
  int Rmin = 100;
  int Rmax = 150;
  int Vmin = 0;
  int Vmax = 255;
  int Bmin = 0;
  int Bmax = 255;
  if(crea_POINT(point) == NULL)          /* Memory Allocation of point */
  {
     fprintf(stderr,"Error of Memory Allocation \n");
     system("PAUSE"); // Windows Only
     return 1; 
  }
  if(crea_POINT(pointv) == NULL) 
  {
     fprintf(stderr,"Error of Memory Allocation \n");
     system("PAUSE"); // Windows Only
     return 1;
  } 
  /* --- Initialisation of Image Borders : Copy of the Original Image  --- */
  for(POINT_X(point) = 0; POINT_X(point) < NCOL(image); 
           POINT_X(point)++)
  {
    POINT_Y(point) = 0;                  /* first line */
	PIXEL_R(imres, point) = PIXEL_R(image, point);
	PIXEL_V(imres, point) = PIXEL_V(image, point);
	PIXEL_B(imres, point) = PIXEL_B(image, point);

    POINT_Y(point) = NLIG(image) - 1;    /* last line */
	PIXEL_R(imres, point) = PIXEL_R(image, point);
	PIXEL_V(imres, point) = PIXEL_V(image, point);
	PIXEL_B(imres, point) = PIXEL_B(image, point);
  } /*--- End of copy of first and last lines --- */ 

  for(POINT_Y(point) = 0; POINT_Y(point) < NLIG(image); 
           POINT_Y(point)++)
  {
    POINT_X(point) = 0;                  /* first column */
	PIXEL_R(imres, point) = PIXEL_R(image, point);
	PIXEL_V(imres, point) = PIXEL_V(image, point);
	PIXEL_B(imres, point) = PIXEL_B(image, point);

    POINT_X(point) = NCOL(image) - 1;    /* last column */
	PIXEL_R(imres, point) = PIXEL_R(image, point);
	PIXEL_V(imres, point) = PIXEL_V(image, point);
	PIXEL_B(imres, point) = PIXEL_B(image, point);
  } /*--- End of copy of first and last columns --- */ 
 
  /* --- Video Scan of the image, except the Border :
         Smooting by Mean Filtering  --- */	
  for(POINT_Y(point) = 1; POINT_Y(point) < NLIG(image) - 1; POINT_Y(point)++)
  for(POINT_X(point) = 1; POINT_X(point) < NCOL(image) - 1; POINT_X(point)++)
  {
	  /*if ((PIXEL_R(image, point) <= Rmax) && (PIXEL_R(image, point) > Rmin)
		  && (PIXEL_V(image, point) < Vmax) && (PIXEL_V(image, point) > Vmin)
		  && (PIXEL_B(image, point) < Bmax) && (PIXEL_B(image, point) > Bmin))
	  {
		  PIXEL_R(imres, point) = (Rmin + Rmax) / 2;
		  PIXEL_V(imres, point) = (Vmin + Vmax) / 2;
		  PIXEL_B(imres, point) = (Bmin + Bmax) / 2;
	  }
	  else
	  {
		  PIXEL_R(imres, point) = 255;
		  PIXEL_V(imres, point) = 255;
		  PIXEL_B(imres, point) = 255;
	  }*/
	  Rmean = 0;
	  Vmean = 0;
	  Bmean = 0;
	  /* ---  Video Scan of the 3x3 neighbourhood --- */
	  for (j = 0; j < 3; j++)
		  for (i = 0; i < 3; i++)
		  {
			  /* Computation of the Image Coordinates of the Neighbour point */
			  POINT_X(pointv) = POINT_X(point) + i - 1;
			  POINT_Y(pointv) = POINT_Y(point) + j - 1;

			  Rmean += PIXEL_R(image, pointv);
			  Vmean += PIXEL_V(image, pointv);
			  Bmean += PIXEL_B(image, pointv);
		  } /* --- End of the Neighbourhood Video Scan --- */

	          Rmean /= 9;    /* Normalisation : At the end ! */
			  Vmean /= 9;
			  Bmean /= 9;

			  PIXEL_R(imres, point) = Rmean;
			  PIXEL_V(imres, point) = Vmean;
			  PIXEL_B(imres, point) = Bmean;

  }
  /* --- End of the Image Video Scan --- */
  /* --- Memory Free  --- */
  free((void *)pointv);
  free((void *)point);
  return 0;
} /* --- End of the Operator --- */  