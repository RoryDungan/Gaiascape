#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

class AboutBox : public QDialog
{
    Q_OBJECT

private:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

public:
    explicit AboutBox(QWidget* parent = 0)
    {
        verticalLayout = new QVBoxLayout(this);
        label = new QLabel(this);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(this);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);

        setWindowTitle(tr("About Gaiascape"));
        label->setText(tr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                          "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>\n"
                          "<h1>Gaiascape</h1>\n"
                          "<p>Written by Rory Dungan &lt;<a href=\"mailto:rorydungan@gmail.com\">rorydungan@gmail.com</a>&gt; and Dylan Ford &lt;<a href=\"mailto:dylan@fordfam.com\">dylan@fordfam.com</a>&gt;</p>\n"
                          "<p>Artwork by Daniel Galbraith &lt;<a href=\"mailto:dgalbraih2@gmail.com\">dgalbraith2@gmail.com</a>&gt;and Daniel Docherty &lt;<a href=\"mailto:ddocherty.z1@gmail.com\">ddocherty.z1@gmail.com</a>&gt;</p>\n"/* and Joshua Dauth &lt;<a href=\"mailto:kalthar@hotmail.com\">kalthar@hotmail.com</a>&gt;*/
                          "<p>Thanks also to Kito Berg-Taylor for the Qt Ogre intregation code. </p>\n"
                          "<p>Portions Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).  All rights reserved.  Contact: Nokia Corporation (qt-info@nokia.com).  This software contains Qt v.4.8.1.  Qt is licensed under the GNU Lesser General Public License v.2.1, which can be found at http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt. </p>"
                          "<p>Portions Copyright (C) 2000-2011 Torus Knot Software Ltd. OGRE is licensed under the MIT License. "
                          "<p>Uses icons from the Tango Desktop Project &lt;<a href=\"http://tango.freedesktop.org/\">tango.freedesktop.org</a>&gt;</p>\n"
                          "</body></html>"));
        QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(accept()));
        setFixedSize(sizeHint());
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        show();
        setModal(true);
    }

    ~AboutBox()
    {
        delete verticalLayout;
        delete label;
        delete buttonBox;
    };

protected:
    void closeEvent(QCloseEvent * event)
    {
        delete this;
    }
};

#endif // ABOUTBOX_H
