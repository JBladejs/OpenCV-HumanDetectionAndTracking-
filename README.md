# OpenCV-HumanDetectionAndTracking-
Program ten ma za zadanie śledzić wybrane sylwetki użytkownika.
Zaimplementowane jest to na trzy sposoby:
1. Wskazanie przez użytkownika sylwetki do śledzenia, po czym ta jest śledzona przez znajdujący się w openCV tracker (szybko i najdokładniej);
2. Wskazanie przez użytkownika sylwetki do śledzenia, po czym ta jest śledzone przez znajdujący się w openCV HOG detector oraz zaimplementowaną do tej potrzeby klasę "PersonTracker" (wolno i niezbyt dokładnie);
3. Wykrywanie i śledznie osoby na podstawie wyćwiczonym na niej własnym Cascade Classifierze oraz klasie "PersonTracker" (najszybiciej i dokładnie).

W punkcie 2 i 3 w każdej klatce wykrywane są osoby, a prostokąty je opisujące są przekazywane do "PersonTrackera". Ten mając listę prostokątów z poprzedniej klatki ustala czy nowo otrzymane prostokąty to jedne z wykrytych już wcześniej (jeśli ich środek ciężkości znajudje się w prostokątach z poprzedniej klatki). Jeżeli jendka tak nie jest to te prostokąty są liczone jako nowe osoby.
Jeżeli prostokąt nie został zaaktualizowany od 10 klatek (nie pojawił się żaden inny prostokąt ze środkiem ciężkości w jego obrębie) to jest on usuwany.

Użytkownik sam wybiera metodę śledzenia po czym może je obserwować na ekranie. Sglądając śledzenie sylwetek może on wcisnąć następujące klawisze:
* esc - zamyka okno i wraca do wyboru metody;
* t - zatrzymuje lub wznawia śledzenie;
* n - sylwetki są wykrywane (lub wskazywane) na nowo;
* d - włącza tryb debugowania i pokazuje okno z parametrami.
