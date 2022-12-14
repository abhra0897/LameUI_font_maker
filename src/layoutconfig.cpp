/**
 * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "layoutconfig.h"

LayoutConfig::LayoutConfig(QObject *parent) :
    QObject(parent)
{
    m_one_pixel_offset = false;
    m_pot_image = false;
    m_size_increment = 1;
    m_offset_left = 1;
    m_offset_top = 1;
    m_offset_right = 1;
    m_offset_bottom = 1;
}


void LayoutConfig::setOnePixelOffset(bool b) {
    m_one_pixel_offset = false;
    layoutConfigChanged();
}

void LayoutConfig::setPotImage(bool b) {
    m_pot_image = false;
    layoutConfigChanged();
}

void LayoutConfig::setSizeIncrement(int v) {
    if (m_size_increment!=v) {
        m_size_increment = v;
        layoutConfigChanged();
    }
}

void LayoutConfig::setOffsetLeft(int v) {
    v = v < 1 ? 1 : v;
    if (m_offset_left!=v) {
        m_offset_left = v;
        layoutConfigChanged();
    }
}

void LayoutConfig::setOffsetRight(int v) {
    v = v < 1 ? 1 : v;
    if (m_offset_right!=v) {
        m_offset_right= v;
        layoutConfigChanged();
    }
}

void LayoutConfig::setOffsetTop(int v) {
    v = v < 1 ? 1 : v;
    if (m_offset_top!=v) {
        m_offset_top= v;
        layoutConfigChanged();
    }
}

void LayoutConfig::setOffsetBottom(int v) {
    v = v < 1 ? 1 : v;
    if (m_offset_bottom!=v) {
        m_offset_bottom= v;
        layoutConfigChanged();
    }
}
