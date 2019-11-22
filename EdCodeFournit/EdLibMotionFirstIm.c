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

int MotionDetectionRefFirstIm(EdIMAGE *image, EdIMAGE *imres, EdIMAGE *imref, unsigned char Seuil)
{
  EdPOINT	*point = NULL; /* current and neighbour (voisin in French) points */

  int idif_R, idif_V, idif_B;
  unsigned char dif_R, dif_V, dif_B;

  if(crea_POINT(point) == NULL)          /* Memory Allocation of point */
  {
     fprintf(stderr,"Error of Memory Allocation \n");
     system("PAUSE"); // Windows Only
     return 1;
  }

  /* --- Video Scan of the image, except the Border :
         Smooting by Mean Filtering  --- */
  for(POINT_Y(point) = 0; POINT_Y(point) < NLIG(image);
           POINT_Y(point)++)
  for(POINT_X(point) = 0; POINT_X(point) < NCOL(image);
           POINT_X(point)++)
  {
    idif_R = PIXEL_R(image, point) - PIXEL_R(imref, point);
    idif_R = (idif_R >= 0 ? idif_R : -idif_R );
    dif_R = (unsigned char)idif_R;

    idif_V = PIXEL_V(image, point) - PIXEL_V(imref, point);
    idif_V = (idif_V >= 0 ? idif_V : -idif_V );
    dif_V = (unsigned char)idif_V;

    idif_B = PIXEL_B(image, point) - PIXEL_B(imref, point);
    idif_B = (idif_B >= 0 ? idif_B : -idif_B );
    dif_B = (unsigned char)idif_B;

    if ((dif_R < Seuil) && (dif_V < Seuil) && (dif_B < Seuil))
    {
      /* --- Pas de mouvement detecte --- */
      PIXEL_R(imres, point) = 255;
      PIXEL_V(imres, point) = 255;
      PIXEL_B(imres, point) = 255;
    }
    else
    {
      /* --- Mouvement detecte --- */
      PIXEL_R(imres, point) = PIXEL_R(image, point);
      PIXEL_V(imres, point) = PIXEL_V(image, point);
      PIXEL_B(imres, point) = PIXEL_B(image, point);
    }
  }/* --- End of the Image Video Scan --- */
  /* --- Memory Free  --- */
  free((void *)point);
  return 0;
} /* --- End of the Operator --- */
