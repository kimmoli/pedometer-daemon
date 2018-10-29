/*
 * (C) 2018 Kimmo Lindholm <kimmo.lindholm@eke.fi>
 *
 * Step counter daemon
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include "stepcounter.h"
#include "adaptor.h"
#include <QtCore/QCoreApplication>

static void daemonize();
static void signalHandler(int sig);

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    daemonize();

    setlinebuf(stdout);
    setlinebuf(stderr);

    printf("Starting daemon version %s\n", APPVERSION);

    StepCounter stepcounter;
    new StpcntrdAdaptor(&stepcounter);

    if (!stepcounter.registerDBus())
    {
        printf("Failed to reqister to dbus\n");
        return EXIT_FAILURE;
    }

    return app.exec();
}

static void daemonize()
{
    /* register signals to monitor / ignore */
    signal(SIGCHLD,SIG_IGN); /* ignore child */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP,SIG_IGN); /* ignore hangup signal */
    signal(SIGTERM,signalHandler); /* catch kill signal */
}


static void signalHandler(int sig) /* signal handler function */
{
    switch(sig)
    {
        case SIGTERM:
            printf("Received signal SIGTERM\n");
            QCoreApplication::quit();
            break;

        default:
            break;
    }
}
