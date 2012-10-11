/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2012  hkrn                                    */
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

#ifndef VPVM_TIMELINETABWIDGET_H
#define VPVM_TIMELINETABWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QAbstractItemView>
#include "common/SceneWidget.h"
#include "common/VPDFile.h"
#include "vpvl2/IKeyframe.h"

namespace vpvl2 {
class IBone;
class IModel;
class IMorph;
}

class QAbstractButton;
class QButtonGroup;
class QRadioButton;
class QSettings;
class QTabWidget;

namespace vpvm
{

class FrameSelectionDialog;
class FrameWeightDialog;
class InterpolationDialog;
class TimelineWidget;
class BoneMotionModel;
class MorphMotionModel;
class MotionBaseModel;
class SceneMotionModel;

class TimelineTabWidget : public QWidget
{
    Q_OBJECT

public:
    enum Type {
        kBone,
        kMorph,
        kScene
    };

    static const int kSceneTabIndex = 0;
    static const int kBoneTabIndex = 1;
    static const int kMorphTabIndex = 2;
    static const int kInterpolationTabIndex = 3;

    explicit TimelineTabWidget(QSettings *settingsRef,
                               BoneMotionModel *bmm,
                               MorphMotionModel *mmm,
                               SceneMotionModel *smm,
                               QWidget *parent = 0);
    ~TimelineTabWidget();

public slots:
    void addKeyframesFromSelectedIndices();
    void loadPose(VPDFilePtr pose, IModel *model);
    void savePose(VPDFile *pose, IModel *model);
    void selectFrameIndices(int fromIndex, int toIndex);

signals:
    void motionDidSeek(const IKeyframe::TimeIndex &frameIndex, bool forceCameraUpdate, bool forceEvenSame);
    void currentTabDidChange(int type);
    void currentModelDidChange(IModel *model);
    void editModeDidSet(SceneWidget::EditMode mode);

private slots:
    void retranslate();
    void addMorphKeyframesAtCurrentFrameIndex(IMorph *morph);
    void setCurrentFrameIndex(int value);
    void setCurrentFrameIndexZero();
    void insertKeyframesBySelectedIndices();
    void deleteKeyframesBySelectedIndices();
    void copyKeyframes();
    void cutKeyframes();
    void pasteKeyframes();
    void pasteKeyframesWithReverse();
    void nextFrame();
    void previousFrame();
    void setCurrentTabIndex(int index);
    void notifyCurrentTabIndex();
    void toggleBoneEnable(IModel *model);
    void toggleMorphEnable(IModel *model);
    void toggleBoneButtonsByBone(const QList<IBone *> &bones);
    void selectAllRegisteredKeyframes();
    void openFrameSelectionDialog();
    void openFrameWeightDialog();
    void openInterpolationDialog(const QModelIndexList &indices);
    void openInterpolationDialogBySelectedIndices();
    void selectBones(const QList<IBone *> &bones);
    void selectBonesByItemSelection(const QItemSelection &selection);
    void selectButton(QAbstractButton *button);
    void setLastSelectedModel(IModel *model);
    void clearLastSelectedModel();

private:
    void seekFrameIndexFromCurrentFrameIndex(int frameIndex);
    TimelineWidget *currentSelectedTimelineWidgetRef() const;

    QScopedPointer<QTabWidget> m_tabWidget;
    QScopedPointer<TimelineWidget> m_boneTimeline;
    QScopedPointer<TimelineWidget> m_morphTimeline;
    QScopedPointer<TimelineWidget> m_sceneTimeline;
    QScopedPointer<QButtonGroup> m_boneButtonGroup;
    QScopedPointer<QRadioButton> m_boneSelectButton;
    QScopedPointer<QRadioButton> m_boneRotateButton;
    QScopedPointer<QRadioButton> m_boneMoveButton;
    QScopedPointer<FrameSelectionDialog> m_frameSelectionDialog;
    QScopedPointer<FrameWeightDialog> m_frameWeightDialog;
    QScopedPointer<InterpolationDialog> m_interpolationDialog;
    QSettings *m_settingsRef;
    IModel *m_lastSelectedModelRef;

    Q_DISABLE_COPY(TimelineTabWidget)
};

} /* namespace vpvm */

#endif // TIMELINETABWIDGET_H
