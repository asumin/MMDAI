/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2013  hkrn                                    */
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
#ifndef VPVL2_IRENDERENGINE_H_
#define VPVL2_IRENDERENGINE_H_

#include "vpvl2/IEffect.h"

namespace vpvl2
{

class IEffect;
class IModel;
class IString;

class VPVL2_API IRenderEngine
{
public:
    enum UpdateOptionFlags {
        kNone = 0,
        kParallelUpdate = 1
    };

    virtual ~IRenderEngine() {}

    virtual IModel *parentModelRef() const = 0;
    virtual bool upload(const IString *dir) = 0;
    virtual void renderModel() = 0;
    virtual void renderEdge() = 0;
    virtual void renderShadow() = 0;
    virtual void renderZPlot() = 0;
    virtual void update() = 0;
    virtual void setUpdateOptions(int options) = 0;
    virtual bool hasPreProcess() const = 0;
    virtual bool hasPostProcess() const = 0;
    virtual void preparePostProcess() = 0;
    virtual void performPreProcess() = 0;
    virtual void performPostProcess(IEffect *nextPostEffect) = 0;
    virtual IEffect *effect(IEffect::ScriptOrderType type) const = 0;
    virtual void setEffect(IEffect::ScriptOrderType type, IEffect *effect, const IString *dir) = 0;
};

} /* namespace vpvl2 */

#endif
