/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2011  hkrn                                    */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAI project team nor the names of     */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#ifndef SCENEMOTIONMODEL_H
#define SCENEMOTIONMODEL_H

#include "models/MotionBaseModel.h"

#include <vpvl/CameraKeyFrame.h>

namespace vpvl {
class Scene;
}

class SceneWidget;

class SceneMotionModel : public MotionBaseModel
{
    Q_OBJECT

public:
    typedef QSharedPointer<vpvl::BaseKeyFrame> KeyFramePtr;
    typedef QPair<int, KeyFramePtr> KeyFramePair;
    typedef QList<KeyFramePair> KeyFramePairList;

    explicit SceneMotionModel(QUndoGroup *undo, const SceneWidget *sceneWidget, QObject *parent = 0);
    ~SceneMotionModel();

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int maxFrameCount() const;
    virtual const QModelIndex frameIndexToModelIndex(ITreeItem *item, int frameIndex) const;

    void saveMotion(vpvl::VMDMotion *motion);
    void addKeyFramesByModelIndices(const QModelIndexList &indices);
    void copyFrames(int frameIndex);
    void pasteFrame(int frameIndex);
    void selectByModelIndices(const QModelIndexList &indices);
    const QByteArray nameFromModelIndex(const QModelIndex &index) const;

    void setFrames(const KeyFramePairList &frames);
    void refreshScene();
    const vpvl::CameraKeyFrame::InterpolationParameter &cameraInterpolationParameter() const {
        return m_cameraInterpolationParameter;
    }
    void setCameraInterpolationParameter(const vpvl::CameraKeyFrame::InterpolationParameter &value) {
        m_cameraInterpolationParameter = value;
    }

public slots:
    virtual void removeMotion();
    virtual void deleteFrameByModelIndex(const QModelIndex &index);
    void loadMotion(vpvl::VMDMotion *motion);
    void markAsNew() { setModified(false); }

signals:
    void cameraFrameDidSelect(const QList<SceneMotionModel::KeyFramePtr> &cameraFrames);

protected:
    virtual ITreeItem *root() const { return m_rootTreeItem; }

private:
    const SceneWidget *m_sceneWidget;
    QModelIndex m_cameraIndex;
    Values m_cameraData;
    vpvl::CameraKeyFrame::InterpolationParameter m_cameraInterpolationParameter;
    ITreeItem *m_rootTreeItem;
    ITreeItem *m_cameraTreeItem;

    Q_DISABLE_COPY(SceneMotionModel)
};

#endif // SCENEMOTIONMODEL_H