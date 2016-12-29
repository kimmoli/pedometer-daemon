/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@eke.fi>
 *
 * Pedometer daemon
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "stepcounter.h"
#include "adaptor.h"
#include <QtCore/QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    setlinebuf(stdout);
    setlinebuf(stderr);

    printf("pedometerd: Starting daemon version %s\n", APPVERSION);

    StepCounter stepcounter;
    new PedometerdAdaptor(&stepcounter);

    if (!stepcounter.registerDBus())
    {
        printf("pedometerd: failed to reqister to dbus\n");
        return EXIT_FAILURE;
    }

    return app.exec();
}
