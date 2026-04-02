#include "Base64ConverUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Base64ConverUI window;
    app.setApplicationName("Base64ConverUI");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Sordayne");
    window.show();
    return app.exec();
}
