#include "mainwindow.h"
#include "server.h"

#include <QApplication>
#include <QIcon>
#include <pthread.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Start the blockchain server in a separate thread
    pthread_t blockchain_thread;
    pthread_create(&blockchain_thread, NULL, (void* (*) (void*) ) start_blockchain_server, NULL);

    // Run the Qt GUI
    MainWindow window;
    window.show();

    int _ = app.exec();

    // Wait for the blockchain thread to finish
    pthread_join(blockchain_thread, NULL);

    return _;
}
