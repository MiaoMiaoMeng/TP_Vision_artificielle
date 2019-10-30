// ------------------------------------------------------------------
/**
 * @file EdLibGradientKirsh4GathMask.h
 *
 * @brief Prototypes of vector Gradient computation in polar coordinate
 * (Norm and Argument) using Kirsh's 4 Algorithm.
 * The computation of masks are gathered
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
#ifndef _GKGM
#define _GKGM
int GradientKirsh4GathMask(EdIMAGE *image, EdIMAGE *imnorm, EdIMAGE *imarg);

#endif
