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

#include <QtGui/QtGui>
#include <vpvl2/vpvl2.h>
#include <vpvl2/qt/CString.h>
#include <vpvl2/qt/Encoding.h>
#include <libxml/xmlwriter.h>
#include <portaudio.h>
#include "common/Application.h"
#include "common/LoggerWidget.h"
#include "common/util.h"
#include "video/VideoEncoder.h"
#include "MainWindow.h"

using namespace vpvl2;
using namespace vpvl2::qt;

static void SetSearchPaths(const QCoreApplication &app)
{
    QStringList paths;
    QDir appDir(app.applicationDirPath()), appBaseDir = appDir;
#if defined(Q_OS_MAC)
    if (appDir.dirName() == "MacOS") {
        appDir.cdUp();
        // macdeployqt deploys on "@executable_path/../Plugins/"
        // It should be loaded Qt's plugins.
        app.addLibraryPath(appDir.absoluteFilePath("PlugIns"));
        appDir.cdUp();
        appDir.cdUp();
    }
#else
    app.addLibraryPath(appDir.absoluteFilePath("plugins"));
#endif
    const QString applicationPath(appDir.absolutePath());
    /* set path to find configurations (e.g. MMDAI.fst) */
#ifdef QMA_CONFIG_PATH
    const QString configPath(QMA_CONFIG_PATH);
#else
    const QString configPath(applicationPath);
#endif
    paths.clear();
    paths.append(configPath);
    QDir::setSearchPaths("MMDAIUserData", paths);

    /* set path to find resources such as model, motion etc. */
#ifdef QMA_RESOURCE_PATH
    const QString resourcePath(QMA_RESOURCE_PATH);
#elif defined(Q_OS_MAC)
    const QString resourcePath(QDir::cleanPath(appBaseDir.absoluteFilePath("../Resources")));
#else
    const QString resourcePath(applicationPath + "/resources");
#endif
    paths.clear();
    paths.append(resourcePath);
    QDir::setSearchPaths("MMDAIResources", paths);

    /* load translation files from Qt's system path and resource path */
#ifdef QMA_TRANSLATION_PATH
    const QString translationPath(QMA_TRANSLATION_PATH);
#elif defined(Q_OS_MAC)
    const QString translationPath(QDir::cleanPath(appBaseDir.absoluteFilePath("../Resources")));
#else
    const QString translationPath(applicationPath + "/locales");
#endif
    paths.clear();
    paths.append(translationPath);
    QDir::setSearchPaths("MMDAITranslations", paths);
}

typedef QSharedPointer<QTranslator> QTranslatorPtr;

static void LoadTranslations(QCoreApplication &app, QList<QTranslatorPtr> &translators)
{
    const QString &dir = QDir("MMDAITranslations:/").absolutePath();
    const QString &locale = QLocale::system().name();
    QTranslator *translator = new QTranslator();
    translator->load("qt_" + locale, dir);
    app.installTranslator(translator);
    translators.append(QTranslatorPtr(translator));
    translator = new QTranslator();
    translator->load("VPVM_" + locale, dir);
    app.installTranslator(translator);
    translators.append(QTranslatorPtr(translator));
}

int main(int argc, char *argv[])
{
    LIBXML_TEST_VERSION;
    xmlInitMemory();
    xmlInitCharEncodingHandlers();
    xmlInitGlobals();
    xmlInitParser();

    vpvm::Application a(argc, argv);
    QList<QTranslatorPtr> translators;
    a.setApplicationName("MMDAI2");
    a.setApplicationVersion("0.25.1");
    a.setOrganizationDomain("mmdai.github.com");
    a.setOrganizationName("MMDAI");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    SetSearchPaths(a);
    LoadTranslations(a, translators);

    int result = -1;
    if (!vpvl2::isLibraryVersionCorrect(VPVL2_VERSION)) {
        QWidget fake;
        vpvm::warning(&fake,
                      QApplication::tr("libvpvl2 version mismatch"),
                      QApplication::tr("libvpvl2's version is incorrect (expected: %1 actual: %2).\n"
                                       "Please replace libvpvl to correct version or reinstall MMDAI.")
                      .arg(VPVL2_VERSION_STRING).arg(vpvl2::libraryVersionString()));
        return result;
    }

    // TODO: make external
    QHash<IEncoding::ConstantType, CString *> constants;
    try {
        constants.insert(IEncoding::kArm, new CString("腕"));
        constants.insert(IEncoding::kAsterisk, new CString("*"));
        constants.insert(IEncoding::kCenter, new CString("センター"));
        constants.insert(IEncoding::kElbow, new CString("ひじ"));
        constants.insert(IEncoding::kFinger, new CString("指"));
        constants.insert(IEncoding::kLeft, new CString("左"));
        constants.insert(IEncoding::kLeftKnee, new CString("左ひざ"));
        constants.insert(IEncoding::kRight, new CString("右"));
        constants.insert(IEncoding::kRightKnee, new CString("右ひざ"));
        constants.insert(IEncoding::kSPAExtension, new CString(".spa"));
        constants.insert(IEncoding::kSPHExtension, new CString(".sph"));
        constants.insert(IEncoding::kWrist, new CString("手首"));
        constants.insert(IEncoding::kRootBoneAsset, new CString("全ての親"));
        constants.insert(IEncoding::kScaleBoneAsset, new CString("拡大率"));
        constants.insert(IEncoding::kOpacityMorphAsset, new CString("不透明度"));
        vpvm::MainWindow w(constants);
        w.show();
        result = a.exec();
    } catch (std::exception &e) {
        QWidget fake;
        vpvm::warning(&fake,
                      QApplication::tr("Exception caught"),
                      QApplication::tr("Exception caught: %1").arg(e.what()));
    }
    vpvm::LoggerWidget::destroyInstance();
    qDeleteAll(constants);
    xmlCleanupParser();
    xmlMemoryDump();

    return result;
}
