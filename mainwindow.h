#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class CanvasWidget;

// No fancy color picker by default. It depends on another library.
#ifndef FANCY_COLOR_PICKER
#define FANCY_COLOR_PICKER 0
#endif

#if FANCY_COLOR_PICKER
namespace color_widgets { class ColorDialog; }
#endif

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
    
#if FANCY_COLOR_PICKER
    color_widgets::ColorDialog* colorPicker;
#endif
};

#endif // MAINWINDOW_H
