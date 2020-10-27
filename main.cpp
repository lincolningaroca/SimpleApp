#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setStyle("Fusion");
  a.setApplicationName("SWMicrosystem's");
  Widget w;
  //  ListaUserDialog w;
  w.setWindowTitle(qApp->applicationName());
  w.show();
  return a.exec();
}
