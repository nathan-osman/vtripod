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

#include <QProgressDialog>

#include "converter.h"

Converter::Converter(QWidget *parent)
    : mTask(nullptr),
      mParent(parent)
{
    mThread.start();
}

Converter::~Converter()
{
    if (mTask) {
        mTask->abort();
    }

    mThread.quit();
    mThread.wait();
}

void Converter::convert(const QString &srcFilename, const QString &destFilename, const QSize &dimensions)
{
    mTask = new ConvertTask(srcFilename, destFilename, dimensions);
    mTask->moveToThread(&mThread);

    QProgressDialog *dialog = new QProgressDialog(mParent);
    dialog->setLabelText(tr("Converting video..."));
    dialog->setMinimumDuration(0);
    dialog->setWindowModality(Qt::WindowModal);

    connect(mTask, &ConvertTask::progress, dialog, &QProgressDialog::setValue);
    connect(mTask, &ConvertTask::finished, dialog, &QProgressDialog::hide);
    connect(mTask, &ConvertTask::finished, this, &Converter::finished);
    connect(mTask, &ConvertTask::finished, mTask, [this, dialog]() {
        dialog->deleteLater();
        mTask->deleteLater();
        mTask = nullptr;
    });
    connect(dialog, &QProgressDialog::canceled, mTask, &ConvertTask::abort, Qt::DirectConnection);

    QMetaObject::invokeMethod(mTask, "run", Qt::QueuedConnection);
}
