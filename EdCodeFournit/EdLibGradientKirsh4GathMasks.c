// ------------------------------------------------------------------
/**
 * @file EdLibGradientKirsh4GathMask.c
 *
 * @brief Gradient Vector Computation using Kirsh4 algorithm,
 * under polar coordinate form :  norm and argument
 *
 * The computation requires 4 convolutions, gathered
 * in our implementation, in an alone image scan.
 * The cordinate transform : from rectangular to polar
 * is easy and quick to perform.
 *
 * Here, the computation of the four mask are gathered
 *
 *
 * @author Patrick Bonnin
 * @email  patrick.bonnin@gmail.com
 * @date 2012.12.01 : integration
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
 * 2012.12.01 : Integration
 */
// ------------------------------------------------------------------

#include "EdStructures.h"
/* ------------------------------------------------------------ */
/* ---- Gradient Vector Computation :
        Kirsh at 4 Directions algorithm on mono spectral image ---- */

// GathMaskGradientKirsh4 ----------------------------------------------------
/**
 * @brief Gradient Vector Computation in norm and argument
 * on a mono spectral image
 *
 *	@param	image		image to be processed.
 *	@param	imnorm		gradient norm image.
 *	@param  imarg		gradient argument image
 *
 * @return 	none.
 */
// ------------------------------------------------------------------

int GradientKirsh4GathMask(EdIMAGE *image, EdIMAGE *imnorm, EdIMAGE *imarg)
{
  EdPOINT	*point = NULL, *pointv = NULL; /* curren and neighbour points */
  int	i,j;       /*  column and line index of the neighbourhood  */
  int	grad_x, grad_y;      /*  variable to perform computations */
  int	grad_d, grad_i;

  unsigned char	pixel; /* temporary variable  */

  if(crea_POINT(point)== NULL) /* Creation of Points */
  {
     fprintf(stderr,"Pb of MEmory Allocation : EdLibGradientKirsh4 \n");
     return 1;
  }

  if(crea_POINT(pointv)== NULL)
  {
     fprintf(stderr,"Pb of MEmory Allocation : EdLibGradientKirsh4 \n");
     return 1;
  }
  /* --- Initialisation of Image Borders : Set to "0"  --- */
  for(POINT_X(point) = 0; POINT_X(point) < NCOL(image);
           POINT_X(point)++)
  {
    POINT_Y(point) = 0;                  /* first line */
    PIXEL(imnorm, point) = 0;
    PIXEL(imarg, point) = 0;
    POINT_Y(point) = NLIG(image) - 1;    /* last line */
    PIXEL(imnorm, point) = 0;
    PIXEL(imarg, point) = 0;
  } /*--- End of first and last line initialisation --- */

  for(POINT_Y(point) = 0; POINT_Y(point) < NLIG(image);
           POINT_Y(point)++)
  {
    POINT_X(point) = 0;                  /* first column */
    PIXEL(imnorm, point) = 0;
    PIXEL(imarg, point) = 0;

    POINT_X(point) = NCOL(image) - 1;    /* last column */
    PIXEL(imnorm, point) = 0;
    PIXEL(imarg, point) = 0;
  } /*--- End of first and last column initialisation --- */

  /* --- Video Scan of the image, except the Border --- */

  for(POINT_Y(point) = 1; POINT_Y(point) < NLIG(image) - 1;
           POINT_Y(point)++)
  for(POINT_X(point) = 1; POINT_X(point) < NCOL(image) - 1;
           POINT_X(point)++)
  {
  /* --- First Mask : To detect Horizontal Gradients
    according to Ox axis : orientation 0 --- */
/* First : x arg = 0
// mask coefficients  :   |	-1  0  1|
//		          1/3     |	-1  0  1|
//			              |	-1  0  1|
*/
/* Second : d arg = 1
// mask coefficients  :    | 0  1  1 |
//			           1/3 |-1  0  1 |
//			               |-1 -1  0 |
*/
/* Third : y arg = 6
// mask coefficients : 	  |-1  -1  -1 |
//			         1/3  | 0	0   0 |
//				          | 1   1   1 |
*/
/* Fourth : i arg = 7
// mask coefficients : 	 | -1  -1  0 |
//			         1/3 | -1	0  1 |
//				         |  0   1  1 |
*/
//=================================================================================
    grad_x = 0;	/* initialisation */
    grad_y = 0;
    grad_d = 0;
    grad_i = 0;

    POINT_X(pointv) = POINT_X(point) - 1;
    POINT_Y(pointv) = POINT_Y(point) - 1;

    grad_x -= (int)PIXEL_R(image, pointv);  /* M(0,0) = -1 */
    grad_y -= (int)PIXEL_R(image, pointv);
    grad_i -= (int)PIXEL_R(image, pointv);

    POINT_Y(pointv) = POINT_Y(point);

    grad_x -= (int)PIXEL_R(image, pointv);  /* M(0,1) = -1 */
    grad_d -= (int)PIXEL_R(image, pointv);
    grad_i -= (int)PIXEL_R(image, pointv);


    POINT_Y(pointv) = POINT_Y(point) + 1;
    grad_x -= (int)PIXEL_R(image, pointv);  /* M(0,2) = -1 */
    grad_d -= (int)PIXEL_R(image, pointv);
    grad_y += (int)PIXEL_R(image, pointv);

    POINT_X(pointv) = POINT_X(point) + 1;
    POINT_Y(pointv) = POINT_Y(point) - 1;
    grad_x += (int)PIXEL_R(image, pointv);  /* M(2,0) = 1 */
    grad_d += (int)PIXEL_R(image, pointv);
    grad_y -= (int)PIXEL_R(image, pointv);

    POINT_Y(pointv) = POINT_Y(point);
    grad_x += (int)PIXEL_R(image, pointv);  /* M(2,1)= 1 */
    grad_d += (int)PIXEL_R(image, pointv);
    grad_i += (int)PIXEL_R(image, pointv);

    POINT_Y(pointv) = POINT_Y(point) + 1;
    grad_x += (int)PIXEL_R(image, pointv);  /* M(2,2) = 1 */
    grad_y += (int)PIXEL_R(image, pointv);
    grad_i += (int)PIXEL_R(image, pointv);

    POINT_X(pointv) = POINT_X(point);
    POINT_Y(pointv) = POINT_Y(point) - 1;
    grad_d += (int)PIXEL_R(image, pointv);  /* M(1,0) = 1 */
    grad_y -= (int)PIXEL_R(image, pointv);
    grad_i -= (int)PIXEL_R(image, pointv);

    POINT_Y(pointv) = POINT_Y(point) + 1;
    grad_d -= (int)PIXEL_R(image, pointv);  /* M(1,2) = - 1 */
    grad_y += (int)PIXEL_R(image, pointv);
    grad_i += (int)PIXEL_R(image, pointv);

    grad_x = ((grad_x >= 0) ? grad_x : - grad_x);   /* absolute value */
    grad_x /=3;                                     /* Normalization */

    grad_d = ((grad_d >= 0) ? grad_d : - grad_d);   /* absolute value */
    grad_d /=3;                                     /* Normalization */

    grad_y = ((grad_y >= 0) ? grad_y : - grad_y);   /* absolute value */
    grad_y /=3;                                     /* Normalization */

    grad_i = ((grad_i >= 0) ? grad_i : - grad_i);   /* absolute value */
    grad_i /=3;                                     /* Normalization */

    PIXEL(imnorm, point) = (unsigned char)grad_x;
    PIXEL(imarg, point) = 0;

    if (grad_d > PIXEL(imnorm, point))
    {
      PIXEL(imnorm, point) = (unsigned char)grad_d;
      PIXEL(imarg, point) = 1;
    }

    if (grad_y > PIXEL(imnorm, point))
    {
      PIXEL(imnorm, point) = (unsigned char)grad_y;
      PIXEL(imarg, point) = 6;
    }

    if (grad_i > PIXEL(imnorm, point))
    {
      PIXEL(imnorm, point) = (unsigned char)grad_i;
      PIXEL(imarg, point) = 7;
    }
	//=================================================================================
	grad_x = 0;	/* initialisation */
	grad_y = 0;
	grad_d = 0;
	grad_i = 0;

	POINT_X(pointv) = POINT_X(point) - 1;
	POINT_Y(pointv) = POINT_Y(point) - 1;

	grad_x -= (int)PIXEL_V(image, pointv);  /* M(0,0) = -1 */
	grad_y -= (int)PIXEL_V(image, pointv);
	grad_i -= (int)PIXEL_V(image, pointv);

	POINT_Y(pointv) = POINT_Y(point);

	grad_x -= (int)PIXEL_V(image, pointv);  /* M(0,1) = -1 */
	grad_d -= (int)PIXEL_V(image, pointv);
	grad_i -= (int)PIXEL_V(image, pointv);


	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_x -= (int)PIXEL_V(image, pointv);  /* M(0,2) = -1 */
	grad_d -= (int)PIXEL_V(image, pointv);
	grad_y += (int)PIXEL_V(image, pointv);

	POINT_X(pointv) = POINT_X(point) + 1;
	POINT_Y(pointv) = POINT_Y(point) - 1;
	grad_x += (int)PIXEL_V(image, pointv);  /* M(2,0) = 1 */
	grad_d += (int)PIXEL_V(image, pointv);
	grad_y -= (int)PIXEL_V(image, pointv);

	POINT_Y(pointv) = POINT_Y(point);
	grad_x += (int)PIXEL_V(image, pointv);  /* M(2,1)= 1 */
	grad_d += (int)PIXEL_V(image, pointv);
	grad_i += (int)PIXEL_V(image, pointv);

	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_x += (int)PIXEL_V(image, pointv);  /* M(2,2) = 1 */
	grad_y += (int)PIXEL_V(image, pointv);
	grad_i += (int)PIXEL_V(image, pointv);

	POINT_X(pointv) = POINT_X(point);
	POINT_Y(pointv) = POINT_Y(point) - 1;
	grad_d += (int)PIXEL_V(image, pointv);  /* M(1,0) = 1 */
	grad_y -= (int)PIXEL_V(image, pointv);
	grad_i -= (int)PIXEL_V(image, pointv);

	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_d -= (int)PIXEL_V(image, pointv);  /* M(1,2) = - 1 */
	grad_y += (int)PIXEL_V(image, pointv);
	grad_i += (int)PIXEL_V(image, pointv);

	grad_x = ((grad_x >= 0) ? grad_x : -grad_x);   /* absolute value */
	grad_x /= 3;                                     /* Normalization */

	grad_d = ((grad_d >= 0) ? grad_d : -grad_d);   /* absolute value */
	grad_d /= 3;                                     /* Normalization */

	grad_y = ((grad_y >= 0) ? grad_y : -grad_y);   /* absolute value */
	grad_y /= 3;                                     /* Normalization */

	grad_i = ((grad_i >= 0) ? grad_i : -grad_i);   /* absolute value */
	grad_i /= 3;                                     /* Normalization */

	PIXEL(imnorm, point) = (unsigned char)grad_x;
	PIXEL(imarg, point) = 0;

	if (grad_d > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_d;
		PIXEL(imarg, point) = 1;
	}

	if (grad_y > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_y;
		PIXEL(imarg, point) = 6;
	}

	if (grad_i > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_i;
		PIXEL(imarg, point) = 7;
	}
	//=================================================================================
	grad_x = 0;	/* initialisation */
	grad_y = 0;
	grad_d = 0;
	grad_i = 0;

	POINT_X(pointv) = POINT_X(point) - 1;
	POINT_Y(pointv) = POINT_Y(point) - 1;

	grad_x -= (int)PIXEL_B(image, pointv);  /* M(0,0) = -1 */
	grad_y -= (int)PIXEL_B(image, pointv);
	grad_i -= (int)PIXEL_B(image, pointv);

	POINT_Y(pointv) = POINT_Y(point);

	grad_x -= (int)PIXEL_B(image, pointv);  /* M(0,1) = -1 */
	grad_d -= (int)PIXEL_B(image, pointv);
	grad_i -= (int)PIXEL_B(image, pointv);


	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_x -= (int)PIXEL_B(image, pointv);  /* M(0,2) = -1 */
	grad_d -= (int)PIXEL_B(image, pointv);
	grad_y += (int)PIXEL_B(image, pointv);

	POINT_X(pointv) = POINT_X(point) + 1;
	POINT_Y(pointv) = POINT_Y(point) - 1;
	grad_x += (int)PIXEL_B(image, pointv);  /* M(2,0) = 1 */
	grad_d += (int)PIXEL_B(image, pointv);
	grad_y -= (int)PIXEL_B(image, pointv);

	POINT_Y(pointv) = POINT_Y(point);
	grad_x += (int)PIXEL_B(image, pointv);  /* M(2,1)= 1 */
	grad_d += (int)PIXEL_B(image, pointv);
	grad_i += (int)PIXEL_B(image, pointv);

	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_x += (int)PIXEL_B(image, pointv);  /* M(2,2) = 1 */
	grad_y += (int)PIXEL_B(image, pointv);
	grad_i += (int)PIXEL_B(image, pointv);

	POINT_X(pointv) = POINT_X(point);
	POINT_Y(pointv) = POINT_Y(point) - 1;
	grad_d += (int)PIXEL_B(image, pointv);  /* M(1,0) = 1 */
	grad_y -= (int)PIXEL_B(image, pointv);
	grad_i -= (int)PIXEL_B(image, pointv);

	POINT_Y(pointv) = POINT_Y(point) + 1;
	grad_d -= (int)PIXEL_B(image, pointv);  /* M(1,2) = - 1 */
	grad_y += (int)PIXEL_B(image, pointv);
	grad_i += (int)PIXEL_B(image, pointv);

	grad_x = ((grad_x >= 0) ? grad_x : -grad_x);   /* absolute value */
	grad_x /= 3;                                     /* Normalization */

	grad_d = ((grad_d >= 0) ? grad_d : -grad_d);   /* absolute value */
	grad_d /= 3;                                     /* Normalization */

	grad_y = ((grad_y >= 0) ? grad_y : -grad_y);   /* absolute value */
	grad_y /= 3;                                     /* Normalization */

	grad_i = ((grad_i >= 0) ? grad_i : -grad_i);   /* absolute value */
	grad_i /= 3;                                     /* Normalization */

	PIXEL(imnorm, point) = (unsigned char)grad_x;
	PIXEL(imarg, point) = 0;

	if (grad_d > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_d;
		PIXEL(imarg, point) = 1;
	}

	if (grad_y > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_y;
		PIXEL(imarg, point) = 6;
	}

	if (grad_i > PIXEL(imnorm, point))
	{
		PIXEL(imnorm, point) = (unsigned char)grad_i;
		PIXEL(imarg, point) = 7;
	}
	//=================================================================================
  }/* --- fin du balayage de l'image --- */

  free((void *)pointv); 	/* free of point data structure  */
  free((void *)point);
  return 0;
} /* --- End of Operator --- */












