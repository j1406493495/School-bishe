#include <QtGui/QApplication>
#include "player.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Player w;
    w.setGeometry(0, 0, 450, 280);
    w.setFixedSize(450, 280);
    w.show();

    return app.exec();
}
