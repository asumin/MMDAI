/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                2010-2013  hkrn                                    */
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

#pragma once
#ifndef VPVL2_VMD_VMDMOTION_H_
#define VPVL2_VMD_VMDMOTION_H_

#include "vpvl2/IEncoding.h"
#include "vpvl2/IMotion.h"
#include "vpvl2/vmd/BoneAnimation.h"
#include "vpvl2/vmd/CameraAnimation.h"
#include "vpvl2/vmd/LightAnimation.h"
#include "vpvl2/vmd/MorphAnimation.h"

namespace vpvl2
{
namespace vmd
{

/**
 * @file
 * @author Nagoya Institute of Technology Department of Computer Science
 * @author hkrn
 *
 * @section DESCRIPTION
 *
 * Bone class represents a Vocaloid Motion Data object, set of bone, face and camera motion.
 */

class VPVL2_API Motion : public IMotion
{
public:

    /**
      * Type of parsing errors.
      */
    enum Error
    {
        kNoError,
        kInvalidHeaderError,
        kInvalidSignatureError,
        kBoneKeyFramesSizeError,
        kBoneKeyFramesError,
        kMorphKeyFramesSizeError,
        kMorphKeyFramesError,
        kCameraKeyFramesSizeError,
        kCameraKeyFramesError,
        kLightKeyFramesSizeError,
        kLightKeyFramesError,
        kMaxErrors
    };

    struct DataInfo
    {
        const uint8_t *basePtr;
        const uint8_t *namePtr;
        const uint8_t *boneKeyframePtr;
        size_t boneKeyframeCount;
        const uint8_t *morphKeyframePtr;
        size_t morphKeyframeCount;
        const uint8_t *cameraKeyframePtr;
        size_t cameraKeyframeCount;
        const uint8_t *lightKeyframePtr;
        size_t lightKeyframeCount;
        const uint8_t *selfShadowKeyframePtr;
        size_t selfShadowKeyframeCount;
    };

    static const uint8_t *kSignature;
    static const int kSignatureSize = 30;
    static const int kNameSize = 20;

    Motion(IModel *model, IEncoding *encoding);
    ~Motion();

    bool preparse(const uint8_t *data, size_t size, DataInfo &info);
    bool load(const uint8_t *data, size_t size);
    void save(uint8_t *data) const;
    size_t estimateSize() const;
    void setParentSceneRef(Scene *value);
    void setParentModelRef(IModel *value);
    void seek(const IKeyframe::TimeIndex &timeIndex);
    void seekScene(const IKeyframe::TimeIndex &timeIndex, Scene *scene);
    void advance(const IKeyframe::TimeIndex &deltaTimeIndex);
    void advanceScene(const IKeyframe::TimeIndex &deltaTimeIndex, Scene *scene);
    void reload();
    void reset();
    IKeyframe::TimeIndex maxTimeIndex() const;
    bool isReachedTo(const IKeyframe::TimeIndex &atEnd) const;
    bool isNullFrameEnabled() const;
    void setNullFrameEnable(bool value);

    void addKeyframe(IKeyframe *value);
    int countKeyframes(IKeyframe::Type value) const;
    void getKeyframes(const IKeyframe::TimeIndex &timeIndex,
                      const IKeyframe::LayerIndex &layerIndex,
                      IKeyframe::Type type,
                      Array<IKeyframe *> &keyframes);
    IKeyframe::LayerIndex countLayers(const IString *name,
                                      IKeyframe::Type type) const;
    IBoneKeyframe *findBoneKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                    const IString *name,
                                    const IKeyframe::LayerIndex &layerIndex) const;
    IBoneKeyframe *findBoneKeyframeAt(int index) const;
    ICameraKeyframe *findCameraKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                        const IKeyframe::LayerIndex &layerIndex) const;
    ICameraKeyframe *findCameraKeyframeAt(int index) const;
    IEffectKeyframe *findEffectKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                        const IString *name,
                                        const IKeyframe::LayerIndex &layerIndex) const;
    IEffectKeyframe *findEffectKeyframeAt(int index) const;
    ILightKeyframe *findLightKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                      const IKeyframe::LayerIndex &layerIndex) const;
    ILightKeyframe *findLightKeyframeAt(int index) const;
    IModelKeyframe *findModelKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                      const IKeyframe::LayerIndex &layerIndex) const;
    IModelKeyframe *findModelKeyframeAt(int index) const;
    IMorphKeyframe *findMorphKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                      const IString *name,
                                      const IKeyframe::LayerIndex &layerIndex) const;
    IMorphKeyframe *findMorphKeyframeAt(int index) const;
    IProjectKeyframe *findProjectKeyframe(const IKeyframe::TimeIndex &timeIndex,
                                          const IKeyframe::LayerIndex &layerIndex) const;
    IProjectKeyframe *findProjectKeyframeAt(int index) const;
    void replaceKeyframe(IKeyframe *value);
    void deleteKeyframe(IKeyframe *&value);
    void deleteKeyframes(const IKeyframe::TimeIndex &timeIndex, IKeyframe::Type type);
    void update(IKeyframe::Type type);
    IMotion *clone() const;

    const IString *name() const {
        return m_name;
    }
    Scene *parentSceneRef() const {
        return m_parentSceneRef;
    }
    IModel *parentModelRef() const {
        return m_parentModelRef;
    }
    Error error() const {
        return m_error;
    }
    const BoneAnimation &boneAnimation() const {
        return m_boneMotion;
    }
    const CameraAnimation &cameraAnimation() const {
        return m_cameraMotion;
    }
    const MorphAnimation &morphAnimation() const {
        return m_morphMotion;
    }
    const LightAnimation &lightAnimation() const {
        return m_lightMotion;
    }
    const DataInfo &result() const {
        return m_result;
    }
    BoneAnimation *mutableBoneAnimation() {
        return &m_boneMotion;
    }
    CameraAnimation *mutableCameraAnimation() {
        return &m_cameraMotion;
    }
    MorphAnimation *mutableMorphAnimation() {
        return &m_morphMotion;
    }
    LightAnimation *mutableLightAnimation() {
        return &m_lightMotion;
    }
    bool isActive() const {
        return m_active;
    }
    Type type() const {
        return kVMDMotion;
    }

private:
    void parseHeader(const DataInfo &info);
    void parseBoneFrames(const DataInfo &info);
    void parseMorphFrames(const DataInfo &info);
    void parseCameraFrames(const DataInfo &info);
    void parseLightFrames(const DataInfo &info);
    void parseSelfShadowFrames(const DataInfo &info);
    void release();

    mutable IMotion *m_motionPtr;
    Scene *m_parentSceneRef;
    IModel *m_parentModelRef;
    IEncoding *m_encodingRef;
    IString *m_name;
    DataInfo m_result;
    BoneAnimation m_boneMotion;
    CameraAnimation m_cameraMotion;
    MorphAnimation m_morphMotion;
    LightAnimation m_lightMotion;
    Error m_error;
    bool m_active;

    VPVL2_DISABLE_COPY_AND_ASSIGN(Motion)
};

}
}

#endif
