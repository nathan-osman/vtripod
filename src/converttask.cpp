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

#include <QMutexLocker>

#include <opencv2/opencv.hpp>

#include "converttask.h"

ConvertTask::ConvertTask(const QString &srcFilename, const QString &destFilename, const QSize &dimensions)
    : mSrcFilename(srcFilename),
      mDestFilename(destFilename),
      mDimensions(dimensions),
      mAbort(false)
{
}

void ConvertTask::run()
{
    cv::VideoCapture cap(mSrcFilename.toStdString());
    if (!cap.isOpened()) {
        emit finished(tr("OpenCV cannot read %1.").arg(mSrcFilename));
        return;
    }

    // Retrieve a few basic properties
    double fps = cap.get(CV_CAP_PROP_FPS);
    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    double totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);

    // Ensure that the properties are valid (prevent divide-by-zero errors)
    if (!fps || !width || !height || !totalFrames) {
        emit finished(tr("Invalid video properties."));
        return;
    }

    // Calculate the appropriate size based on the aspect ratio
    double wScale = static_cast<double>(mDimensions.width()) / width;
    double hScale = static_cast<double>(mDimensions.height()) / height;
    double scale = qMin(wScale, hScale);

    CvSize size = {
        static_cast<int>(width * scale),
        static_cast<int>(height * scale)
    };

    cv::VideoWriter enc(
        mDestFilename.toStdString(),
        CV_FOURCC('M', 'J', 'P', 'G'),
        fps,
        size
    );
    if (!enc.isOpened()) {
        emit finished(tr("OpenCV cannot open %1.").arg(mDestFilename));
        return;
    }

    forever {

        // Decode the next frame
        cv::Mat src;
        if (!cap.read(src)) {
            break;
        }

        // Resize the frame
        cv::Mat dest;
        cv::resize(src, dest, size);

        // Encode the frame
        enc.write(dest);

        // Calculate file decoding progress
        double curFrame = cap.get(CV_CAP_PROP_POS_FRAMES);
        emit progress(static_cast<int>(curFrame / totalFrames * 100.0));

        // Check to see if the user wanted to abort the conversion
        {
            QMutexLocker locker(&mMutex);
            Q_UNUSED(locker)

            if (mAbort) {
                emit finished(tr("Conversion aborted by user."));
                return;
            }
        }
    }

    emit finished(QString());
}

void ConvertTask::abort()
{
    QMutexLocker locker(&mMutex);
    Q_UNUSED(locker)

    mAbort = true;
}
