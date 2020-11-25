#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvaswidget.h"

#if FANCY_COLOR_PICKER
#include "QtColorWidgets/color_dialog.hpp"
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QPainter>
#include <QCloseEvent>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = ui->paintCanvas;
    // canvas->resize( 300, 300 );
    
#if FANCY_COLOR_PICKER
    colorPicker = new color_widgets::ColorDialog(this);
    colorPicker->setColorSpace( color_widgets::ColorWheel::ColorLCH );
    colorPicker->setColor( canvas->getColor() );
    colorPicker->setAlphaEnabled( true );
    colorPicker->setButtonMode( color_widgets::ColorDialog::Close );
    connect( colorPicker, &color_widgets::ColorDialog::colorChanged, canvas, &CanvasWidget::setColor );
#endif
    
    // Update the preview window.
    connect( canvas, &CanvasWidget::brushChanged, this, &MainWindow::updateBrushPreview );
    updateBrushPreview( canvas->getBrushImage() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if( saveChangesIfNeeded() ) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::open()
{
    if( !saveChangesIfNeeded() ) return false;

    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open an Existing Image...",
        QDir::currentPath(),
        "Images (*.png *.jpg *.jpeg *.gif *.bmp *.ppm *.svg)"
        );

    if( filePath.isEmpty() ) return false;

    return canvas->loadFromPath( filePath );
}

bool MainWindow::saveAs()
{
    QString defaultPath = QDir::currentPath() + "/untitled.png";

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Save As...",
        defaultPath,
        "Images (*.png *.jpg *.jpeg *.gif *.bmp *.ppm *.svg)"
        );

    if( savePath.isEmpty() ) { return false; }

    return canvas->saveToPath( savePath );
}

void MainWindow::chooseColor()
{
#if FANCY_COLOR_PICKER
    colorPicker->show();
#else
    QColor newColor = QColorDialog::getColor( canvas->getColor(), 0, "Choose Color...", QColorDialog::ShowAlphaChannel );
    if( !newColor.isValid() ) return;

    canvas->setColor( newColor );
#endif
}

bool MainWindow::saveChangesIfNeeded()
{
    if( !canvas->isModified() ) return true;

    QMessageBox::StandardButton result;
    result = QMessageBox::warning( this,
                                   "Airbrush", "Do you want to save your changes?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
                                   );

    if( result == QMessageBox::Save ) {
        return saveAs();
    } else if( result == QMessageBox::Cancel ) {
        return false;
    } else { // Discard Changes
        return true;
    }
}

void MainWindow::updateBrushPreview( const QImage& brush )
{
    ui->airbrushPreview->setPixmap( QPixmap::fromImage( brush ) );
    ui->airbrushPreview->update();
}
