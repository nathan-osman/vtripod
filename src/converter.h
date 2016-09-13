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

#ifndef CONVERTER_H
#define CONVERTER_H

#include <QProgressDialog>
#include <QSize>
#include <QThread>

#include "converttask.h"

/**
 * @brief Video conversion object
 *
 * Because running the stabilization algorithm on the full-resolution video
 * could be very CPU-intensive, a temporary copy is made at a much smaller
 * resolution that is used for preview.
 *
 * The actual conversion is performed in a separate thread by the ConvertTask
 * class. Signals and slots are used to communicate with the thread.
 */
class Converter : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Create a converter for a file
     */
    Converter();

    /**
     * @brief Destroy the converter
     */
    virtual ~Converter();

    /**
     * @brief Perform the conversion
     * @param srcFilename name of the file being read
     * @param destFilename name of the file being written
     * @param size dimensions of the file being written
     */
    void convert(const QString &srcFilename, const QString &destFilename, const QSize &dimensions);

signals:

    /**
     * @brief Indicate the conversion was complete
     * @param error non-NULL if an error occurred
     *
     * This signal is emitted directly from the task when it completes, either
     * by finishing or encountering an error.
     */
    void finished(const QString &error);

private:

    QThread mThread;
    QProgressDialog mDialog;

    ConvertTask *mTask;
};

#endif // CONVERTER_H
