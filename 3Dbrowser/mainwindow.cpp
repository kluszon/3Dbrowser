#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NORMALIZACJA /255.0     //! \def normalizacja kolorów
#define INIT_DIFF_POW 100       //! \def wartość domyślna dla rozproszenia
#define INIT_SPEC 0             //! \def wartość domyślna dla rozbłysku

/*!
 *  \brief MainWindow::MainWindow - konstruktor klasy
 *  \param parent - wskaznik na obiekt nadrzędny
 *  \param ui - wskażnik na interfejs aplikacji
 *
 */


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(trUtf8("GUT"));

    MainWindow::setWindowFlags(Qt::WindowStaysOnTopHint);

    //Ustawienia ToolTip-ów

    ui->centralWidget->setToolTip("Aby odblokowac okno programu <font color='red'><b>Wczytaj obiekt</b></font>.");
    ui->dockWidget->setToolTip("Aby odblokowac okno programu <font color='red'><b>Wczytaj obiekt</b></font>.");
    ui->dockWidget2->setToolTip("Aby odblokowac okno programu <font color='red'><b>Wczytaj obiekt</b></font>.");
    ui->actionHideObject->setToolTip("Aby odblokowac okno programu <font color='red'><b>Wczytaj obiekt</b></font>.");

    ui->actionExit->setIconVisibleInMenu(true);
    ui->actionAboutApp->setIconVisibleInMenu(true);
    ui->actionAboutQt->setIconVisibleInMenu(true);
    ui->actionOpenObject->setIconVisibleInMenu(true);
    ui->actionSaveWidget->setIconVisibleInMenu(true);
    ui->actionSaveWindow->setIconVisibleInMenu(true);

////////////////////////////////////////////////////////////////
// Obsługa VTK

    // utworzenie renderera - wirtualnej sceny
    ren = vtkRenderer::New();

    // podłączenie renderera do wigetu:
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);    // dodanie sceny do widgetu!
    renWin = ui->qvtkWidget->GetRenderWindow();             // pobranie wskaźnika do widgetu

    // utowrzenie strumienia wizualziacji:
    //  * inicjalizacja obiektów VTK
    //  * podłączenie przetwarzania danych do wizualziacji (do okna wizualizacji)

    //viewObject(); // przykładowy strumień VTK

////////////////////////////////////////////////////////////////
// Zestawienie połączeń SIGNAL->SLOT interfejsu Qt
    setConnections();
}


/*!
 * \brief MainWindow::setConnections() - zestawianie połączeń sygnałów i slotów
 *
 */

void MainWindow::setConnections()
{

    connect(ui->actionSaveWindow, SIGNAL(triggered()),          // połączenie przycisku z menu z metodą wykonującą zrzut ekranu
            this, SLOT(takeScreen()));

    connect(ui->actionSaveWidget, SIGNAL(triggered()),          // połączenie przycisku z menu z metodą wykonującą zrzut części widgetu
            this, SLOT(takeScreenWidgetArea()));

    connect(ui->actionExit, SIGNAL(triggered()),                // połączenie przycisku z menu z metodą zamykajacą aplikację
            this, SLOT(close()));

    connect(ui->actionAboutApp, SIGNAL(triggered()),            // połączenie przycisku z menu z metodą wyświetlającą informację o aplikacji
            this, SLOT(aboutApp()));

    connect(ui->actionOpenObject, SIGNAL(triggered()),          // połączenie przycisku z menu z metodą wykonującą wczytanie obiektu
            this, SLOT(openFile()));

    connect(ui->actionAboutQt, SIGNAL(triggered()),             // połączenie przycisku z menu z metodą wyświetlajacą informacje o Qt
            qApp, SLOT(aboutQt()));

    connect(this, SIGNAL(setStatusText(QString)),               // połączenie sygnału ustawienia statusu z metodą wyświetlajacą informacje
            ui->statusBar, SLOT(showMessage(QString)) );

    connect(ui->qvtkWidget, SIGNAL(mouseEvent(QMouseEvent*)),   //sygnał z widgetu, nie z VTK połaczony z metodą aktualizującą współrzędne
            this, SLOT(updateCoords()));

    connect(ui->pushButtonView, SIGNAL(clicked()),                  // łączy przycisk pushButtonView z metodą do wizualizacji krojenia obiektu
            this, SLOT(objectToVisualization()));

    connect(ui->checkBoxOnSlicer, SIGNAL(released()),               // łączy checkBoxOnSlider z metodą aktywującą częśc widgetu do sterowania krojeniem
            this, SLOT(changeCheckBox()));

    connect(ui->horizontalSliderX, SIGNAL(valueChanged(int)),       // łączy slajder z labelem
            this , SLOT(setLabel(int)));

    connect(ui->horizontalSliderY, SIGNAL(valueChanged(int)),       // łączy slajder z labelem
            this, SLOT(setLabel(int)));

    connect(ui->horizontalSliderZ, SIGNAL(valueChanged(int)),       // łączy slajder z labelem
            this, SLOT(setLabel(int)));

    connect(ui->actionHideObject, SIGNAL(triggered()),              // łącze przycisk actionHideObject z metodą ukrywającą
            this, SLOT(hideActor()));

    connect(ui->pushButtonSlice, SIGNAL(clicked()),                 // łącze przycisk pushButtonSlice z metodą do krojenia obiektu
            this, SLOT(objectToSlice()));

    connect(ui->sliderDiff, SIGNAL(sliderMoved(int)),               // łacze sliderDiff z obiektem przestrzennym
            this, SLOT(setDiff(int)));

    connect(ui->sliderSpecSlider, SIGNAL(sliderMoved(int)),         // łacze sliderSpecSlider z obiektem przestrzennym
            this, SLOT(setSpec(int)));

    connect(ui->sliderSpecPow, SIGNAL(sliderMoved(int)),            // łacze sliderSpecPow z obiektem przestrzennym
            this, SLOT(setSpecPow(int)));

    connect(ui->pushButtonColor, SIGNAL(clicked()),                 // ustawianie koloru w zewnętrznym oknie
            this, SLOT(setColor()));

    connect(this, SIGNAL(wellOpen()),                               // aktywacja kontrolek po wgraniu modelu
            this, SLOT( phongEnabled() ));

    connect(ui->pushButtonSaveProp, SIGNAL(clicked()),              // zapisanie parametrów renderu w pliku z ustaweiniami
            this, SLOT(saveRenderParam()));

    connect(ui->pushButtonLoadProp, SIGNAL(clicked()),              // wgranie parametrów z pliku z ustawieniami
            this, SLOT(loadRenderParam()));

    connect(ui->sliderDiff, SIGNAL(valueChanged(int)),              // łaczenie slajderów z odpowiednimi labelami
            ui->labelDiffIndicator, SLOT(setNum(int)));

    connect(ui->sliderSpecPow, SIGNAL(valueChanged(int)),
            ui->labelSpecPowIndicator, SLOT(setNum(int)));

    connect(ui->sliderSpecSlider, SIGNAL(valueChanged(int)),
            ui->labelSpecIndicator, SLOT(setNum(int)));

    connect(ui->actionPointsView, SIGNAL(triggered()),              // połączenie przycisków w menii z metodami zmieniającymi reprezentacje obiektu
            this, SLOT(setPointsObjectView()));

    connect(ui->actionWireframeView, SIGNAL(triggered()),
            this, SLOT(setWireframeObjectView()));

    connect(ui->actionSurfaceView, SIGNAL(triggered()),
            this, SLOT(setSurfaceObjectView()));

    // Zestawienie połączeń VTK->Qt:
    vtkSmartPointer<vtkEventQtSlotConnect> connections =
            vtkSmartPointer<vtkEventQtSlotConnect>::New();      // obiekt-pomost

    // przechwycenie zdarzeń vtk:
    connections->Connect(ui->qvtkWidget->GetInteractor(),       // pobranie obiektu (interaktora) z VTK
                            vtkCommand::LeftButtonPressEvent,   // wybranie sygnału
                            this,                               // ustalenie odbiorcy
                            SLOT(updateCoords()));              // ustalenie slotu odbiorcy, do którego trafi sygnał z VTK
                                                                // prototyp slotu odbiorcy:
                                                                //      SLOT( slotOdbiorcy( vtkObject*, unsigned long, void*, void*))

    connections->PrintSelf(cout, vtkIndent());  // tak można podglądać dowolne obiekty VTK!
}

/*!
 * \brief MainWindow::phongSliderReset() - metoda resetująca ustawienia suwaktków interfejsu renderu modelu
 *
 */

void MainWindow::phongSliderReset()
{
   ui->sliderDiff->setValue(INIT_DIFF_POW);
   ui->labelDiffIndicator->setText("100");
   ui->sliderSpecPow->setValue(INIT_DIFF_POW);
   ui->labelSpecPowIndicator->setText("100");
   ui->sliderSpecSlider->setValue(INIT_SPEC);
   ui->labelSpecIndicator->setText("0");

}

/*!
 * \brief MainWindow::phongEnabled() - metoda aktywująca interfejs renderu modelu w momencie poprawnego
 * wgrania modelu do przeglądarki
 *
 */

void MainWindow::phongEnabled()
{
    qDebug() << "aktywuje okienka do manipulacji Phongiem: ";
    ui->dockWidgetContentsPhong->setEnabled(true);
    ui->dockWidgetContentsSlice->setEnabled(true);
    ui->actionHideObject->setEnabled(true);
    ui->actionPointsView->setEnabled(true);
    ui->actionSurfaceView->setEnabled(true);
    ui->actionWireframeView->setEnabled(true);
    phongSliderReset();

    //Ustawienia ToolTip-ów

    ui->centralWidget->setToolTip("Kliknij i obroc mysza dowolnie wyswietlany obiekt.");
    ui->pushButtonView->setToolTip("Wybierz odpowiednie nastawy podgladnij przykladowy wycinek przed wykonaniem krojenie.");
    ui->pushButtonSlice->setToolTip("Dokonaj automatycznego krojenia obiektu z wybranymi nastawami do pliku.<font color='green'><b>(Zapis do katalogu z wczytanym obiektem 3D.)</b></font>");
    ui->dockWidget2->setToolTip("Wybierz odpowiednie nastawy aby zmienic papametry <font color='green'><b>modelu Phonga.</b></font>");
    ui->actionHideObject->setToolTip("Wlacz lub wylacz wyswietlany obiekt.");

    ui->sliderDiff->setToolTip("Zmien wartosc <font color='green'><b>rozproszenia swiatla.</b></font>");
    ui->sliderSpecPow->setToolTip("<font color='green'>Zmien <font color='green'><b>moc rozblysku swiatla.</b></font>");
    ui->sliderSpecSlider->setToolTip("<font color='green'>Zmien <font color='green'><b>wartosc rozblysku.</b></font>");

    ui->pushButtonColor->setToolTip("Wywolaj palete kolorow i wybierz kolorowanie obiektu.");
    ui->pushButtonSaveProp->setToolTip("<font color='green'><b>Zapisz</b></font> ustawienia modelu Phonga do pliku.");
    ui->pushButtonLoadProp->setToolTip("<font color='green'><b>Wczytaj</b></font> ustawienia modelu Phonga do pliku.");

    ui->groupBox1->setToolTip("Ustaw wartosc <font color='green'><b>grubosci ostrza</b></font>, aby otrzymac bardziej wyraziste wycinki obiektu.");
    ui->groupBox2->setToolTip("Ustaw wartosc <font color='green'><b>grubosci plastra</b></font>, aby okreslic szerokosc krojenia obiektow.");

    ui->horizontalSliderX->setToolTip("Podaj wspolrzedna <font color='green'><b>X</b></font> aby utworzyc plaszczyzne tnaca ( (X,Y,Z) -> XZ (1,0,0);XY (0,0,1),YZ (0,1,0)");
    ui->horizontalSliderY->setToolTip("Podaj wspolrzedna <font color='green'><b>Y</b></font> aby utworzyc plaszczyzne tnaca ( (X,Y,Z) -> XZ (1,0,0);XY (0,0,1),YZ (0,1,0)");
    ui->horizontalSliderZ->setToolTip("Podaj wspolrzedna <font color='green'><b>Z</b></font> aby utworzyc plaszczyzne tnaca ( (X,Y,Z) -> XZ (1,0,0);XY (0,0,1),YZ (0,1,0)");
}

// ______________________ZMIANY PARAMETRÓW RENDEROWANIA OBIEKTU___________________________

/*!
 * \brief MainWindow::setDiff(int value) - metoda odpowiadająca za ustawienie rozproszenia w renderu modelu
 * \param value - wartość rozproszenia
 */

void MainWindow::setDiff(int value)
{
    actor->GetProperty()->SetDiffuse( (float(value)/100) );
    ui->qvtkWidget->repaint();      // wymuszenie odświerzenia okna VTK
}

/*!
 * \brief MainWindow::setSpec(int value) - metoda odpowiadająca za ustawienie rozbłysku w renderu modelu
 * \param value - wartość rozbłysku
 *
 */

void MainWindow::setSpec(int value)
{
    actor->GetProperty()->SetSpecular( (float(value)/100) );
    ui->qvtkWidget->repaint();
}

/*!
 * \brief MainWindow::setSpecPow(int value) - metoda odpowiadająca za ustawienie mocy rozbłysku renderu modelu
 * \param value - wartość mocy rozbłysku
 */

void MainWindow::setSpecPow(int value)
{
    actor->GetProperty()->SetSpecularPower( (float(value)/100) );
    ui->qvtkWidget->repaint();
}

/*!
 * \brief MainWindow::setColor() - metoda odpowiadająca za ustawienie koloru renderu modelu
 *
 */

void MainWindow::setColor()
{

    QColor color = QColorDialog::getColor();                   // wywołanie okienka z paletą kolorów

    double r=(color.red())NORMALIZACJA;                        // uzyskanie koloru czerwonego znormalizowanego
    double g=(color.green())NORMALIZACJA;
    double b=(color.blue())NORMALIZACJA;

    if (color.isValid())                                     // sprawdzenie poprawnosci
    {
        qDebug()<< "usyzkane skladowe RGB: "<< r << "\t"<< g<< "\t" << b;
        actor->GetProperty()->SetColor(r,g,b);
        ui->qvtkWidget->repaint();
    }

}

//_________________________OBSŁUGA PARAMETRÓW RENDERU___________________________________

/*!
 * \brief MainWindow::saveRenderParam() - metoda odpowiadająca za zapisywanie nastaw renderu modelu
 *
 */

void MainWindow::saveRenderParam()
{
    QSettings renderParams("QtWIZUproj", "GUT_browser");
    renderParams.setValue("rozproszenie", this->actor->GetProperty()->GetDiffuse());
    renderParams.setValue("rozblysk", this->actor->GetProperty()->GetSpecular());
    renderParams.setValue("moc_rozblysku", this->actor->GetProperty()->GetSpecularPower());

    double rComp,gComp,bComp;
    actor->GetProperty()->GetColor(rComp,gComp,bComp);
    //qDebug() << "r:" << rComp << "g;" << gComp << "b:" << bComp;
    renderParams.setValue("R", rComp );
    renderParams.setValue("G", gComp );
    renderParams.setValue("B", bComp );

    qDebug() << "nastawy zapisane\n";
    qDebug() << " w pliku: " << renderParams.fileName();
}

/*!
 * \brief MainWindow::loadRenderParam() - metoda odpowiadająca za wczytywanie nastaw renderu modelu
 *
 */

void MainWindow::loadRenderParam()
{
    QSettings renderParams("QtWIZUproj", "GUT_browser");
    QVariant temp_bufor = renderParams.value("rozproszenie");
    actor->GetProperty()->SetDiffuse(temp_bufor.toDouble());
    temp_bufor=(temp_bufor.toDouble()*100);
    ui->sliderDiff->setValue( temp_bufor.toInt() );
    temp_bufor=(temp_bufor.toInt());
    ui->labelDiffIndicator->setText(temp_bufor.toString());

    temp_bufor=renderParams.value("rozblysk");
    actor->GetProperty()->SetSpecular( temp_bufor.toDouble() );
    temp_bufor=(temp_bufor.toDouble()*100);
    ui->sliderSpecSlider->setValue( temp_bufor.toInt() );
    temp_bufor=(temp_bufor.toInt());
    ui->labelSpecIndicator->setText(temp_bufor.toString());

    temp_bufor=renderParams.value("moc_rozblysku") ;
    actor->GetProperty()->SetSpecularPower( temp_bufor.toDouble() );
    temp_bufor=(temp_bufor.toDouble()*100);
    ui->sliderSpecPow->setValue( temp_bufor.toInt() );
    temp_bufor=(temp_bufor.toInt());
    ui->labelSpecPowIndicator->setText(temp_bufor.toString());

    //pobranie kolorów
    double rgb[3];
    temp_bufor=renderParams.value("R");
    *rgb=temp_bufor.toDouble();
    temp_bufor=renderParams.value("G");
    *(rgb+1)=temp_bufor.toDouble();
    temp_bufor=renderParams.value("B");
    *(rgb+2)=temp_bufor.toDouble();
    actor->GetProperty()->SetColor(rgb);

    qDebug() << "nastawy wgrane!!!!!!!!!!!";
    ui->qvtkWidget->repaint();
}

/*!
 * \brief MainWindow::showMessageOnStatusBar() - metoda odpowiadająca za wyświetlenie informacji na status bar
 * o zakończeniu krojenia obiektu
 *
 */

void MainWindow::showMessageOnStatusBar()
{

  ui->statusBar->showMessage(QString::fromUtf8("Działanie zakończone! :)"));

}

/*!
 * \brief MainWindow::showOnStatusBar - metoda odpowiadająca za wyświetlanie wartości parametrów ustawionych do krojenia
 *
 */

void MainWindow::showOnStatusBar()
{
    double x = (ui->horizontalSliderX->value())/100.0;
    double y = (ui->horizontalSliderY->value())/100.0;
    double z = (ui->horizontalSliderZ->value())/100.0;
    double tk = (ui->spinBoxSliceLineWidth->value());
    double tp = (ui->spinBoxSliceWidth->value());

    // wystawienie wartości INT na StatusBar
    ui->statusBar->showMessage(QString::fromUtf8("Wartość x: %1; y: %2; z: %3; tk: %4; tp: %5 <- trwa krojenie....").arg(x).arg(y).arg(z).arg(tk).arg(tp));
}

/*!
 * \brief MainWindow::updateCoords() - metoda odpowiadająca za pobranie współrzędnych kamery zmienionych poprzez przekręcanie
 * obiektu za pomocą myszki w oknie programu.
 *
 */


void MainWindow::updateCoords()
{
    double camPosition[3];
    ren->GetActiveCamera()->GetPosition(camPosition);   // pobranie wsp. kamery

    QString str = QString("x=%1 : y=%2 : z=%3")
                  .arg(camPosition[0]).arg( camPosition[1]).arg(camPosition[2]);

    //    qDebug() << str;      // wyslanie str na konsole

}

/*!
 * \brief MainWindow::viewObject() - metoda analizująca informacje o pliku, wyłuskująca informacje o roszerzeniu,
 * lokalizacji, nazwie oraz wywołująca odpowiedni szablon klasy czytajacej obiekt.
 *
 */

void MainWindow::viewObject()
{

    filePath = QFileInfo(fileInfo).absoluteFilePath();
    QString fileFolder = QFileInfo(fileInfo).absolutePath();
    fileName = QFileInfo(fileInfo).fileName();
    fileExt = QFileInfo(fileInfo).suffix();

    qDebug() << "Sciezka elementu: " << filePath;
    qDebug() << "sciezka folderu: " << fileFolder;
    qDebug() << "Nazwa pliku: " << fileName;
    qDebug() << "Rozszerzenie pliku: " << fileExt;

    QDir::setCurrent(fileFolder);
    qDebug() << QDir::currentPath();

    if (fileExt == "stl")
    {

        readObject<vtkSTLReader>();
        emit wellOpen();

    }
    else if(fileExt == "vtp")
    {

        readObject<vtkXMLPolyDataReader>();
        emit wellOpen();

    }
    else
        qDebug() << "Rozszerzenie nie obslugiwane!";

}

/*!
 * \brief MainWindow::readObject() - szablon funkcji do otwarcia obiektów .stl i .obj. Dzięki wykorzystaniu szablonu
 * można było rozwiazać problem tworzenia obiektów o różnym typie. Wewnątrz funkcji tworzone są wszystkie elementy niezbędne
 * do pokazania wczytanego obiektu w oknie aplikacji.
 *
 */

template <class T>
void MainWindow::readObject()
{

    vtkSmartPointer<T> reader = vtkSmartPointer<T>::New();
    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    // Mapper od mappera zaczyna się część wizualizująca

    mapper = vtkPolyDataMapper::New();
    mapper->ImmediateModeRenderingOn();
    mapper->SetInputConnection(reader->GetOutputPort());

    // Aktor - graficzna reprezentacja "obiektu" na wirtualnej scenie
    actor = vtkActor::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetInterpolationToPhong();

    qDebug() << "\n****parametry renderu obiektu****";
    qDebug() << "interpolacja: " << actor->GetProperty()->GetInterpolation();

    double r, g, b;
    actor->GetProperty()->GetColor(r,g,b);

    qDebug() << "kolor: "<< r <<"\t"<< g << "\t"<<b;
    qDebug() << "Diff: "<< actor->GetProperty()->GetDiffuse();
    qDebug() << "Spec: "<< actor->GetProperty()->GetSpecular();
    qDebug() << "SpecPow: "<< actor->GetProperty()->GetSpecularPower();

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    ren->RemoveAllObservers();
    ren->RemoveAllViewProps();
    ren->RemoveAllProps();
    ren->AddActor(actor);
    ren->SetBackground(.3, .6, .3);
    ren->GradientBackgroundOn();

    // Reset kamery

    ren->ResetCamera();
    ren->GetRenderWindow()->Render();
    ren->ResetCameraClippingRange();

    renWin->Render();
    renWin->Start();

}

/*!
 * \brief MainWindow::setPointsObjectView() - funkcja do zmiany sposobu reprezentacji obiektu na punktowy
 */


void MainWindow::setPointsObjectView()
{

    actor->GetProperty()->SetRepresentationToPoints();
    ui->qvtkWidget->repaint();

}

/*!
 * \brief MainWindow::setSurfaceObjectView() - funkcja do zmiany sposobu reprezentacji obiektu na płaszczyznowy
 */

void MainWindow::setSurfaceObjectView()
{

    actor->GetProperty()->SetRepresentationToSurface();
    ui->qvtkWidget->repaint();

}

/*!
 * \brief MainWindow::setWireframeObjectView() - funkcja do zmiany sposobu reprezentacji obiektu na szkieletowy
 */


void MainWindow::setWireframeObjectView()
{

    actor->GetProperty()->SetRepresentationToWireframe();
    ui->qvtkWidget->repaint();

}


/*!
 * \brief MainWindow::hideActor() - funkcja do ukrywania aktora
 */

void MainWindow::hideActor()
{
    // Hide one actor at a time

    vtkPropCollection* props = ren->GetViewProps(); //iterate through and set each visibility to 0
    props->InitTraversal();

    if (OnOffobject == true)
    {

        qDebug() << "numer Off: "<<props->GetNumberOfItems();
        props->GetNextProp()->VisibilityOff();
        OnOffobject = false;

    }else{

        qDebug() << "numer On: "<<props->GetNumberOfItems();
        props->GetNextProp()->VisibilityOn();
        OnOffobject = true;

    }

    ren->ResetCamera();
    renWin->Render();
    renWin->Start();

}

/*!
 * \brief MainWindow::setLabel(int actualValue) - funkcja do wyświetlania przekalowanej wartości na label-ach
 *
 * \param actualValue - wartość aktualnie ustawiona na slajderze
 */

void MainWindow::setLabel(int actualValue)
{
    QString labelTextX,labelTextY,labelTextZ;

    if(actualValue == ui->horizontalSliderX->value())
    {
        labelTextX = QString("Wartosc x: %1").arg(actualValue/100.0);
        ui->labelX->setText(labelTextX);
    }

    if(actualValue == ui->horizontalSliderY->value())
    {
        labelTextY = QString("Wartosc y: %1").arg(actualValue/100.0);
        ui->labelY->setText(labelTextY);
    }
    if (actualValue == ui->horizontalSliderZ->value())
    {
        labelTextZ = QString("Wartosc z: %1").arg(actualValue/100.0);
        ui->labelZ->setText(labelTextZ);
    }
}

/*!
 * \brief MainWindow::changeCheckBox() - funkcja do aktywacji groupBoxSlider części formularza
 * odpowiadającego za ustalanie parametrów i krojenie obiektu.
 */

void MainWindow::changeCheckBox()
{

    if(ui->checkBoxOnSlicer->checkState() == 0)
    {

        ui->groupBoxSlider->setEnabled((!ui->groupBoxSlider->isEnabled()));

        hideCutter();

    }else{

        ui->groupBoxSlider->setDisabled((ui->groupBoxSlider->isEnabled()));

    }

    ren->ResetCamera();
    renWin->Render();
    renWin->Start();

}

/*!
 * \brief MainWindow::hideCutter() - funkcja do ukrywania ostrza.
 */

void MainWindow::hideCutter()
{
    vtkPropCollection* props = ren->GetViewProps(); //iterate through and set each visibility to 0

    props->InitTraversal();

    for (int i = 0; i < props->GetNumberOfItems(); i++)
    {

        if (i != 0)

             props->GetNextProp()->VisibilityOff();

        else

            props->GetNextProp()->VisibilityOn();

     }
}

/*!
 * \brief MainWindow::openFile() - funkcja otwarcia pliku .obj lub .stl przy wykorzystaniu dodatkowego okna dialogowego
 */

void MainWindow::openFile()
{
    QString format1 = "stl";
    QString format2 = "vtp";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format1;

    fileInfo = QFileDialog::getOpenFileName(this, tr("Open File"), initialPath,
                                                    tr("%1 Files (*.%2);;%3 Files (*.%4)")
                                                    .arg(format1.toUpper())
                                                    .arg(format1)
                                                    .arg(format2.toUpper())
                                                    .arg(format2));

    viewObject();

}

/*!
 * \brief MainWindow::takeScreen() - funkcja wykonująca zrzut całego okna aplikcji
 */

void MainWindow::takeScreen()
{
    //image = QPixmap::grabWidget(this);                                        // sposób nr1 - tylko widget(nie widzi VTKWidget ;/ )
    //image = QPixmap::grabWindow(QApplication::desktop()->winId())             // sposób nr2 - cały pulpit
    image = QPixmap::grabWindow(QApplication::desktop()->winId(),QMainWindow::x(),QMainWindow::y(),QMainWindow::width(),QMainWindow::height());  // sposób nr.3 - tylko okno

    saveScreen();
}

/*!
 * \brief MainWindow::takeScreenWidgetArea() - funkcja wykonująca zrzut obszaru widgetu i wywołuje fukcję odpowiadającą za zapis.
 */


void MainWindow::takeScreenWidgetArea()
{

    int coordinateX = QMainWindow::x()+5;
    int coordinateY = QMainWindow::y()+77;

    image = QPixmap::grabWindow(QApplication::desktop()->winId(), coordinateX,  coordinateY,ui->qvtkWidget->width(),ui->qvtkWidget->height());

    saveScreen();
}

/*!
 * \brief MainWindow::takeScreenWidgetArea(QString screenName) - funkcja wykonująca zrzut obszaru widgetu automatycznie do lokalizacji
 * z której otwierany był obiekt typu polydate.
 *
 * \param screenName - zmienna przekazująca spreparowaną nazwę aktualnie zapisywanego przekroju
 */


void MainWindow::takeAutoScreenWidgetArea(QString screenName)
{

    int coordinateX = QMainWindow::x()+5;
    int coordinateY = QMainWindow::y()+77;

    image = QPixmap::grabWindow(QApplication::desktop()->winId(), coordinateX,  coordinateY,ui->qvtkWidget->width(),ui->qvtkWidget->height());

    QString format = ".png";
    screenName = screenName + format;

    QPainter painter( &image );

    painter.setRenderHint( QPainter::Antialiasing );

    painter.setPen( Qt::black );
    painter.setBrush( Qt::gray );

    QDir::setCurrent(filePath);
    image.save(screenName,"PNG");

}


/*!
 * \brief MainWindow::saveScreen() - funkcja zapisuje wykonany zrzut ekranu do lokalizacji wybranej przez
 * użytkownika.
 */

void MainWindow::saveScreen()
{
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(format.toUpper())
                                                    .arg(format));
    if (!fileName.isEmpty())
        image.save(fileName, format.toLatin1().constData());

}

/*!
 * \brief MainWindow::objectToVisualization() - funkcja wywołująca odpowiedni szablon funkcji do wizualiacji.
 */

void MainWindow::objectToVisualization()
{
    if (fileExt == "stl")
        sliceObject<vtkSTLReader>();
    else if(fileExt == "vtp")
        sliceObject<vtkXMLPolyDataReader>();
    else
        qDebug() << "Rozszerzenie nie obslugiwane!";

}

/*!
 * \brief MainWindow::objectToSlice() - funkcja wywołująca odpowiedni szablon funkcji do krojenia obiektu oraz odbiera parametry
 * ustawione w oknie aplikacji, przeskalowywuje je i wysyłą do funkcji tnącej.
 */

void MainWindow::objectToSlice()
{
    float coordinateX,coordinateY,coordinateZ, sliceWidth, thicknessKnife;

    coordinateX = (ui->horizontalSliderX->value())/100.0;
    coordinateY = (ui->horizontalSliderY->value())/100.0;
    coordinateZ = (ui->horizontalSliderZ->value())/100.0;
    sliceWidth = ui->spinBoxSliceWidth->value();
    thicknessKnife = ui->spinBoxSliceLineWidth->value();

    if (fileExt == "stl")
        sliceObject<vtkSTLReader>(coordinateX,coordinateY,coordinateZ,sliceWidth,thicknessKnife);
    else if(fileExt == "vtp")
        sliceObject<vtkXMLPolyDataReader>(coordinateX,coordinateY,coordinateZ,sliceWidth,thicknessKnife);
    else
        qDebug() << "Rozszerzenie nie obslugiwane!";

}

/*!
 * \brief MainWindow::sliceObject(float coordinateX,float coordinateY,float coordinateZ,float sliceWidth,float thicknessKnife) - przeciążony szablon funkcji do krojenia obiektów odbierająca parametry potrzebne do cięcia.
 * Szablon został zastosowany w celu obejścia problemu tworzenia obiektów różnego typu dla wczytywanych plików polydate.
 * Wewnąrz funkcji dokonywane jest krojenie na plastry, wystawianie kamery i zapis do pliku poprzez wykożystanie funkcji
 * zewnętrznych. O zakończeniu operacji użytkownik informowany jest komunikatem na dolnym pasku status bar.
 *
 * \param coordinateX - współrzędna X płaszczyzny tnącej odbierana ze slajdera
 * \param coordinateY - współrzędna Y płaszczyzny tnącej odbierana ze slajdera
 * \param coordinateZ - współrzędna Z płaszczyzny tnącej odbierana ze slajdera
 * \param sliceWidth - grubość wycinanego plastra odbierana ze spinbox-a
 * \param thicknessKnife - grubość noża tnącego odbierana ze spinbox-a
 *
 */

template <class T>
void MainWindow::sliceObject(float coordinateX,float coordinateY,float coordinateZ,float sliceWidth,float thicknessKnife)
{

    hideCutter();

    QString screenName;

    int screenNumber = 0;

    showOnStatusBar();

    // Parse command line arguments

    vtkSmartPointer<T> obj = vtkSmartPointer<T>::New();
    obj->SetFileName(fileName.toStdString().c_str());
    obj->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper1->SetInputConnection(obj->GetOutputPort());
    mapper2->SetInputConnection(obj->GetOutputPort());
    qDebug()<<"Get Length: "<<mapper1->GetLength();

    double maxObjectLength = mapper1->GetLength();

    // Create plane
    vtkSmartPointer<vtkPlane> plane1 = vtkSmartPointer<vtkPlane>::New();
    vtkSmartPointer<vtkPlane> plane2 = vtkSmartPointer<vtkPlane>::New();

    // Create cutter
    vtkSmartPointer<vtkCutter> cutter1 = vtkSmartPointer<vtkCutter>::New();
    vtkSmartPointer<vtkCutter> cutter2 = vtkSmartPointer<vtkCutter>::New();

    //Create mapper
    vtkSmartPointer<vtkPolyDataMapper> cutterMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> cutterMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();

    // Create plane actor
    vtkSmartPointer<vtkActor> planeActor1 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> planeActor2 = vtkSmartPointer<vtkActor>::New();

    // Create camera
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();

    // Create pro
    vtkPropCollection* props = ren->GetViewProps();
    props->InitTraversal();
    props->GetNextProp()->VisibilityOff();

    int intScaledSliceWidth = (int)(sliceWidth*100);

    for(int i=0; i<((maxObjectLength/2)*100); i=i+intScaledSliceWidth)
    {
        screenName = QString("screen %1").arg(i/intScaledSliceWidth);

        plane1->SetOrigin(coordinateX+(i-intScaledSliceWidth),coordinateY+(i-intScaledSliceWidth),coordinateZ+(i-intScaledSliceWidth));
        plane1->SetNormal(coordinateX,coordinateY,coordinateZ);

        plane2->SetOrigin(coordinateX+(float)i/100,coordinateY+(float)i/100,coordinateZ+(float)i/100);
        plane2->SetNormal(coordinateX,coordinateY,coordinateZ);

        //first contour
        cutter1->SetCutFunction(plane1);
        cutter1->SetInputConnection(obj->GetOutputPort());
        cutter1->Update();

        //second contour
        cutter2->SetCutFunction(plane2);
        cutter2->SetInputConnection(obj->GetOutputPort());
        cutter2->Update();

        cutterMapper1->SetInputConnection(cutter1->GetOutputPort());
        cutterMapper1->Update();

        cutterMapper2->SetInputConnection(cutter2->GetOutputPort());
        cutterMapper2->Update();

        planeActor1->GetProperty()->SetColor(1.0, 1, 0);
        planeActor1->GetProperty()->SetLineWidth(thicknessKnife);
        planeActor1->SetMapper(cutterMapper1);

        planeActor2->GetProperty()->SetColor(1.0, 1, 0);
        planeActor2->GetProperty()->SetLineWidth(thicknessKnife);
        planeActor2->SetMapper(cutterMapper2);

        ren->AddActor(planeActor1); //display the rectangle resulting from the cut
        ren->AddActor(planeActor2);

        camera->SetPosition((coordinateX*100)+30, (coordinateY*100)+30, (coordinateZ*100)+30);
        camera->SetFocalPoint(0, 0, 0);

        ren->SetActiveCamera(camera);

        renWin->Render();
        renWin->Start();

        takeAutoScreenWidgetArea(screenName);

        screenNumber = i/intScaledSliceWidth;
    }

    int screenNumberTemp = screenNumber;

    for(int i=0; i<((maxObjectLength/2)*100); i=i+intScaledSliceWidth)
    {
        screenNumber = screenNumberTemp + i/intScaledSliceWidth;
        screenName = QString("screen %1").arg(screenNumber);

        plane1->SetOrigin(coordinateX-(i-intScaledSliceWidth),coordinateY-(i-intScaledSliceWidth),coordinateZ-(i-intScaledSliceWidth));
        plane1->SetNormal(coordinateX,coordinateY,coordinateZ);

        plane2->SetOrigin(coordinateX-(float)i/100,coordinateY-(float)i/100,coordinateZ-(float)i/100);
        plane2->SetNormal(coordinateX,coordinateY,coordinateZ);

        //first contour
        cutter1->SetCutFunction(plane1);
        cutter1->SetInputConnection(obj->GetOutputPort());
        cutter1->Update();

        //second contour
        cutter2->SetCutFunction(plane2);
        cutter2->SetInputConnection(obj->GetOutputPort());
        cutter2->Update();

        cutterMapper1->SetInputConnection(cutter1->GetOutputPort());
        cutterMapper1->Update();

        cutterMapper2->SetInputConnection(cutter2->GetOutputPort());
        cutterMapper2->Update();

        planeActor1->GetProperty()->SetColor(1.0, 1, 0);
        planeActor1->GetProperty()->SetLineWidth(thicknessKnife);
        planeActor1->SetMapper(cutterMapper1);

        planeActor2->GetProperty()->SetColor(1.0, 1, 0);
        planeActor2->GetProperty()->SetLineWidth(thicknessKnife);
        planeActor2->SetMapper(cutterMapper2);

        ren->AddActor(planeActor1); //display the rectangle resulting from the cut
        ren->AddActor(planeActor2);

        camera->SetPosition((coordinateX*100)+(0.4*maxObjectLength), (coordinateY*100)+(0.4*maxObjectLength), (coordinateZ*100)+(0.4*maxObjectLength));
        camera->SetFocalPoint(0, 0, 0);

        ren->SetActiveCamera(camera);

        renWin->Render();
        renWin->Start();

        takeAutoScreenWidgetArea(screenName);

    }

    OnOffobject = false;
    hideActor();

    showMessageOnStatusBar();
}

/*!
 * \brief MainWindow::sliceObject() - funkcja do krojenia obiektów wykorzystywana do wizualizacji sposobu krojenia.
 * Szablon został zastosowany w celu obejścia problemu tworzenia obiektów różnego typu dla wczytywanych plików polydate.
 * Efektem działania fukcji jest dorysowywanie do obiektu dwóch "obrysów" o ustalonej grubości obrazujących górną i dolną
 * powierzchnię wycinanego plastra obiektu 3D.
 */

template <class T>
void MainWindow::sliceObject()
{
    showOnStatusBar();
    hideCutter();

    // Parse command line arguments

    vtkSmartPointer<T> obj = vtkSmartPointer<T>::New();
    obj->SetFileName(fileName.toStdString().c_str());
    obj->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper1->SetInputConnection(obj->GetOutputPort());
    mapper2->SetInputConnection(obj->GetOutputPort());

    qDebug()<<"Get Length: "<<mapper1->GetLength();

    // Information for tooltip
    // Create a plane to cut(xz normal=(1,0,0);XY =(0,0,1),YZ =(0,1,0)

    vtkSmartPointer<vtkPlane> plane1 = vtkSmartPointer<vtkPlane>::New();
    vtkSmartPointer<vtkPlane> plane2 = vtkSmartPointer<vtkPlane>::New();

    float coordinateX = (ui->horizontalSliderX->value())/100.0;
    float coordinateY = (ui->horizontalSliderY->value())/100.0;
    float coordinateZ = (ui->horizontalSliderZ->value())/100.0;

    plane1->SetOrigin(0, 0, 0);
    plane1->SetNormal(coordinateX, coordinateY, coordinateZ);

    plane2->SetOrigin((ui->spinBoxSliceWidth->value())*coordinateX, (ui->spinBoxSliceWidth->value())*coordinateY, (ui->spinBoxSliceWidth->value())*coordinateZ);
    plane2->SetNormal(coordinateX, coordinateY, coordinateZ);

    // Create cutter
    vtkSmartPointer<vtkCutter> cutter1 = vtkSmartPointer<vtkCutter>::New();
    vtkSmartPointer<vtkCutter> cutter2 = vtkSmartPointer<vtkCutter>::New();

    //first contour
    cutter1->SetCutFunction(plane1);
    cutter1->SetInputConnection(obj->GetOutputPort());
    cutter1->Update();

    //second contour
    cutter2->SetCutFunction(plane2);
    cutter2->SetInputConnection(obj->GetOutputPort());
    cutter2->Update();

    vtkSmartPointer<vtkPolyDataMapper> cutterMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> cutterMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();

    cutterMapper1->SetInputConnection(cutter1->GetOutputPort());
    cutterMapper1->Update();

    cutterMapper2->SetInputConnection(cutter2->GetOutputPort());
    cutterMapper2->Update();

    // Create plane actor
    vtkSmartPointer<vtkActor> planeActor1 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> planeActor2 = vtkSmartPointer<vtkActor>::New();

    planeActor1->GetProperty()->SetColor(1.0, 1, 0);
    planeActor1->GetProperty()->SetLineWidth(ui->spinBoxSliceLineWidth->value());
    planeActor1->SetMapper(cutterMapper1);

    planeActor2->GetProperty()->SetColor(1.0, 1, 0);
    planeActor2->GetProperty()->SetLineWidth(ui->spinBoxSliceLineWidth->value());
    planeActor2->SetMapper(cutterMapper2);

    ren->AddActor(planeActor1); //display the rectangle resulting from the cut
    ren->AddActor(planeActor2);

    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition((ui->horizontalSliderX->value())+(0.4*(mapper1->GetLength())), (ui->horizontalSliderY->value())+(0.4*(mapper1->GetLength())), (ui->horizontalSliderZ->value())+(0.4*(mapper1->GetLength())));
    camera->SetFocalPoint(0, 0, 0);

    ren->SetActiveCamera(camera);

    renWin->Render();
    renWin->Start();

    showMessageOnStatusBar();

}

/*!
 * \brief MainWindow::aboutApp() - funkcja wyświetlająca okno "O aplikacji"
 */

void MainWindow::aboutApp()
{
    QMessageBox mssgbox;
    QString info;
    info = QString("<b>Nasza wspaniala przegladrka :D</b><br /><br />") +
           QString("<p><img src=\":/Images/Images/logo_ver2.png\" alt=\"logo\"></p>")+
           QString("Program <em><b>\"GUT\"</b></em>   jest efektem pracy przy projekcie: \"Przegladarka obiektow siatkowych 3D z widgetem do sterowania wspolczynnikami wygladu powierzchni 3D\" ") +
           QString("zmodyfikowany o mozliwosc dokonywania przekrojow. Stanowi on projekt zaliczeniowym autorstwa studentow: <br />")+
           QString("Michala Kluski  i Macieja Kucharskiego <br>") +
           QString("realizowanego w ramach przedmiotu \"Wizualizacja w systemach biomedycznych\"<br/>AGH Czerwiec A.D.2016<br />")+
           QString("<hr><br>Osiagniete cele:<br /><ul>") +
           QString("<li>Wyswietla siatki obiektow 3D.</li>") +
           QString("<li>Pozwala na ustawianie i zapisywanie/wczytywanie nastaw renderu powierzchni obiektow 3D.</li>") +
           QString("<li>Posiada widget do sterowania generacja przekrojow obiektu.</li>") +
           QString("<li>Zapisuje serie obrazow PNG przedstawiajacych przekroje obiektow 3D.</li>") +
           QString("<li>Pieknie dziala \\(^.^)/ .</li></ul>");

    mssgbox.information(this,"Informacje o programie",info);
//tytułem projektu, krótkim celem i założeniami projektu, danymi autora oraz nazwą przedmiotu, rok i miesiąc wykonania

}

/*!
 * \brief MainWindow::~MainWindow() - destruktor klasy MainWindow
 */

MainWindow::~MainWindow()
{
    ren->Delete();

    delete ui;
}
