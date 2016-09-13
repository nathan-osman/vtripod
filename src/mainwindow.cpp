/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRect>
#include <QStyle>
#include <QUrl>

#include "mainwindow.h"

MainWindow::MainWindow()
    : mRender(nullptr),
      mConverter(this)
{
    connect(&mConverter, &Converter::finished, this, &MainWindow::onOpenFinished);

    initMenu();

    // Resize and center the window
    resize(600, 400);
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            QApplication::desktop()->availableGeometry()
        )
    );
}

void MainWindow::initMenu()
{
    QMenuBar *bar = new QMenuBar(this);

    // Create the file menu
    QMenu *file = new QMenu(tr("&File"), bar);
    file->addAction(tr("&Open..."), this, SLOT(onOpen()));
    mRender = file->addAction(tr("&Render..."), this, SLOT(onRender()));
    mRender->setEnabled(false);
    file->addSeparator();
    file->addAction(tr("&Quit"), this, SLOT(close()));

    // Create the help menu
    QMenu *help = new QMenu(tr("&Help"), bar);
    help->addAction(tr("&Online Documentation"), this, SLOT(onOnlineDocs()));
    help->addAction(tr("&About VTripod..."), this, SLOT(onAbout()));

    // Add the top-level menus
    bar->addMenu(file);
    bar->addMenu(help);

    setMenuBar(bar);
}

void MainWindow::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (!filename.isNull()) {
        mConverter.convert(filename, mTemp.fileName(), QSize(720, 480));
    }
}

void MainWindow::onOpenFinished(const QString &error)
{
    if (error.isNull()) {
        QMessageBox::information(this, tr("Information"), tr("Conversion completed!"));
    } else {
        QMessageBox::critical(this, tr("Error"), error);
    }
}

void MainWindow::onRender()
{
    //...
}

void MainWindow::onOnlineDocs()
{
    QDesktopServices::openUrl(QUrl("https://github.com/nathan-osman/vtripod"));
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, tr("About VTripod"), tr("Copyright 2016 - Nathan Osman"));
}
