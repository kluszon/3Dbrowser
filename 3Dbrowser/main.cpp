#include "mainwindow.h"
#include <QApplication>

/*!
\mainpage Przeglądarka obiektów polydate "GUT"
\author Michał Kluska & Maciek Kucharski
\date 29.06.16
\version 1.0

\section Cel Cel
*
*Temat projektu: "Przeglądarka obiektów siatkowych 3D z widgetem do sterowania współczynnikami wyglądu powierzchni 3D"
*
* Celem projektu była realizacja w pełni działającej aplikacji w framwork-u Qt z wykorzystaniem bibliteki do wizualizacji danych VTK. Realizacja miała obejmować założenia podstawowe
* oraz dodatkowe z uwagi na realizację projektu w zespole dwuosobowym.
*
* Aplikacja miała mieć możliwość pracy z obiektami typu polydata. Użykownik ma posiadać pełną kontrolę nad parametrami renderu powierzchni zgodnie z modelem Phonga. Ponadto aplikacja
* ma umożliwiać krojenie obiektu na plastry o zadanej przez użytkownika grubości oraz zapis do pliku wyników wytworzonych przekrojów.
*
* Poza częścią zwiazaną z tematem projektu, tworzony program powinien umożliwiać wykonywanie zrzutów ekranu aplikacji oraz zapis ich na dysku. Inne wymagane elementy to: zakładka
* "O programie", menu do obsługi operacji na plikach (np. otwieranie plików wejściowych, zapisywanie wizualizacji, pomoc), działające automatyczne rozmieszczanie widgetów w oknie programu
* (musi on dostosowywać rozmieszczenie i rozmiar elementów GUI przy zmianie rozmiarów okna), "dymki" (podpowiedzi) objaśniające przeznaczenie elementów interfejsu użytkownika, wymyślone,
* narysowane w programie Inkscape i umieszczone w aplikacji logo programu, a także wymyślona i zaimplementowana nazwę programu.
*
* Tworzona aplikacja w przyszłości może być podstawą do wytworzenia narzędzia przeznaczonego dla artystów, które umożliwi drukowanie wycinków zeskannownaych obiektów w technologi 3D.
* Byłoby to znaczne uproszczenie ich pracy, szczególnie w pracy rzeźbiarskiej.
*
\section Zalozenia Założenia projektu:
*
* -# program wczytuje obiekty typu PolyDate z plików STL, OBJ lub innych danych typu PolyData
* -# opracowanie widgetu Qt do sterowania współczynnikami modelu oświetlenia Phonga:
*   - współczynnik rozproszenia, rozbłysku, mocy rozbłysku
*   - zmiana koloru obiektu
*   - podgląd nastaw
*   - możliwość zapisu i odczytu nastaw
* -# opracowanie widgetu Qt umożliwiającego wizualizację i krojenie obiektów 3D w sposób określony przez użytkownika.
*   - automatyczne krojenie obiketu
*   - zapis wycinków do pliku
*   - wizualizacja krojenia
* -# Dodatkowo
*   - wykonywanie i zapis zrzutów ekranu
*   - zakładka "O programie"
*   - menu do obsługi funkcji programu (otwieranie plików wejściowych, zapisywanie wizualizacji, pomoc)
*   - dymki z podpowiedziami objasniającymi przeznaczenie elementów interfejsu
*   - wykonanie loga programu
*   - wymyślona nazwa programu
*
\section Sposob Sposób realizacji:
*
* .
* W celu lepszego zobrazowania osiągnętych efektów, na końcu dokumentacji załączono "Dodatek" zawierający zestawienie zrzutów ekranu. Przedstawiają one poszczególne etapy wykorzystania
* programu oraz wycinek z serii wygenerowanych przekrojów.
*
* Zwarzając na chęć dodatkowego rozwinięcia zdolności programisycznych, jak i sposobów kontroli kodu, zdecydowano się na skorzystanie z rozproszonego systemu kontroli wersji Git,
* w celu podziału prac i równoległego rozwoju aplikacji. Równocześnie skorzystano z serwisu Bitbucket, który pozwala na darmowe korzystanie z systemu kontroli wersji, a zarazem jest
* profesjonalnym rozwiazaniem nawet dla dużych zespołów.
*
* Podstawową kwestą okazało się wgrywanie modeli 3D. Wczytywanie obiektów typu polydate zostało rozwiązane za pomocą szablonu funkcji do tworzenia odpowiednich obiektów do czytania
* plików .stl i .vtl (vtkSTLReader oraz vtkXMLPolyDataReader). Cały proces, od kliknięcia przez użytkownika przycisku w menu programu, po wyświetlenie wybranego obiektu w oknie programu,
* obsługiwany jest przez kilka metod do analizy danych o pliku oraz tworzenia elementów i wyświetlania ich na wirtualnej scenie.
*
* Inną sprawą, którą uznano za podstawową, była wizualizacja wgranych danych. W tym celu, w początkowej fazie, spróbowano wykorzystać biblioteke QVTKWidget. Niestety, problemy
* z wersją Qt, uniemożliwiły jej zastosowanie wprost. Wersja 5.2 nie wspera tego widgetu, dlatego też, w finalnej wersji użyto Qt 4.8. Sam widget został stworzony poprzez transformowanie
* obektu QWidget, dziedzicząc po wspominanej bibliotece.
*
* Wyświetlane obiekty stanowią typowy przykład przeływu danych wykorzystujący strukturę biblioteki VTK. Zawarte zostały one w szablonie readObject(), służącym do otwierania plików
* polydata. Wczytany obiekt jest przypisywany do źródła, opisanego jako reader. Następnie dane przechodzą do mappera, aktora oraz rendererów obiektu i okana. Ostatnie dwa są szczególnie
* potrzebne do powiązania wyświetlanego obrazu z interfejsem urzytkownika. W górnej belce przygotowano także przyciski do wyboru sposobu reprezentacji samych danych. Domyślnie załadowany
* model renderowany jest w postaci bryłowej, z bazowymi ustawieniami interpolacji powierzchni według modelu Phonga. Przyciski pozwalają na zmianę sposobu wyświetlania danych umożliwiając
* przedstawienie w postaci: chmury punktów - przycisk "Points View"(Zrzut 5); siatki - przycisk "Wireframe View"(Zrzut 4); bryłowy - "Surface View"(Zrzut 3); lub poprostu wyłaczenie
* podglądu obiektu - przycisk "On/Off object".
* Interakcję z wizualizowanym modelem, za pomocą myszy, zapewnia obiekt-interaktor powiązany z QVTKWidget-em.
*
* Manipulację parametrami renderu wgranych brył zapewnia widget "Sterowanie Model Phonga"(Zrzut 6). Odwołuje się on do podklasy vtkProperty obiektu aktora, a dzięki indykatorom umiejscowionym
* przy odpowiednich suwakach, na bieżąco wskazuje wartość poszczególnych nastaw. Użytkownik ma możliwość zmainy podstawowych parametrów renderu według modelu Phonga, tj. rozproszenia(diffuse),
* rozbłysku(specular), mocy rozbłysku(specular power) oraz koloru. Podstawowe warości tych parametrów to kolejno: 1, 0, 1, biały. Wyświetlane wartosci podawane są w procentach
* i są przeskalowywane do wymiaru przyjmowanego przez metody klasy vtkProperty.
* W widgecie zaimplementowano także możliwość zapisu nastaw. Jest ona szczególnie przydatna, przy wielokrotnych zmianach wgrywanego obiektu, gdyż program resetuje ustaweina renderu do
* nastaw podstawowych(typowych dla surowego wyświetlania wizualizacji w VTK). Mając na względzie multipatformowość rozwijanej aplikacji zdecydowano się na wykorzystanie biblioteki
* QSettings. Wytwarza ona obiekt zdolny do gromadzenia dowolnych rodzajów danych, zapisując je w dostosowanym do systemu operacyjnego pliku konfiguracyjnym. Wspomniane dostosowanie
* odbywa się automatycznie, przy tworzeniu pliku z ustawieniami.
*
* Krojenie obiektów wykonane zostało przy użyciu klasy vtkCutter, dzięki której było możliwe stworzenie płaszczyzn krojących oraz obliczenie przekroju obiektu. W oknie aplikacji
* dokonywana jest wizualizacja krojenia o zadanych parametrach przez użytkownika. Użytkownik, za pomocą wyszczególnionego widgetu "Sterowanie krojenie obiektu" (Zrzut 8), może dostosować
* krok krojenia (grubość plastrów) oraz grubość noża. Dodatkowo, ma on pełną kontrolę nad ustwaieniem płaszczyzny tnącej, poprzez podawania współrzędnych składowych x,y,z.
* Aplikacja została wyposarzona w zestaw funkcji do automatycznego krojenia całego obiektu typu polydata. W skład zestawu wchodzi metoda do tworzenia poszczególnych przekrojów oraz
* wizualizacji swojego działania, a także metoda do zapisu stanu widgetu do pliku .png w lokalizacji, z której został wczytany model bazowy. W Zrzutach 9-19 przedstawiono przykładową
* sekwencję uzyskanych przekrojów.
*
* Obydwa widgety zostały zaprojektowane w taki sposób, by były intuicyjne dla użytkownika. Równocześnie zabezpieczono je tak, aby uaktywaniały się dopiero w momencie poprawnego
* załadowania modelu do programu (Zrzut 1,2). Dodatkowo, w celu zwiększenia konfortu użytkowania, widgety można odpiąć od głównego okna programu za pomocą odpowiedniego przycisku,
* znajdującego się po prawej, górnej stronie każdego z nich.
*
*
\section INSTRUKCJA Instrukcja obsługi:
*
* W nijenszym rodziale zostanie przedstawione przykładowe wykorzystanie aplikacji w celu uzyskania przekrojów w płaszczyźnie strzałkowej, gotowych do wydruku.
* -# Uruchom aplikację GUT.
* -# W menu głównym programu wybierz: Plik > Wczytaj obiekt(Zrzut 1).
* -# W okienku kontekstowym wybierz lokalizacje i plik z modelem, z którego chcesz wygenerować przekroje.
*       - Pamiętaj, że przekroje będą zapisywane w tym samym folderze.
* -# Jeżeli poprawnie wybrałeś plik, w aplikacji uaktywnią się 2 widgety (Zrzut 2):
*       - Widget do sterowania parametrami renderu powierzchni,
*       - Widget do tworzenia przekrojów.
* -# W celu manipulacji ustawienia orientacji obiektu , kliknij kursorem na wyświetlony obraz i przesuń mysz w taki sposób, aby uzyskać zadowalający kąt.
* -# Możesz zmienić paramety renderu za pomocą widgetu "Sterowanie Model Phonga" (Zrzut 7).
* -# Zaznacz "Włącz/Wyłacz krojenie obiektu" w widgecie "Sterowanie krojenie obiektu".
*       - W tym momencie uaktywnią się pozostałe kontrolki w tym widgecie.
* -# Ustaw pożądzane wartości grubości ostrza, grubość plastra oraz współrzędne płaszczyzny tnącej.
*       - W celu uzyskania płaszczyzny sztrzałkowej należy ustawić wspołrzedną x w maksymalnej wartosci, a pozostałe na minimum.
* -# Zwizualizuj efekt wybranych nastaw klikając przycisk "Podgląd".
*       - W tym momencie na ekranie podglądu obiektu wyrysowane zostaną żółte linie, reprezentujące wspólną granicę wgranego modelu oraz płaszczyzny tnącej.
*       - Jeżeli osiągnięty rezultat nie jest zadowalający, zmodyfikuj nastawy powtarzając kroki 8 i 9.
* -# Uruchom algorytm generujący przekroje, klikając przycisk "Krój".
*       - Na ekranie wizualizacji wyświetlona zostanie animacja cięcia obiektu. Równocześnie aplikacja zostanie zablokowana do końca działania algorytmu.
* -# Po zakończeniu generowania przekrojów, w dolnym pasku aplikacji zostanie wyświetlona informacja o końcu obliczeń. Utworzone przekroje znajdyją się w folderze z wgranym modelem.
*
*
*
\section WNIOSKI Wnioski:
*
* .
* Zadanie postawione w temacie pracy zostało wykonane. Udało się zrealizować w satysfakcjonujący sposób wszystkie cele i założenia, a także poboczne elementy, jak np. możliwość zapisywania
* zrzutów ekranowych czy logo programu. Stworzona apliakcja jest łatwa w obsłudze i posiada przyjazny, intuicyjny dla użytkownika interfejs.
*
* Z uwagi na to, iż jest to pierwsza wersja programu, posiada ona duże możliwości rozwoju, w zależności od zapotrzebowania użytkownika. Kolejnym etapem projektu powinna być weryfikacja założeń,
* przeprowadzona poprzez udostępnienie aplikacji artystom-rzeźbiarzom, stanowiących docelową grupę użytkowników. Jako zespół jesteśmy otwarci na propozycje rozwoju aplikacji i dostosowawania jej
* do bardziej szczególowych zastosowań.
*
* Równocześnie starano się dopracować jak najbardziej zaprezentowane rozwiązania i wyeliminować wszystkie wady. Kontynułacja pracy nad aplikacją zależy od zainteresowania osób widzących jej
* zastosowanie w konkretnej pracy.
*
* Relacjonując samą pracę nad programem, wyciągnięto szereg wniosków. Jako pierwszą należy omówić kwestie środowiska Git. Praca z rozproszonym systemem kontroli wersji w przedstawionej wcześniej
* konfiguracji przebiegała sprawnie, aż do momentu łaczenia branch-y. Na zaawansowanym etapie projektu Git zupełnie nie radził sobie z merge-owaniem różnych części kodu. Niejednokrotnie zgłaszał
* konflikty w miejscach projektu, w których go nie było. Skutkowało to tym, iż połączenie ich w ten sposób stało się niemożliwe. Do samego merge-owania zostało zastosowane narzędzie graficzne DiffMerge.
* Być może konfiguracja i wykorzystanie narzędzi wbudowanych w Qt Creator, w czasie z pracy z projektem, umożliwiłoby uniknięcie takiego problemu.
*
* Drugim problemem na jaki natroafiono były kłopoty z działaniem klasy vtkOBJReader, która nie chciała współpracować z Qt. Mimo implementacji wczytywania obiektu, zgodnie ze specyfikacją w dokumentacji
* klasy, w trakcie wczytywania obiektów .obj aplikacja przerwywała niespodziewanie prace. Po przyjrzeniu się dokłądniej temu zagadnieniu okazało się, iż problem występuje w strukturze klasy i może być
* spowodowany barkiem zgodności z użytą wersją Qt. Biorąc pod uwagę ten fakt oraz brak rozwiązań problemu wśród społeczności korzystajacej z Qt i VTK zaniechano wykorzystania powyższej klasy.
*
* Następnym problemem okazała się kwestia zapamiętywania ustawień renderu modelu. Zastosowanie wspominanej wcześniej klasy QSettings zapewniło wbudowaną multiplatformowość, jednakże utrudniło odczyt
* warości, zwłaszcza w przypadku próby odtworzenia wybranego koloru. Problem ten rozwiązano stosując dodatkową klase QVariant. Zaliczają się do niej obiektey, które w rzeczyywistości są wytwarzane w klasie
* QSettings. Do tego, zastosowano rzutowanie typów wbudwanych, co pozwoliło na przesłanie odpowiednich wartosci do panelu sterowania nastawami, a w przypadku samego koloru, zastosowano rozdzielenie barwy na
* zestaw składowych RGB.
*
* Nadal nie roziwiązanym problemem pozostaje kwestia "przeszkadzania" aplikacji. W momenci uruchomienia automatycznego algorytmu krojącego, należy pozostawić aplikacię w takim stanie w jakim sie znajduje(nie przesuwać
* okna). Jest to powiązane ze sposobem generowania przekroju. W tym czasie aplikacja dokonuje zrzutu ekranowanego prezentowanego przekroju. Jeżeli w jakiś sposób ją przesuniemy, to przesunięcie zostanie zarejesteowane na
* przekroju. Nie udało się zaimplementować innego sposobu eksportowania wizualizacji.
*
* Aplikacja została przetestowana na dwóch różnych ustawieniach systemu operacyjnego Linux. Pierszym z nich, była dystrybucja Mint, stanowiąca normalny system operacyjny komputera. Aplikacja zachowywała
* pełną sprawność i funcjonalność. Drugą z nich, była dystrybucja Ubuntu 14.04, działajaca jako maszyna wirtualna na komputezrze z systemem operacyjnym Windows 7. W tym przypadku, odnotowano problemy ze
* skalowaniem okienek - czasami aplikcja nie mieściła się na ekranie, co uniemożliwiało np. dokonanie podglądu ustawień tworzenia przekrojów. Prawdopodobnie, problemy te sa powiązane z za małą rozdzielczością
* ekranu wirtualnej maszyny, w stosunku do rzeczywistej rozdzielczości komputera. Dlatego też, by w pewien sposób obejść ten dość niespodziewany problem, zdecydowano się właśnie na dokowane widgety.
* Rozłączenie niektórych z nich, pozwala na zochowanie pełnej funkcjonalności. Rozwiązanie to stanowi swoiste, dodatkowe zabezpieczenie, przy wystąpieniu takiego problemu.
*
* W kolejnych rozdziałch została zaprezentowana dokumentacja kodu, zawierająca opisy działania poszczegónych metod i elementów zawrtych w kodzie.
* Została ona wygenrowania autoamatycznie w programie Doxygen, w oparciu o zamieszczony kod programu GUT.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
