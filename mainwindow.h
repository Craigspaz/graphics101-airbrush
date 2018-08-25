#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class CanvasWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Overload the close event to ask the user about saving.
    void closeEvent( QCloseEvent* event ) override;

private slots:
    bool open();
    bool saveAs();
    void chooseColor();

    void updateBrushPreview( const QImage& brush );

private:
    bool saveChangesIfNeeded();

private:
    Ui::MainWindow* ui;
    CanvasWidget* canvas;
};

#endif // MAINWINDOW_H
