#include "main_window.h"

#include <QtWidgets/QApplication>


#include "core/core.h"
#include "common/public/log.h"
#include "common/public/load_lib.h"

int main(int argc, char* argv[])
{
    smart_glog::initLogger("toolbox", R"(C:\Users\wangbaoyu\Desktop\ToolBox)");

    LOG(INFO) << "toolbox start...";

    CoreMgr.init();

    QApplication a(argc,argv);
    TOOLBOX::MainWindow w;
    w.setWindowTitle("ByToolBox");
    w.show();
    return a.exec();
}
