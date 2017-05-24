#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVTKWidget.h>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleUnicam.h>

#include <vtkEventQtSlotConnect.h>  // połączenie VTK->Qt

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkObject.h>

#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <QMessageBox>
#include <QPixmap>
#include <QDir>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <vtkXMLPolyDataReader.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkProperty.h>
#include <vtkPropCollection.h>
#include <QPainter>
#include <QSettings>
#include <QVariant>
#include <QColorDialog>

/*! Klasa MainWindow - odpowiada za połączenie widżetów z metodami oraz działanie całej aplikacji
 *
 * W obrębie klasy wykonywane są wszystkie działania związane z działaniami w głównym oknie programu.
 * Skonfigurowany jest również wygląd podpowiedzi oraz działania podstawowych opcji w menu.
 *
 *
 * Klasa posiada:
 * chronione sloty - pozwalające na otrzymywanie inforamcji
 * signały - do przesyłania danych do metod i innych klas
 * zmienne oraz wskażniki prywatne - potrzebne do poprawnego działania metod klasy oraz wywoływania
 * chronione funkcje - odpowiadające za akcje wewnątrz klasy
 * konstruktor jawny

    Michał Kluska & Maciek Kucharski, czerwiec 2016

*/

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    QString fileExt;                            ///< pole klasy zawierające rozszerzenie otwartego pliku
    QString fileName;                           ///< pole klasy zawierajace nazwę otwawrtego pliku
    QString filePath;                           ///< pole klasy zawierające ścieżkę otwartego pliku
    QFileInfo fileInfo;                         ///< pole klasy zawierające informacje o pliku
    bool OnOffobject = true;                    ///< pole kalsy zawierające informacje o widzialności obiketu
    void viewObject();                          ///< metoda do inicjalizacji strumienia VTK
    void setConnections();                      ///< metoda do inicjalizacji połoczeń SIGNAL-SLOT
    void phongSliderReset();                    ///< metoda do resetowanie ustawień Slider'ów do stanu poczatkowego
    template <class T> void readObject();       ///< szablon klasy odowiadajacy za wczytanie wybranego pliku
    template <class T> void sliceObject();      ///< szablon klasy odpowiadajacy za wizualizację krojenia
    template <class T> void sliceObject(float, float, float, float, float); ///< szablon klasy odpowiadający za automatyczne krojenie obiektu 3D
    void showOnStatusBar();                     ///< metoda odpowiadająca za wyświetlanie wartości nastaw na pasku status bar
    void showMessageOnStatusBar();              ///< metoda odpowiadająca za wyświetlanie inforacji na pasku status bar
    void hideCutter();                          ///< metoda odpowiadająca za ukrywanie wizualizacji cięcia

signals:

    void setStatusText(QString);                ///< metoda do wystanie tekstu na StatusBar
    void wellOpen();                            ///< flaga, że plik został wgrany poprawnie

protected slots:

    void updateCoords();                        ///< metoda do aktualizacji współrzędnych kamery
    void openFile();                            ///< metoda do otwierania pliku
    void takeScreen();                          ///< metoda wykonywanie screenshot-a
    void takeScreenWidgetArea();                ///< metoda screenshot-a samego widgetu
    void takeAutoScreenWidgetArea(QString);     ///< metoda do wykonywania autmatycznych screenshot-ów
    void saveScreen();                          ///< metoda do zapisywania screen-ów
    void aboutApp();                            ///< metoda do wyświetlania inforamcji o aplikacji
    void objectToVisualization();               ///< metoda do wywoływania szablonu wizualizacji cięcia
    void objectToSlice();                       ///< metoda do wywoływania szablonu klasy do cięcia automatycznego obiektu
    void changeCheckBox();                      ///< metoda do aktywacji cześci okna odpowiadającego za ustawianie parametrów krojenia
    void setLabel(int);                         ///< metoda do ustawiania wartości na labelach
    void hideActor();                           ///< metoda do ukrywania aktora
    void setDiff(int);                          ///< metoda do ustawiania wartości rozproszenia renderu modelu
    void setSpec(int);                          ///< metoda do ustawiania wartości rozbłysku renderu modelu
    void setSpecPow(int);                       ///< metoda do ustawiania wartości mocy rozbłysku renderu modelu
    void setColor();                            ///< metoda do ustawienia koloru modelu
    void phongEnabled();                        ///< metoda ustawiająca dostęp do kontrolek renderu modelu
    void saveRenderParam();                     ///< metoda zapisująca nastawy parametrów renderu
    void loadRenderParam();                     ///< metoda wgrywająca nastawy parametrów renderu
    void setPointsObjectView();                 ///< metoda do zmiany sposobu reprezentacji obiektu na punktowy
    void setSurfaceObjectView();                ///< metoda do zmiany sposobu reprezentacji obiektu na powierzchniowy
    void setWireframeObjectView();              ///< metoda do zmiany sposobu reprezentacji obiektu na szkieletowy

private:

    Ui::MainWindow *ui;                     ///< wyklikany interfejs użytkownika
    vtkConeSource* source;                  ///< wskaźnik na obiekty VTK
    vtkPolyDataMapper* mapper;              ///< wskaźnik na mapper-a
    vtkActor* actor;                        ///< wskaźnik na aktora
    vtkRenderer* ren;                       ///< wskaźnik na render-era
    vtkRenderWindow* renWin;                ///< wskaźnik na okno
    vtkProperty* propAktora;                ///< wskaźnik na ustawienia aktora
    QVTKInteractor* iren;                   ///< wskaźnik na interactor-a
    QPixmap image;                              ///<zmienna zapamiętująca obraz

};

#endif // MAINWINDOW_H
