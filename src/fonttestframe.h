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

#ifndef FONTTESTFRAME_H
#define FONTTESTFRAME_H

#include <QFrame>

namespace Ui {
    class FontTestFrame;
}

class LayoutData;
struct RendererData;
class FontConfig;

class FontTestFrame : public QFrame {
    Q_OBJECT
public:
    FontTestFrame(QWidget *parent = 0);
    ~FontTestFrame();

    void setLayoutData(const LayoutData* data);
    void setRendererData(const RendererData* data);
    void setFontConfig(const FontConfig* config);
public slots:
    void refresh();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::FontTestFrame *ui;

private slots:
    void on_radioButton_3_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);
    void on_radioButton_toggled(bool checked);
    void on_useKerningCheckBox_toggled(bool checked);
    void on_plainTextEdit_textChanged();
};

#endif // FONTTESTFRAME_H
