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

#ifndef CONVERTTASK_H
#define CONVERTTASK_H

#include <QMutex>
#include <QObject>
#include <QSize>

/**
 * @brief Video conversion task
 *
 * Uses OpenCV to convert a video file to a smaller "preview" resolution in
 * H.264 format (best chance for hardware acceleration). This class *cannot*
 * be reused. Create a new instance for each conversion.
 */
class ConvertTask : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Create a conversion task for the specified video
     * @param srcFilename name of the file being read
     * @param destFilename name of the file being written
     * @param size dimensions of the file being written
     */
    ConvertTask(const QString &srcFilename, const QString &destFilename, const QSize &dimensions);

signals:

    /**
     * @brief Indicate conversion progress
     * @param value percentage of completion
     */
    void progress(int value);

    /**
     * @brief Indicate the conversion is finished
     * @param error non-NULL if an error occurred
     */
    void finished(const QString &error);

public slots:

    /**
     * @brief Perform the conversion
     */
    void run();

    /**
     * @brief Stop the conversion
     */
    void abort();

private:

    QString mSrcFilename;
    QString mDestFilename;
    QSize mDimensions;

    QMutex mMutex;
    bool mAbort;
};

#endif // CONVERTTASK_H
