/*------------------------------------------------------------------------------
Copyright (c) 2009-2017 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

/*
 * @file GRRLIB_collision.h
 * Inline functions for collision detection.
 */

/**
 * Determine whether the specified point lies within the specified rectangle.
 * @param hotx Specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param hoty Specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param hotw The width of the rectangle.
 * @param hoth The height of the rectangle.
 * @param wpadx Specifies the x-coordinate of the point.
 * @param wpady Specifies the y-coordinate of the point.
 * @return If the specified point lies within the rectangle, the return value is @c true otherwise it's @c false.
 */
INLINE
bool  GRRLIB_PtInRect (const int hotx,   const int hoty,
                       const int hotw,   const int hoth,
                       const int wpadx,  const int wpady) {
    return( ((wpadx>=hotx) && (wpadx<=(hotx+hotw))) &&
            ((wpady>=hoty) && (wpady<=(hoty+hoth)))  );
}

/**
 * Determine whether a specified rectangle lies within another rectangle.
 * @param rect1x Specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param rect1y Specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param rect1w Specifies the width of the rectangle.
 * @param rect1h Specifies the height of the rectangle.
 * @param rect2x Specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param rect2y Specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param rect2w Specifies the width of the rectangle.
 * @param rect2h Specifies the height of the rectangle.
 * @return If the specified rectangle lies within the other rectangle, the return value is @c true otherwise it's @c false.
 */
INLINE
bool  GRRLIB_RectInRect (const int rect1x, const int rect1y,
                         const int rect1w, const int rect1h,
                         const int rect2x, const int rect2y,
                         const int rect2w, const int rect2h) {
    return ( (rect1x >= rect2x) && (rect1y >= rect2y) &&
             (rect1x+rect1w <= rect2x+rect2w) &&
             (rect1y+rect1h <= rect2y+rect2h) );
}

/**
 * Determine whether a part of a specified rectangle lies on another rectangle.
 * @param rect1x Specifies the x-coordinate of the upper-left corner of the first rectangle.
 * @param rect1y Specifies the y-coordinate of the upper-left corner of the first rectangle.
 * @param rect1w Specifies the width of the first rectangle.
 * @param rect1h Specifies the height of the first rectangle.
 * @param rect2x Specifies the x-coordinate of the upper-left corner of the second rectangle.
 * @param rect2y Specifies the y-coordinate of the upper-left corner of the second rectangle.
 * @param rect2w Specifies the width of the second rectangle.
 * @param rect2h Specifies the height of the second rectangle.
 * @return If the specified rectangle lies on the other rectangle, the return value is @c true otherwise it's @c false.
 */
INLINE
bool  GRRLIB_RectOnRect (const int rect1x, const int rect1y,
                         const int rect1w, const int rect1h,
                         const int rect2x, const int rect2y,
                         const int rect2w, const int rect2h) {
    return GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x, rect2y) ||
           GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x+rect2w, rect2y) ||
           GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x+rect2w, rect2y+rect2h) ||
           GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x, rect2y+rect2h);
}
